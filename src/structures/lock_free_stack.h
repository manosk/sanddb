//
// Created by manolee on 11/15/17.
//

#ifndef SANDDB_LOCK_FREE_STACK_H
#define SANDDB_LOCK_FREE_STACK_H

#include "common/common.h"

/**
 * Lock-free implementation from https://github.com/subjam/concurrency-in-action
 *
 * @tparam T the type of data stored in the stack
 */
template<typename T>
class lock_free_stack
{
private:
    struct node
    {
        //Use ptr to data to guarantee exception safety;
        //return-by-value at pop() time would be problematic
        //in the presence of exceptions
        std::shared_ptr<T> data;
        node *next;

        //'data' is a reference to a const T
        node(const T& data_):
                //create shared_ptr for NEWLY allocated T
                data(std::make_shared<T>(data_))
        {}
    };

    std::atomic<node*> head;

    /**
     * GC logic, side-effect of using a lock-free implementation
     *
     * Logic to reclaim nodes 'deleted' as a result of pop()
     *
     * In high-load situations, there may never be such a quiescent state, because other
     * threads have entered pop() before all the threads initially in pop() have left.
     * Under such a scenario, the to_be_deleted list would grow without bounds,
     * and you'd be essentially leaking memory again.
     * If there aren't going to be any quiescent periods,
     * you need to find an alternative mechanism for reclaiming the nodes.
     * The key is to identify when no more threads are accessing a particular node
     * so that it can be reclaimed.
     * By far the easiest such mechanism to reason about is the use of hazard pointers.
    **/
    std::atomic<unsigned> threads_in_pop;
    std::atomic<node*> to_be_deleted;

    static void delete_nodes(node *nodes)
    {
        while (nodes) {
            node *next = nodes->next;
            delete nodes;
            nodes = next;
        }
    }

    void try_reclaim(node *old_head)
    {
        //"am I the only thread currently in pop()?"
        if(threads_in_pop == 1)
        {
            //yes: safe to delete the node just removed (old_head)

            //Claim list of to-be-deleted nodes (Set the to_be_deleted atomic var to nullptr)
//        std::shared_ptr<node> nodes_to_delete = to_be_deleted.exchange(nullptr);
            node *nodes_to_delete = to_be_deleted.exchange(nullptr);

            //am I STILL the only thread in pop()?
            if(!--threads_in_pop)
            {
                //yes: delete chain of pending nodes
                delete_nodes(nodes_to_delete);
            }
            else if(nodes_to_delete)
            {
                // not safe to reclaim the nodes, so if there are any,
                // you must chain them back onto the list of nodes pending deletion.
                // This can happen if there are multiple threads accessing the
                // data structure concurrently. Other threads might have
                // called pop() in between the first tet of thread_in_pop and
                // the "claiming" of the list, potentially adding new nodes to
                // the list that are still being accessed by one or more of
                // those other threads.
                chain_pending_nodes(nodes_to_delete);
            }
//        if(!old_head.unique())
//        {
//            std::cout<<"'Old head' node in the deletion list should be independent\n";
//        }
//        old_head.reset();
            delete old_head;
        }
        else
        {
            //no: not safe to delete any nodes, add the node to the pending list
            chain_pending_node(old_head);
            --threads_in_pop;
        }
    }

    void chain_pending_nodes(node *nodes)
    {
        node* last = nodes;
        // traverse the chain to find the end
        while (node* const next = last->next) {
            last = next;
        }
        chain_pending_nodes(nodes, last);
    }

    void chain_pending_nodes(node* first, node* last)
    {
        // replace the next pointer from the last node with
        // the current to_be_deleted pointer
        assert(last != nullptr);
        last->next = to_be_deleted;
        // store the first node in the chain as the new to_be_deleted pointer
        // have to use compare_exchange_weak in a loop here in order to ensure
        // that you don't leak any nodes that have been added by another thread
        while (!to_be_deleted.compare_exchange_weak(
                last->next,first));

    }

    void chain_pending_node(node* n)
    {
        // adding a single node onto the list is a special case where the
        // first node onto the list is a special case where the first node
        // in the chain to be added is the same as the last one.
        chain_pending_nodes(n,n);
    }

public:

    lock_free_stack()
            : head(nullptr)
            , threads_in_pop(0)
            , to_be_deleted(nullptr)
    {}

    /**
     * Sequential logic:
     * 1. Create a new node.
     * 2. Set its next pointer to the current head node.
     * 3. Set the head node to point to it.
     *
     * @param data
     */
    void push(const T& data)
    {
        //std::shared_ptr<node> const new_node = std::make_shared<node>(data);
        node *const new_node = new node(data);

        //'load': nonmodifying query of the value stored in head
        new_node->next = head.load();

        //Wrap over CAS: Ensure that the head pointer still has the same value
        //as the one we stored in new_node->next.
        //If true:  Set head to new_node
        //If false: The comparison failed(for example, because head was modified by another thread);
        //Then, the value supplied as the first parameter (new_node->next)
        //is updated to the current value of head.
        //We therefore don’t have to reload head each time through the loop
        while(!head.compare_exchange_weak(new_node->next,new_node));
    }

    /**
     * Sequential logic:
     * 1. Read the current value of head.
     * 2. Read head->next.
     * 3. Set head to head->next.
     * 4. Return the data from the retrieved node.
     * 5. Delete the retrieved node.
     *
     * @param result
     */
    std::shared_ptr<T> pop()
    {
        ++threads_in_pop;

        node *old_head = head.load();
        //Checking that old_head is not a nullptr before dereferencing
        while(old_head != nullptr   &&
                !head.compare_exchange_weak(old_head,old_head->next));

        std::shared_ptr<T> res;
        //null-check again
        if(old_head != nullptr)    {
            //remove data from node
            //(i.e., extract data from node)
            res.swap(old_head->data);
        }
        try_reclaim(old_head);

        return res; // reclaim deleted nodes if you can
    }
};

#endif //SANDDB_LOCK_FREE_STACK_H
