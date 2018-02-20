//
// Created by manolee on 12/18/17.
//

#ifndef SANDDB_SELECT_H
#define SANDDB_SELECT_H


#include <expressions/expressions.h>
#include "common/common.h"
#include "sql_operator.h"

class Select : public SqlOperator  {
public:
    Select(unique_ptr<SqlOperator> child, unique_ptr<Expression> cond);

    const SqlOperator* getChild();

    const unique_ptr<Expression> &getCond() const;

    string print() const override;

private:
    std::unique_ptr<SqlOperator> child;
    std::unique_ptr<Expression> cond;
};

#endif //SANDDB_SELECT_H
