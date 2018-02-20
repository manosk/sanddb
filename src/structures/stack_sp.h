//
// Created by manolee on 11/17/17.
//

#ifndef SANDDB_STACK_SP_H
#define SANDDB_STACK_SP_H

#include <thread>
#include <memory>
#include <atomic>
#include <iostream>

/**
 * Pros:
 * Way simpler GC logic than the pure lock-free implementations
 *
 * Cons:
 * Delegates GC to shared_ptr internals.
 * shared_ptr has a locking implementation on most platforms
 *
 */
template <typename T>
class stack_sp
{
private:
    struct node
    {
        //TODO I think we don't need to hold data in a shared_ptr
        std::shared_ptr<T> data;
        std::shared_ptr<node> next;

        node(T const& data_)
                : data(std::make_shared<T>(data_))
        {}

        node(std::unique_ptr<T> data_)
                : data(std::move(data_))
        {}

        node(std::shared_ptr<T> data_)
                : data(data_)
        {}
    };

    std::shared_ptr<node> head;
//    std::atomic<std::shared_ptr<node>> head;
public:
    void push(T const& data)
    {
        std::shared_ptr<node> const new_node = std::make_shared<node>(data);
        new_node->next = std::atomic_load(&head);
        while (!std::atomic_compare_exchange_weak(&head,
                                                  &new_node->next, new_node));
    }
    void push(std::shared_ptr<T> data)
    {
        std::shared_ptr<node> const new_node = std::make_shared<node>(data);
        new_node->next = std::atomic_load(&head);
        while (!std::atomic_compare_exchange_weak(&head,
                                                  &new_node->next, new_node));
    }
    void push(std::unique_ptr<T> data)
    {
        std::shared_ptr<node> const new_node = std::make_shared<node>(std::move(data));
        new_node->next = std::atomic_load(&head);
        while (!std::atomic_compare_exchange_weak(&head,
                                                  &new_node->next, new_node));
    }
    std::shared_ptr<T> pop()
    {
        std::shared_ptr<node> old_head = std::atomic_load(&head);
        while (old_head && !std::atomic_compare_exchange_weak(&head,
                                                              &old_head, old_head->next));
        return old_head ? old_head->data : std::shared_ptr<T>();
    }
};

//void push_sp(stack_sp<int>* s);
//void push_sp(stack_sp<int>* s)
//{
//    for (int i = 0; i < 10; ++i) {
//        printf("pushing %d\n", i);
//        s->push(i);
//    }
//}
//
//void pop_sp(stack_sp<int>* s);
//void pop_sp(stack_sp<int>* s)
//{
//    int count = 0;
//    std::shared_ptr<int> e;
//    while (count < 10) {
//        if (e = s->pop()) {
//            using namespace std::chrono_literals;
//            std::this_thread::sleep_for(1s);
//            printf("popping %d\n", *e);
//            ++count;
//        }
//    }
//}

//int main()
//{
//    std::shared_ptr<int> sp = std::make_shared<int>(1);
//    //printf("std::atomic_is_lock_free(std::shared_ptr): %d\n",
//    //        std::atomic_is_lock_free(&sp));
//    stack_sp<int> s;
//    std::thread t1(push, &s);
//    std::thread t2(pop, &s);
//    t1.join();
//    t2.join();
//    return 0;
//}


#endif //SANDDB_STACK_SP_H
