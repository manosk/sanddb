//
// Created by manolee on 12/18/17.
//

#include "select.h"

/**
 * NOTE: Caller of constructors must ALSO use std::move!
 * @param child
 */
Select::Select(unique_ptr<SqlOperator> child, unique_ptr<Expression> cond) :
        child(std::move(child)),
        cond(std::move(cond)) {}

const SqlOperator* Select::getChild() {
    return child.get();
}

const unique_ptr<Expression> &Select::getCond() const {
    return cond;
}

string Select::print()    const  {
    std::stringstream ss;
    ss  <<  "Select(";
    ss  <<  cond->print();
    ss  <<  ")";
    ss  <<  "\n|";
    ss  <<  "\n|\n";
    ss  << child->print();
    return ss.str();
}