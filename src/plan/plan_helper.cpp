//
// Created by manolee on 12/18/17.
//

#include <operators/project.h>
#include <operators/scan.h>
#include <operators/select.h>

#include "plan_helper.h"

PlanHelper::PlanHelper(Catalog& catalog) : catalog(&catalog) {}

std::unique_ptr<SqlOperator> PlanHelper::parsePlan(const string &path)  {
    if(!fileExists(path))   {
        throw std::runtime_error("Could not open file");
    }
    // Load the json file in this ptree
    pt::read_json(path, root);


    //placeholder
    return parseOperator(root);
}

std::unique_ptr<SqlOperator> PlanHelper::parseOperator(const pt::ptree &tree)   {
    std::unique_ptr<SqlOperator> op = nullptr;

    auto operatorLabel = "operator";
    auto singleChildLabel = "child";

    auto scanLabel = "scan";
    auto selectLabel = "select";
    auto projectionLabel = "projection";

    auto opName = tree.get<std::string>(operatorLabel);
    if(opName == scanLabel)    {
        auto relName =  tree.get<std::string>("name");
        auto relation = parseRelation(relName, tree.get_child("tupleType"));
        auto relPath = catalog->getFilePath(relName);

        op = std::make_unique<Scan>(relName,relation,relPath);
    }   else if(opName == selectLabel) {
        auto conditionLabel = "cond";

        //visit child operator
        auto childOp = parseOperator(tree.get_child(singleChildLabel));

        //move ownership of childOp to currRoot
        auto currRoot = std::make_unique<std::unique_ptr<SqlOperator>>(std::move(childOp));
        //visit condition
        auto parsedCond = parseExpression(tree.get_child(conditionLabel),currRoot);

        op = std::make_unique<Select>(std::move(*currRoot), std::move(parsedCond));
    }   else if(opName == projectionLabel) {

        //visit child operator
        auto childOp = parseOperator(tree.get_child(singleChildLabel));

        //(new) tuple type handling
        auto newRelName =  tree.get<std::string>("name");
        auto newRelation = parseRelation(newRelName, tree.get_child("tupleType"));

        //ownership of childOp moved to currRoot
        auto currRoot = std::make_unique<std::unique_ptr<SqlOperator>>(std::move(childOp));

        auto expressions = tree.get_child("e");
        vector<std::unique_ptr<Expression>> projections;
        for(auto& expr: expressions)    {
            auto parsedExpr = parseExpression(expr.second,currRoot);
            projections.push_back(std::move(parsedExpr));
        }

        op = std::make_unique<Project>(std::move(*currRoot),newRelation,std::move(projections));
    }   else  {
        throw std::runtime_error("unimplemented sql operator!");
    }
    return op;
}

Relation* PlanHelper::parseRelation(const string& relName, const pt::ptree &attrList)   {
    if(catalog->getRelations().find(relName) == catalog->getRelations().end()) {
        std::vector<Attribute> relAtts;
        for (auto &attr : attrList) {
            //auto relName = attr.second.get<std::string>("rel");
            auto attrName = attr.second.get<std::string>("attr");
            auto dataType = attr.second.get<std::string>("type");
            relAtts.emplace_back(attrName, Attribute::parseType(dataType));
        }
        auto relation = std::make_unique<Relation>(relName, relAtts);
        catalog->addRelation(relName,std::move(relation));
    }
    return (catalog->getRelations()).at(relName).get();
}

