//
// Created by manolee on 11/28/17.
//

#include "float_column.h"

FloatColumn::FloatColumn(VectorType vt) :
        ColumnBase(Type::FLOAT, vt)
{}

FloatColumn::FloatColumn() :
        ColumnBase(Type::FLOAT, VectorType::ARRAY_CPP)
{}

void FloatColumn::addVector(std::unique_ptr<DataVector<float>> vec) {
    vectors.emplace_back(std::move(vec));
}

std::unique_ptr<DataVector<bool>>& FloatColumn::getVectorBoolean(int idx) {
    (void)idx;
    throw std::logic_error("Invalid casting call");
}

std::unique_ptr<DataVector<int>>& FloatColumn::getVectorInt(int idx) {
    (void)idx;
    throw std::logic_error("Invalid casting call");
}

std::unique_ptr<DataVector<float>>& FloatColumn::getVectorFloat(int idx) {
    return vectors.at(idx);
}

std::unique_ptr<DataVector<double>>& FloatColumn::getVectorDouble(int idx) {
    (void)idx;
    throw std::logic_error("Invalid casting call");
}

std::unique_ptr<DataVector<string>>& FloatColumn::getVectorString(int idx) {
    (void)idx;
    throw std::logic_error("Invalid casting call");
}

void FloatColumn::setBoolean(size_t pos, bool val) {
    (void)pos;
    (void)val;
    throw std::logic_error("Invalid setter");
}

void FloatColumn::setInt(size_t pos, int val) {
    (void)pos;
    (void)val;
    throw std::logic_error("Invalid setter");
}

void FloatColumn::setFloat(size_t pos, float val) {
    if(vectors.empty() || (pos + 1) % VECTORSIZE == 0) {
        if(this->getVectorType() == VectorType::ARRAY_CPP)  {
            vectors.emplace_back(std::make_unique<ArrayVector<float>>());
            vectors.at(vectors.size() - 1)->setInit();
        } else  {
            throw std::logic_error("Arrow functionality still missing");
        }
    }
    vectors.at(pos / VECTORSIZE)->set(pos % VECTORSIZE, val);
}

void FloatColumn::setDouble(size_t pos, double val) {
    (void)pos;
    (void)val;
    throw std::logic_error("Invalid setter");
}

void FloatColumn::setString(size_t pos, const string& val) {
    (void)pos;
    (void)val;
    throw std::logic_error("Invalid setter");
}