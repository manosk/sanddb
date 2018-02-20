//
// Created by manolee on 11/16/17.
//

#ifndef SANDDB_LOCK_FREE_STACK_HP_H
#define SANDDB_LOCK_FREE_STACK_HP_H

#include <iostream>
#include <cstdio>
#include <atomic>
#include <memory>
#include <thread>
#include <sstream>

#include "hazard_pointer.h"

std::atomic<void*>& get_hazard_pointer_for_current_thread();
std::atomic<void*>& get_hazard_pointer_for_current_thread()
{
    // The first time each thread calls this function, a new instance of
    // hp_owner is created. The constructor for this new instance then
    // searchs through the table of owner/pointer pairs looking for an entry
    // without an owner. It uses compare_exchange_strong() to check for an
    // entry without an owner and claim it in one go.
    //
    // Once the hp_owner instance has been created for a given thread, further
    // accesses are much faster because the pointer is cached, so the table
    // doesn't have to be scanned again.

//    printf("get hazard pointer for current thread, thread id: %d\n", std::this_thread::get_id());
    std::cout << "get hazard pointer for current thread, thread id: "
              << std::this_thread::get_id()
              << std::endl;
    thread_local static hp_owner hazard;
    return hazard.get_pointer();
}

/**
 * PRETTY EXPENSIVE.
 * Lots of atomic operations.
 * For every node in the 'list to be removed',
 * we may have to compare its 'p' against max_hazard_pointers
 *
 * Atomic ops can be 100x slower than plain reads
 */
bool outstanding_hazard_pointers_for(void *p);
bool outstanding_hazard_pointers_for(void *p)
{
    for (unsigned i = 0; i < max_hazard_pointers; ++i) {
        if (hazard_pointers[i].pointer.load() == p) {
            return true;
        }
    }
    return false;
}

template <typename T>
void do_delete(void* p)
{
    delete static_cast<T*>(p);
}

struct data_to_reclaim
{
    void* data;
    std::function<void(void *)> deleter;
    data_to_reclaim* next;

    template<typename T>
    data_to_reclaim(T* p)
            : data(p)
            , deleter(&do_delete<T>)
            , next(0)
    {}

    //Not called when adding nodes to the list;
    //it's called when there are no more hazard pointers to that node.
    //This is the responsibility of delete_nodes_with_no_hazards()
    ~data_to_reclaim()
    {
        deleter(data);
    }
};

std::atomic<data_to_reclaim*> nodes_to_reclaim;
void add_to_reclaim_list(data_to_reclaim* node);
void add_to_reclaim_list(data_to_reclaim* node)
{
    node->next = nodes_to_reclaim.load();
    //we may not be the only ones adding stuff to reclaim list
    while (!nodes_to_reclaim.compare_exchange_weak(node->next, node));
}

//No reason to restrict to stacks -> function template
template<typename T>
void reclaim_later(T* data)
{
    add_to_reclaim_list(new data_to_reclaim(data));
}

void delete_nodes_with_no_hazards();
void delete_nodes_with_no_hazards()
{
    // first claims the entire list of nodes to be reclaimed;
    // ensures that this is the only thread trying to reclaim
    // this particular set of nodes; other threads are now free
    // to add futher nodes to the list or even try to reclaim
    // them without impacting the operation of this thread.
    data_to_reclaim* current = nodes_to_reclaim.exchange(nullptr);

    while (current != nullptr) {
        data_to_reclaim* const next = current->next;

        // check each node in turn to see if there are any outstanding
        // hazard pointers.
        if (!outstanding_hazard_pointers_for(current->data)) {
            // if there aren't, delete the entry
            delete current;
        }
        else {
            // otherwise, just add the item back on the list for
            // reclaiming later
            add_to_reclaim_list(current);
        }
        current=next;
    }
}

template <typename T>
class lock_free_stack_hp
{
private:
    struct node
    {
        std::shared_ptr<T> data;
        node *next;

        node(const T& data_)
                : data(std::make_shared<T>(data_))
        {}
    };

    std::atomic<node *> head;

public:
    lock_free_stack_hp() : head(nullptr)
    {}

    void push(const T& data)
    {
        node *const new_node = new node(data);
        new_node->next = head.load();
        while (!head.compare_exchange_weak(new_node->next, new_node));
    }

    std::shared_ptr<T> pop()
    {
        //Set hazard pointer part.
        std::atomic<void*>& hp = get_hazard_pointer_for_current_thread();
        node* old_head = head.load();

        //Use STRONG comparison:
        //If we used the weak version, spurious failures would result
        //in unnecessary, expensive work (the storing and validation of hp)
        do {
        //every time you reload old_head (bc cmp failed),
        //you need to update the hazard pointer
        //before you dereference the freshly read pointer value.
            node* temp;
            do {
                // loop until you've set the hazard pointer to head
                temp = old_head;
                hp.store(old_head);
                old_head = head.load();
            } while (old_head != temp);
        }
        while (old_head != nullptr &&
               !head.compare_exchange_strong(old_head, old_head->next));

        //The node is now ours (new 'pop()' calls will see the new head);
        //can clear hazard pointer
        hp.store(nullptr);
        std::shared_ptr<T> res;
        if (old_head != nullptr) {
            res.swap(old_head->data);
            if (outstanding_hazard_pointers_for(old_head)) {
                // check for hazard pointers referencing
                // a node before you delete it
                // (i.e., from concurrent pop() calls that had set a hp
                // before we reached this point.
                reclaim_later(old_head);
            }
            else {
                delete old_head;
            }
            //the 'periodic' gc part
            delete_nodes_with_no_hazards();
        }
        return res;
    }
};

void push_hp(lock_free_stack_hp<int>* s);
void push_hp(lock_free_stack_hp<int>* s)
{
    std::stringstream ss;
    for (int i = 0; i < 10; ++i) {
        ss << "pushing " << i << std::endl;
        std::cout << ss.str();
        ss.str("");
        s->push(i);
    }
}

void pop_hp(lock_free_stack_hp<int>* s);
void pop_hp(lock_free_stack_hp<int>* s)
{
    std::stringstream ss;
    int count = 0;
    std::shared_ptr<int> e;
    while (count < 10) {
        e = s->pop();
        if (e) {
            ss << "popping " << *e << std::endl;
            std::cout << ss.str();
            ss.str("");
            ++count;
        }
    }
}
#endif //SANDDB_LOCK_FREE_STACK_HP_H
