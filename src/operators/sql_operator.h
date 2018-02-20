//
// Created by manolee on 12/18/17.
//

#ifndef SANDDB_SQLOPERATOR_H
#define SANDDB_SQLOPERATOR_H

#include "common/common.h"

/**
 * TODO Decide how to expose 'volcano' interface
 */
class SqlOperator {
public:
    //using Bindings = std::vector<std::string>;
    std::vector<std::string> bindings;
//    virtual void open() = 0;
//    virtual void next() = 0;
//    virtual void close() = 0;
    virtual string print() const = 0;
//
//    std::ostream& operator <<(std::ostream& ostr, const SqlOperator& op) {
//        ostr << op.print();
//        return ostr;
//    }
};


#endif //SANDDB_SQLOPERATOR_H
