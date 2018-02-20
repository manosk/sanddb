//
// Created by manolee on 11/28/17.
//

#include "double_column.h"

DoubleColumn::DoubleColumn(VectorType vt) :
        ColumnBase(Type::DOUBLE, vt)
{}

DoubleColumn::DoubleColumn() :
        ColumnBase(Type::DOUBLE, VectorType::ARRAY_CPP)
{}

void DoubleColumn::addVector(std::unique_ptr<DataVector<double>> vec) {
    vectors.emplace_back(std::move(vec));
}

std::unique_ptr<DataVector<bool>>& DoubleColumn::getVectorBoolean(int idx) {
    (void)idx;
    throw std::logic_error("Invalid casting call");
}

std::unique_ptr<DataVector<int>>& DoubleColumn::getVectorInt(int idx) {
    (void)idx;
    throw std::logic_error("Invalid casting call");
}

std::unique_ptr<DataVector<float>>& DoubleColumn::getVectorFloat(int idx) {
    (void)idx;
    throw std::logic_error("Invalid casting call");
}

std::unique_ptr<DataVector<double>>& DoubleColumn::getVectorDouble(int idx) {
    return vectors.at(idx);
}

std::unique_ptr<DataVector<string>>& DoubleColumn::getVectorString(int idx) {
    (void)idx;
    throw std::logic_error("Invalid casting call");
}

void DoubleColumn::setBoolean(size_t pos, bool val) {
    (void)pos;
    (void)val;
    throw std::logic_error("Invalid setter");
}

void DoubleColumn::setInt(size_t pos, int val) {
    (void)pos;
    (void)val;
    throw std::logic_error("Invalid setter");
}

void DoubleColumn::setFloat(size_t pos, float val) {
    (void)pos;
    (void)val;
    throw std::logic_error("Invalid setter");
}

void DoubleColumn::setDouble(size_t pos, double val) {
    if(vectors.empty() || (pos + 1) % VECTORSIZE == 0) {
        if(this->getVectorType() == VectorType::ARRAY_CPP)  {
            vectors.emplace_back(std::make_unique<ArrayVector<double>>());
            vectors.at(vectors.size() - 1)->setInit();
        } else  {
            throw std::logic_error("Arrow functionality still missing");
        }
    }
    vectors.at(pos / VECTORSIZE)->set(pos % VECTORSIZE, val);
}

void DoubleColumn::setString(size_t pos, const string& val) {
    (void)pos;
    (void)val;
    throw std::logic_error("Invalid setter");
}