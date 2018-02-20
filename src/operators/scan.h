//
// Created by manolee on 12/18/17.
//

#ifndef SANDDB_SCAN_H
#define SANDDB_SCAN_H


#include <schema/types.h>
#include "operators/sql_operator.h"

class Scan : public SqlOperator {
public:
    Scan(const string &relName, const Relation* const rel, const string &path);
    string print() const override;
private:

    const std::string relName;
    const Relation* const rel;
    const std::string path;
};


#endif //SANDDB_SCAN_H
