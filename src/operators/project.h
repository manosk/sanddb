//
// Created by manolee on 12/18/17.
//

#ifndef SANDDB_PROJECT_H
#define SANDDB_PROJECT_H

#include <expressions/expressions.h>
#include "common/common.h"
#include "sql_operator.h"

class Project : public SqlOperator {
public:
    Project(unique_ptr<SqlOperator> child, const Relation* const rel, vector<std::unique_ptr<Expression>> exprs);
    Project(unique_ptr<SqlOperator> child, const Relation* const rel, const vector<std::unique_ptr<Expression>*>& exprs);

    const Relation* getRel() const;
    const SqlOperator* getChild();
    string print() const override;
private:
    std::unique_ptr<SqlOperator> child;
    //reminder: projections produce new bindings
    const Relation* const rel;
    vector<std::unique_ptr<Expression>> projections;
};


#endif //SANDDB_PROJECT_H
