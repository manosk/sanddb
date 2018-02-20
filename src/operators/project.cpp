//
// Created by manolee on 12/18/17.
//

#include <schema/types.h>
#include "project.h"

Project::Project(unique_ptr<SqlOperator> child, const Relation* const rel, vector<std::unique_ptr<Expression>> exprs) :
        child(std::move(child)), rel(rel), projections(std::move(exprs)) {}

Project::Project(unique_ptr<SqlOperator> child, const Relation* const rel, const vector<std::unique_ptr<Expression>*>& exprs) :
        child(std::move(child)),
        rel(rel)
{
    for(auto& expr: exprs)  {
        projections.push_back((*expr)->clone());
    }
}

const Relation* Project::getRel() const {
    return rel;
}

const SqlOperator* Project::getChild() {
    return child.get();
}

string Project::print()    const  {
    std::stringstream ss;
    ss  <<  "Project(";
    ss  <<  "outputRelation: [ " << (*rel).print() << " ], ";
    //ss  <<  "\n\t";

    ss  <<  "projections: [ ";
    int count = projections.size();
    for(auto& proj : projections)   {
        ss << proj->print();
        if (--count != 0)   {
            ss << ", ";
        }
    }
    ss << " ]";
    ss << ")";
    ss  <<  "\n|";
    ss  <<  "\n|\n";
    ss  << child->print();
    return ss.str();
}