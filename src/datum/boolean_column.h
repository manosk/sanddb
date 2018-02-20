//
// Created by manolee on 11/28/17.
//

#ifndef SANDDB_BOOL_COLUMN_H
#define SANDDB_BOOL_COLUMN_H

#include "column.h"

class BooleanColumn :   public  ColumnBase    {
private:
    vector<std::unique_ptr<DataVector<bool>>> vectors;
public:
    BooleanColumn(VectorType vt);

    BooleanColumn();

    void addVector(std::unique_ptr<DataVector<bool>> vec);

    virtual std::unique_ptr<DataVector<bool>>& getVectorBoolean(int idx) override;

    virtual std::unique_ptr<DataVector<int>>& getVectorInt(int idx);

    virtual std::unique_ptr<DataVector<float>>& getVectorFloat(int idx) override;

    virtual std::unique_ptr<DataVector<double>>& getVectorDouble(int idx) override;

    virtual std::unique_ptr<DataVector<string>>& getVectorString(int idx) override;

    virtual void setBoolean(size_t pos, bool val) override;

    virtual void setInt(size_t pos, int val) override;

    virtual void setFloat(size_t pos, float val) override;

    virtual void setDouble(size_t pos, double val) override;

    virtual void setString(size_t pos, const string& val) override;

//    template <typename T>
//    void setValue(size_t pos, T val)    {
//        static_assert(std::is_same<decltype(val), bool>::value, "val must be bool");
//        if(vectors.empty() || (pos + 1) % VECTORSIZE == 0) {
//            if(this->getVectorType() == VectorType::ARRAY_CPP)  {
//                vectors.emplace_back(std::make_unique<ArrayVector<bool>>());
//                vectors.at(vectors.size() - 1)->setInit();
//            } else  {
//                throw std::logic_error("Arrow functionality still missing");
//            }
//        }
//        vectors.at(pos / VECTORSIZE)->set(pos % VECTORSIZE, val);
//    }
};


#endif //SANDDB_BOOL_COLUMN_H
