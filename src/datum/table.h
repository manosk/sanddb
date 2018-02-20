//
// Created by manolee on 11/22/17.
//

#ifndef SANDDB_TABLE_H
#define SANDDB_TABLE_H

#include <schema/types.h>
#include "common/common.h"
#include "column.h"
#include "boolean_column.h"
#include "double_column.h"
#include "float_column.h"
#include "int_column.h"
#include "string_column.h"

//T meant to be sth such as Column<int>
//template <typename T>
//T* do_access(void* p)
//{
//    return static_cast<T*>(p);
//}

class Table {
    //using std::experimental::fundamentals_v1::any;
public:
    Table(std::string name) :
            name(name)
    {}

    Table(std::string name, const vector<Attribute>& atts, const vector<VectorType>& vt)  :
            name(name)
    {
        if(atts.size() != vt.size())    {
            throw std::logic_error("Incompatibility in column definitions");
        }
        auto cnt = 0;
        for(auto& attr: atts)    {
            switch(attr.getType())  {
                case Type::BOOL:
                {
                    this->addColumn<bool>(std::make_unique<BooleanColumn>(vt.at(cnt++)));
                    break;
                }
                case Type::DOUBLE:  {
                    this->addColumn<double>(std::make_unique<DoubleColumn>(vt.at(cnt++)));
                    break;
                }
                case Type::FLOAT:   {
                    this->addColumn<float>(std::make_unique<FloatColumn>(vt.at(cnt++)));
                    break;
                }
                case Type::INT: {
                    this->addColumn<int>(std::make_unique<IntColumn>(vt.at(cnt++)));
                    break;
                }
                case Type::STRING:  {
                    this->addColumn<string>(std::make_unique<StringColumn>(vt.at(cnt++)));
                    break;
                }
            }
        }
    }

    Table(std::string name, const vector<Attribute>& atts)  :
            name(name)
    {
        for(auto& attr: atts)    {
            switch(attr.getType())  {
                case Type::BOOL:
                {
                    this->addColumn<bool>(std::make_unique<BooleanColumn>());
                    break;
                }
                case Type::DOUBLE:  {
                    this->addColumn<double>(std::make_unique<DoubleColumn>());
                    break;
                }
                case Type::FLOAT:   {
                    this->addColumn<float>(std::make_unique<FloatColumn>());
                    break;
                }
                case Type::INT: {
                    this->addColumn<int>(std::make_unique<IntColumn>());
                    break;
                }
                case Type::STRING:  {
                    this->addColumn<string>(std::make_unique<StringColumn>());
                    break;
                }
            }
        }
    }

    template <typename T> void addColumn(std::unique_ptr<ColumnBase> col)    {
       columns.push_back(std::move(col));
    }

    std::unique_ptr<ColumnBase>& getColumn(unsigned idx) {
        if(idx >= columns.size())    {
            throw std::logic_error("Wrong column addressing");
        }
        return columns[idx];
    }

    void setColumn(unsigned idx, unsigned colNo, void *val)   {
        switch(columns.at(colNo)->getType())    {
            case Type::BOOL:
            {
                columns.at(colNo)->setBoolean(idx,*(static_cast<bool*>(val)));
                break;
            }
            case Type::DOUBLE:  {
                columns.at(colNo)->setDouble(idx,*(static_cast<double*>(val)));
                break;
            }
            case Type::FLOAT:   {
                columns.at(colNo)->setFloat(idx,*(static_cast<float*>(val)));
                break;
            }
            case Type::INT: {
                columns.at(colNo)->setInt(idx,*(static_cast<int*>(val)));
                break;
            }
            case Type::STRING:  {
                columns.at(colNo)->setString(idx,*(static_cast<string*>(val)));
                break;
            }
        }
    }

private:

    std::string name;
    std::vector<std::unique_ptr<ColumnBase>> columns;
};
#endif //SANDDB_TABLE_H
