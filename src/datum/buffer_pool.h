//
// Created by manolee on 11/10/17.
//

#ifndef SANDDB_BUFFER_POOL_H
#define SANDDB_BUFFER_POOL_H

#include "common/common.h"
#include "datum/data_vector.h"
//#include "array_vector.h"
#include "structures/stack_sp.h"

constexpr int MAXBUFFERPOOLSIZE = 10;//1024;


enum class BufferState {
    EMPTY = 0, FULL, PRODUCED, CONSUMED
};

/*
 * Each buffer expected to have info such as
 * - data
 * - state
 * - production_ts
 * - write_pos
 * - consumer_id
 * - consumers_done
 **/
/**
 *
 * @tparam DT datatype -> will typically be a vector
 * @tparam VT vectortype
 *
 * //TODO Not sure the use of enable_if will do what I intend. Test it.
 *
 */
template<typename DT>
        //,
        //typename VT,
        //Maybe I must move this to a static assert
        //typename = typename std::enable_if<std::is_base_of<DataVector<DT>, VT>::value>
//>
class Buffer {
private:
    unique_ptr<DT> data;
    BufferState state;
    int producer_id;
    int consumers_done;
public:
    Buffer()    {
        //static_assert(std::is_base_of<DataVector<DT>, VT<DT>>::value, "VT not derived from DataVector");
        data = make_unique<DT>();
        state = BufferState::EMPTY;
        producer_id = -1;
        consumers_done = 0;
    }

    BufferState& getState() {
        return state;
    }
    void setState(BufferState st) {
        this->state = st;
    }
    //TODO Rest of the constructors / destructors / movers
};

/**
 * Treat as singleton
 *
 * Buffer pool per datatype (&vector type actually)
 *
 * Currently serves exchange logic
 *
 * TODO eventually consider thread safety, either here or at the code calling the pool
 */
template<typename DT>
class BufferPool {

    using BufferPtr = typename std::shared_ptr<Buffer<DT>>;
private:

    stack_sp<Buffer<DT>> pool;
    int maxElems;
    std::atomic_int availableElems; //Buffers still available for operators

    //int currElems;
    mutable std::mutex lock; //use to sync. inserts and deletions from pool
    mutable std::condition_variable all_occupied;


    //Forbid copy constructor. Also: Default constructor should suffice
    BufferPool(const BufferPool&) = delete;


public:
    BufferPool()    : maxElems(MAXBUFFERPOOLSIZE), availableElems(MAXBUFFERPOOLSIZE)   {
        for(int i = 0; i < maxElems; i++)   {
            //pool_.push_front(std::make_shared<Buffer<DT, VT>>());
            auto tmp = make_unique<Buffer<DT>>();
            pool.push(std::move(tmp));
        }
    }

    void freeBuffer(BufferPtr buf) {
        buf->setState(BufferState::EMPTY);
        pool.push(buf);

        availableElems++;
        all_occupied.notify_one();
    }

    BufferPtr getBuffer()   {

        //Make sure there are available buffers
        //Policy atm: LOCK till sb returns the buffer it holds
        std::unique_lock<std::mutex> g(lock); //Lock mutex
        all_occupied.wait(g, [this](){return availableElems.load() != 0;});

        BufferPtr buf = pool.pop();

        availableElems--;
        return buf;
    }

};

//
///**
// * For now, don't recycle buffers; create new one when there's a request
// *
// * Treat as singleton
// *
// * Try having a buffer pool per datatype
// *
// * TODO eventually consider thread safety, either here or at the code calling the pool
// */
//template<typename DT, typename VT>
//class BufferPool {
//
//    using BufferPtr = typename std::shared_ptr<Buffer<DT, VT>>;
//    using BufferIter = typename std::forward_list<BufferPtr>::iterator;
//private:
//    std::forward_list<BufferPtr> pool_;
//    int maxElems;
//    int currElems; //Buffers provided to operators
//    mutable std::mutex lock; //use to sync. inserts and deletions from pool
//    mutable std::condition_variable all_occupied;
//
//
//    //Forbid copy constructor. Also: Default constructor should suffice
//    BufferPool(const BufferPool&) = delete;
//
//
//public:
//    BufferPool()    : maxElems(MAXBUFFERPOOLSIZE), currElems(0)   {
//        for(int i = 0; i < maxElems; i++)   {
//            pool_.push_front(std::make_shared<Buffer<DT, VT>>());
//        }
//    }
//
//    //TODO optimization: maintain first empty buff. Most likely replace list with (lock-free) stack
//    //TODO consider finer-grained locking logic
//    BufferIter getBuffer()   {
//        //std::lock_guard<std::mutex> g(m);
//        std::unique_lock<std::mutex> g(lock); //Lock mutex
//
//        //Make sure there are available buffers
//        //Policy atm: Lock till sb returns the buffer it holds
//        all_occupied.wait(g, [this](){return currElems != maxElems; });
//
//        BufferIter it = std::begin(pool_);
//        for(; it != std::end(pool_); it++)   {
//            BufferPtr b = *it;
//            if(b->getState() == BufferState::EMPTY)    {
//                b->setState(BufferState::PRODUCED);
//                currElems++;
//                break;
//            }
//        }
//
//        g.unlock(); //unlock mutex
//
//        if(it == std::end(pool_))   {
//            //Should not be possible
//            throw std::logic_error("Conditional var logic should have prevented reaching end-of buffer pool");
//        }
//
//        return it;
//    }
//
//    void freeBuffer(BufferIter it)   {
//        //std::lock_guard<std::mutex> g(lock); //Lock mutex
//        std::unique_lock<std::mutex> g(lock);
//        BufferPtr b = *it;
//        b->setState(BufferState::EMPTY);
//        currElems--;
//        all_occupied.notify_one();
//        g.unlock();
//    } //unlock mutex
//
//
//};



#endif //SANDDB_BUFFER_POOL_H
