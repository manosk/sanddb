//
// Created by manolee on 11/28/17.
//

#include "string_column.h"

StringColumn::StringColumn(VectorType vt) :
        ColumnBase(Type::STRING, vt)
{}

StringColumn::StringColumn() :
        ColumnBase(Type::STRING,VectorType::ARRAY_CPP)
{}

void StringColumn::addVector(std::unique_ptr<DataVector<string>> vec) {
    vectors.emplace_back(std::move(vec));
}

std::unique_ptr<DataVector<bool>>& StringColumn::getVectorBoolean(int idx) {
    (void)idx;
    throw std::logic_error("Invalid casting call");
}

std::unique_ptr<DataVector<int>>& StringColumn::getVectorInt(int idx) {
    (void)idx;
    throw std::logic_error("Invalid casting call");
}

std::unique_ptr<DataVector<float>>& StringColumn::getVectorFloat(int idx) {
    (void)idx;
    throw std::logic_error("Invalid casting call");
}

std::unique_ptr<DataVector<double>>& StringColumn::getVectorDouble(int idx) {
    (void)idx;
    throw std::logic_error("Invalid casting call");
}

std::unique_ptr<DataVector<string>>& StringColumn::getVectorString(int idx) {
    return vectors.at(idx);
}

void StringColumn::setBoolean(size_t pos, bool val) {
    (void)pos;
    (void)val;
    throw std::logic_error("Invalid setter");
}

void StringColumn::setInt(size_t pos, int val) {
    (void)pos;
    (void)val;
    throw std::logic_error("Invalid setter");
}

void StringColumn::setFloat(size_t pos, float val) {
    (void)pos;
    (void)val;
    throw std::logic_error("Invalid setter");
}

void StringColumn::setDouble(size_t pos, double val) {
    (void)pos;
    (void)val;
    throw std::logic_error("Invalid setter");
}

void StringColumn::setString(size_t pos, const string& val) {
    if(vectors.empty() || (pos + 1) % VECTORSIZE == 0) {
        if(this->getVectorType() == VectorType::ARRAY_CPP)  {
            vectors.emplace_back(std::make_unique<ArrayVector<string>>());
            vectors.at(vectors.size() - 1)->setInit();
        } else  {
            throw std::logic_error("Arrow functionality still missing");
        }
    }
    vectors.at(pos / VECTORSIZE)->set(pos % VECTORSIZE, val);
}