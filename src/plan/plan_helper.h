//
// Created by manolee on 12/18/17.
//

#ifndef SANDDB_PLAN_HELPER_H
#define SANDDB_PLAN_HELPER_H

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <schema/types.h>
#include <expressions/expressions.h>
#include "util/catalog.h"
#include "operators/sql_operator.h"
#include "common/common.h"

namespace pt = boost::property_tree;

class PlanHelper {
public:
    PlanHelper(Catalog& catalog);

    std::unique_ptr<SqlOperator> parsePlan(const string &path);
private:
    pt::ptree root;
    Catalog* const catalog;

    //Use it to create 'unique' operator labels.
    //Increment after each use
    int opID = 1;
    int attrID = 1;

    std::unique_ptr<SqlOperator> parseOperator(const pt::ptree &tree);
    Relation* parseRelation(const string& relName, const pt::ptree &attrList);

    using op_ptr = std::unique_ptr<std::unique_ptr<SqlOperator>>;
    std::unique_ptr<Expression> parseExpression(const pt::ptree &rawExpr, const op_ptr &currentRoot);

//    std::unique_ptr<Constant> parseConstant(const pt::ptree &rawConstant);
//    std::unique_ptr<Argument> parseArgument(const pt::ptree &rawArgument);
//    std::unique_ptr<BinaryExpression> parseBinaryExpression(const pt::ptree &rawBinExpr);
//    std::unique_ptr<BinaryBooleanExpression> parseBinaryBooleanExpression(const pt::ptree &rawBinExpr);
//    std::unique_ptr<AggregateExpression> parseAggregateExpression(const pt::ptree &rawAgg);
    //Attribute parseAttribute(const pt::ptree &tree);0

};


#endif //SANDDB_PLAN_HELPER_H