//
// Created by manolee on 11/28/17.
//

#include "boolean_column.h"

BooleanColumn::BooleanColumn(VectorType vt) :
        ColumnBase(Type::BOOL, vt)
{}

BooleanColumn::BooleanColumn() :
        ColumnBase(Type::BOOL, VectorType::ARRAY_CPP)
{}

void BooleanColumn::addVector(std::unique_ptr<DataVector<bool>> vec) {
    vectors.emplace_back(std::move(vec));
}

std::unique_ptr<DataVector<bool>>& BooleanColumn::getVectorBoolean(int idx) {
    return vectors.at(idx);
}

std::unique_ptr<DataVector<int>>& BooleanColumn::getVectorInt(int idx) {
    (void)idx;
    throw std::logic_error("Invalid casting call");
}

std::unique_ptr<DataVector<float>>& BooleanColumn::getVectorFloat(int idx) {
    (void)idx;
    throw std::logic_error("Invalid casting call");
}

std::unique_ptr<DataVector<double>>& BooleanColumn::getVectorDouble(int idx) {
    (void)idx;
    throw std::logic_error("Invalid casting call");
}

std::unique_ptr<DataVector<string>>& BooleanColumn::getVectorString(int idx) {
    (void)idx;
    throw std::logic_error("Invalid casting call");
}

void BooleanColumn::setBoolean(size_t pos, bool val) {
    //static_assert(std::is_same<decltype(val), bool>::value, "val must be bool");
    if(vectors.empty() || (pos + 1) % VECTORSIZE == 0) {
        if(this->getVectorType() == VectorType::ARRAY_CPP)  {
            vectors.emplace_back(std::make_unique<ArrayVector<bool>>());
            vectors.at(vectors.size() - 1)->setInit();
        } else  {
            throw std::logic_error("Arrow functionality still missing");
        }
    }
    vectors.at(pos / VECTORSIZE)->set(pos % VECTORSIZE, val);
}

void BooleanColumn::setInt(size_t pos, int val) {
    (void)pos;
    (void)val;
    throw std::logic_error("Invalid setter");
}

void BooleanColumn::setFloat(size_t pos, float val) {
    (void)pos;
    (void)val;
    throw std::logic_error("Invalid setter");
}

void BooleanColumn::setDouble(size_t pos, double val) {
    (void)pos;
    (void)val;
    throw std::logic_error("Invalid setter");
}

void BooleanColumn::setString(size_t pos, const string& val) {
    (void)pos;
    (void)val;
    throw std::logic_error("Invalid setter");
}