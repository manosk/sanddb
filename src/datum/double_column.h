//
// Created by manolee on 11/28/17.
//

#ifndef SANDDB_DOUBLE_COLUMN_H
#define SANDDB_DOUBLE_COLUMN_H

#include "column.h"

class DoubleColumn :   public  ColumnBase    {
private:
    vector<std::unique_ptr<DataVector<double>>> vectors;
public:
    DoubleColumn(VectorType vt);

    DoubleColumn();

    void addVector(std::unique_ptr<DataVector<double>> vec);

    virtual std::unique_ptr<DataVector<bool>>& getVectorBoolean(int idx) override;

    virtual std::unique_ptr<DataVector<int>>& getVectorInt(int idx) override;

    virtual std::unique_ptr<DataVector<float>>& getVectorFloat(int idx) override;

    virtual std::unique_ptr<DataVector<double>>& getVectorDouble(int idx) override;

    virtual std::unique_ptr<DataVector<string>>& getVectorString(int idx) override;

    virtual void setBoolean(size_t pos, bool val) override;

    virtual void setInt(size_t pos, int val) override;

    virtual void setFloat(size_t pos, float val) override;

    virtual void setDouble(size_t pos, double val) override;

    virtual void setString(size_t pos, const string& val) override;
};


#endif //SANDDB_DOUBLE_COLUMN_H
