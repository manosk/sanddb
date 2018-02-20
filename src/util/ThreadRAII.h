//
// Created by manolee on 11/14/17.
//

#ifndef SANDDB_THREADRAII_H
#define SANDDB_THREADRAII_H

#include <thread>

/**
 * Pattern from Item 37 of 'Effective Modern C++'
 *
 * Reason of existence:
 * If the destructor for a joinable thread is invoked, execution of the program is terminated
 */
class ThreadRAII {
public:
    enum class DtorAction { join, detach };

    ThreadRAII(std::thread&& t, DtorAction a)
            : action(a), t(std::move(t)) {} // in dtor, take action a on t

    ~ThreadRAII()
    {
        if (t.joinable()) { // see below for joinability test
            if (action == DtorAction::join) {
                t.join();
            } else {
                t.detach();
            }
        }
    }
    std::thread& get() { return t; }

    ThreadRAII(ThreadRAII&&) = default;
    ThreadRAII& operator=(ThreadRAII&&) = default;

private:
    DtorAction action;
    std::thread t;
};


#endif //SANDDB_THREADRAII_H
