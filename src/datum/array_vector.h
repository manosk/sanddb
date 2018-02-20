//
// Created by manolee on 11/9/17.
//

#ifndef SANDDB_ARRAY_VECTOR_H
#define SANDDB_ARRAY_VECTOR_H

#include "data_vector.h"
#include "buffer_pool.h"

//Forward declaration -- original in "buffer_pool.h"
//class BufferPool;
//class Buffer;

template<typename T>
class ArrayVector : public DataVector<T> {

    using BufferPtr = typename std::shared_ptr<Buffer<ArrayVector<T>>>;

public:
    virtual size_t size() override { return vec->size(); }
    virtual const T& get(size_t pos) const override { return vec->at(pos); }
    void set(size_t pos, const T &elem) override { vec->at(pos) = elem; }
    virtual VectorType getVectorType() override { return VectorType::ARRAY_CPP; }
    virtual bool isInit() override { return init; }
    virtual void setInit() override { init = true; }

    //Used during buffer pool's construction process
    ArrayVector()   : init(false)   {
        std::cout << "Uninitialized array vector wrapper constructed" << std::endl;
        vec = std::make_shared<std::array<T, VECTORSIZE>>();
    }

    ArrayVector(const std::shared_ptr<std::array<T, VECTORSIZE>> &vec)
            : vec(vec), init(true)//, pool(pool)
    {}

    ArrayVector(const ArrayVector &obj) :
            //vec(std::make_shared<std::array<T, VECTORSIZE>>(*(obj.vec))), init(true)
            vec(obj.vec), init(true)
    {}

    ArrayVector& operator=(const ArrayVector &rhs) {
        if(this != &rhs)    {
            vec = std::make_shared<std::array<T, VECTORSIZE>>(*(rhs.vec));
            init = true;
        }
        return *this;
    }

    virtual ~ArrayVector()  {
        this->vec = nullptr;
        this->init = false;
    }

    //Move
    ArrayVector(ArrayVector&& obj) noexcept {
        this->vec = std::move(obj.vec);
        this->init = false;
        obj.vec = nullptr;
    }

    ArrayVector& operator=(ArrayVector&& rhs) noexcept {
        if(this != &rhs) {
            this->vec = std::move(rhs.vec);
            this->init = rhs.init;
            rhs.vec = nullptr;
        }
        return *this;
    }

    static std::shared_ptr<Buffer<ArrayVector<T>>> getBuffer()    {
        return pool.getBuffer();
    }

    static void freeBuffer(BufferPtr buff)   {
        return pool.freeBuffer(buff);
    }

private:
    std::shared_ptr<std::array<T,VECTORSIZE>> vec;
    bool init;
    static BufferPool<ArrayVector<T>> pool;
};

//Initializing static var
template<typename T> BufferPool<ArrayVector<T>> ArrayVector<T>::pool;

#endif //SANDDB_ARRAY_VECTOR_H
