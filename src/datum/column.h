//
// Created by manolee on 11/22/17.
//

#ifndef SANDDB_COLUMN_H
#define SANDDB_COLUMN_H

#include <schema/types.h>
#include "common/common.h"
#include "data_vector.h"
#include "array_vector.h"

class ColumnBase    {
private:
    Type dt;
    VectorType  vt;
public:
    ColumnBase(Type dt, VectorType vt) :
            dt(dt), vt(vt)
    {}

    Type getType() const {
        return dt;
    }

    VectorType getVectorType() const {
        return vt;
    }

    virtual std::unique_ptr<DataVector<bool>>&      getVectorBoolean(int idx) = 0;
    virtual std::unique_ptr<DataVector<int>>&       getVectorInt(int idx) = 0;
    virtual std::unique_ptr<DataVector<float>>&     getVectorFloat(int idx) = 0;
    virtual std::unique_ptr<DataVector<double>>&    getVectorDouble(int idx) = 0;
    virtual std::unique_ptr<DataVector<string>>&    getVectorString(int idx) = 0;

    /**
     * Member function templates cannot be declared virtual.
     * This constraint is imposed because
     * the usual implementation of the virtual function call mechanism uses a fixed-size table
     * with one entry per virtual function.
     * However, the number of instantiations of a member function template is not fixed
     * until the entire program has been translated.
     * Hence, supporting virtual member function templates would require
     * support for a whole new kind of mechanism in C++ compilers and linkers.
     * In contrast, the ordinary members of class templates can be virtual
     * because their number is fixed when a class is instantiated.
     **/
    //virtual template <typename T>
    //void setValue(size_t pos, T val) = 0;
    virtual void setBoolean(size_t pos, bool val) = 0;
    virtual void setInt(size_t pos, int val) = 0;
    virtual void setFloat(size_t pos, float val) = 0;
    virtual void setDouble(size_t pos, double val) = 0;
    virtual void setString(size_t pos, const string& val) = 0;
};
#endif //SANDDB_COLUMN_H