std::unique_ptr<Expression> PlanHelper::parseExpression(const pt::ptree &rawExpr, const op_ptr &currentRoot) {
//std::unique_ptr<Expression> PlanHelper::parseExpression(const pt::ptree &rawExpr)  {
    std::unique_ptr<Expression> parsedExpr = nullptr;

    auto exprLabel = "expression";
    auto typeLabel = "type";

    auto constantLabel = "constant";
    auto argumentLabel = "argument";
    auto aggregateLabel = "aggregate";
    auto binaryOpLabel = "binOp";

    std::cout << rawExpr.data() << std::endl;
    auto exprType = rawExpr.get<std::string>(exprLabel);

    if(exprType == constantLabel)               {
        auto valueLabel = "v";
        //(All constant constructors can process string as an input)
        auto unparsedValue = rawExpr.get<std::string>(valueLabel);

        Type constantType = Attribute::parseType(rawExpr.get<std::string>(typeLabel));
        switch(constantType) {
            case Type::BOOL:
                parsedExpr = std::make_unique<BoolConstant>(BoolConstant(unparsedValue));
                break;
            case Type::INT:
                parsedExpr = std::make_unique<IntConstant>(IntConstant(unparsedValue));
                break;
            case Type::FLOAT:
                parsedExpr = std::make_unique<FloatConstant>(FloatConstant(unparsedValue));
                break;
            case Type::DOUBLE:
                parsedExpr = std::make_unique<DoubleConstant>(DoubleConstant(unparsedValue));
                break;
            case Type::STRING:
                parsedExpr = std::make_unique<StringConstant>(StringConstant(unparsedValue));
                break;
        }
    }   else if (exprType == argumentLabel)     {
        auto relationLabel = "rel";
        auto attrLabel = "attr";

        Type argType = Attribute::parseType(rawExpr.get<std::string>(typeLabel));
        auto relationName = rawExpr.get<std::string>(relationLabel);
        auto attributeName = rawExpr.get<std::string>(attrLabel);
        parsedExpr = std::make_unique<Argument>(Argument(relationName,attributeName,argType));

    }   else if (exprType == aggregateLabel)    {
        //TODO(manos) fill up this case
    }   else if (exprType == binaryOpLabel)     {
        auto operationLabel = "op";

        auto exprLeft = rawExpr.get_child("left");
        auto parsedExprLeft = parseExpression(exprLeft,currentRoot);

        if(parsedExprLeft->producesValues())    {
            //create unique aliases for (1) the new projection operator and (2) the new 'attribute'
            auto opAlias = "expr" + std::to_string((this->opID)++);
            auto attrAlias = "attr" + std::to_string((this->attrID)++);

            vector<Attribute> attsLeft;
            attsLeft.emplace_back(attrAlias,parsedExprLeft->getType());
            auto relationLeft = std::make_unique<Relation>(opAlias,attsLeft);
            catalog->addRelation(opAlias,std::move(relationLeft));

//            vector<std::unique_ptr<Expression>> expsLeft;
//            expsLeft.push_back(std::move(parsedExprLeft));
            vector<std::unique_ptr<Expression>*> expsLeft;
            expsLeft.push_back(&parsedExprLeft);

            std::unique_ptr<SqlOperator> newProjOp =
                    std::make_unique<Project>(std::move(*currentRoot),
                                              catalog->getRelations().at(opAlias).get(),
                                              expsLeft);
            *currentRoot = std::move(newProjOp);
        }

        auto exprRight = rawExpr.get_child("right");
        auto parsedExprRight = parseExpression(exprRight,currentRoot);
        if(parsedExprRight->producesValues())    {
            //create unique aliases for (1) the new projection operator and (2) the new 'attribute'
            auto opAlias = "expr" + std::to_string((this->opID)++);
            auto attrAlias = "attr" + std::to_string((this->attrID)++);

            vector<Attribute> attsRight;
            attsRight.emplace_back(attrAlias,parsedExprRight->getType());
            auto relationRight = std::make_unique<Relation>(opAlias,attsRight);
            catalog->addRelation(opAlias,std::move(relationRight));

//            vector<std::unique_ptr<Expression>> expsRight;
//            expsRight.push_back(std::move(parsedExprRight));
            vector<std::unique_ptr<Expression>*> expsRight;
            expsRight.push_back(&parsedExprRight);

            std::unique_ptr<SqlOperator> newProjOp =
                    std::make_unique<Project>(std::move(*currentRoot),
                                              catalog->getRelations().at(opAlias).get(),
                                              expsRight);
            *currentRoot = std::move(newProjOp);
        }

        auto operation = rawExpr.get<std::string>(operationLabel);
        parsedExpr = std::make_unique<BinaryExpression>(operation,std::move(parsedExprLeft),std::move(parsedExprRight));
    }

    return parsedExpr;
}