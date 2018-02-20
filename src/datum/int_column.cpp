//
// Created by manolee on 11/28/17.
//

#include "int_column.h"

IntColumn::IntColumn(VectorType vt) :
        ColumnBase(Type::INT, vt)
{}

IntColumn::IntColumn() :
        ColumnBase(Type::INT, VectorType::ARRAY_CPP)
{}

void IntColumn::addVector(std::unique_ptr<DataVector<int>> vec) {
    vectors.emplace_back(std::move(vec));
}

std::unique_ptr<DataVector<bool>>& IntColumn::getVectorBoolean(int idx) {
    (void)idx;
    throw std::logic_error("Invalid casting call");
}

std::unique_ptr<DataVector<int>>& IntColumn::getVectorInt(int idx) {
    return vectors.at(idx);
}

std::unique_ptr<DataVector<float>>& IntColumn::getVectorFloat(int idx) {
    (void)idx;
    throw std::logic_error("Invalid casting call");
}

std::unique_ptr<DataVector<double>>& IntColumn::getVectorDouble(int idx) {
    (void)idx;
    throw std::logic_error("Invalid casting call");
}

std::unique_ptr<DataVector<string>>& IntColumn::getVectorString(int idx) {
    (void)idx;
    throw std::logic_error("Invalid casting call");
}

void IntColumn::setBoolean(size_t pos, bool val) {
    (void)pos;
    (void)val;
    throw std::logic_error("Invalid setter");
}

void IntColumn::setInt(size_t pos, int val) {
    if(pos % VECTORSIZE == 0) {
        //either no vectors exist, or current vector is full
        if(this->getVectorType() == VectorType::ARRAY_CPP)  {
            vectors.emplace_back(std::make_unique<ArrayVector<int>>());
            vectors.at(vectors.size() - 1)->setInit();
        } else  {
            throw std::logic_error("Arrow functionality still missing");
        }
    }
    vectors.at(pos / VECTORSIZE)->set(pos % VECTORSIZE, val);
}

void IntColumn::setFloat(size_t pos, float val) {
    (void)pos;
    (void)val;
    throw std::logic_error("Invalid setter");
}

void IntColumn::setDouble(size_t pos, double val) {
    (void)pos;
    (void)val;
    throw std::logic_error("Invalid setter");
}

void IntColumn::setString(size_t pos, const string& val) {
    (void)pos;
    (void)val;
    throw std::logic_error("Invalid setter");
}