//
// Created by manolee on 11/9/17.
//

#ifndef SANDDB_DATA_VECTOR_H
#define SANDDB_DATA_VECTOR_H

#include "common/common.h"

/**
 * @tparam T the type of the vector
 * Common API to be used, regardless of whether we settle for std::array-based vectors or arrow-based ones
 *
 */
//TODO inject selVector here?
// NO: It's not every column that needs one

//TODO consider how necessary OID is.
//  Is it just the joins that need to deal with it explicitly?
//  If yes: maybe the output can deal with generating the new one

constexpr int VECTORSIZE = 10;//1024;

enum class VectorType {
    ARRAY_CPP,ARROW
};

template<typename T>
class DataVector {

public:
    virtual size_t size() = 0;
    virtual const T& get(size_t pos) const = 0;
    virtual void set(size_t pos, const T &elem) = 0;
    virtual bool isInit()   = 0;
    virtual void setInit()  = 0;
    virtual VectorType getVectorType() = 0;
};


#endif //SANDDB_DATA_VECTOR_H
