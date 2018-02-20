//
// Created by manolee on 1/5/18.
//

#include "expressions/expressions.h"

BinaryExpression::BinaryExpression(const string& opName,std::unique_ptr<Expression> lhs,std::unique_ptr<Expression> rhs)
        :    lhs(std::move(lhs)),
             rhs(std::move(rhs))
{
    if(caseInsCompare(opName, "gt"))            {
        type = BinOpType::GT;
        returnsBoolean = true;
    }   else if(caseInsCompare(opName, "ge"))   {
        type = BinOpType::GE;
        returnsBoolean = true;
    }   else if(caseInsCompare(opName, "lt"))   {
        type = BinOpType::LT;
        returnsBoolean = true;
    }   else if(caseInsCompare(opName, "le"))   {
        type = BinOpType::LE;
        returnsBoolean = true;
    }   else if(caseInsCompare(opName, "eq"))   {
        type = BinOpType::EQ;
        returnsBoolean = true;
    }   else if(caseInsCompare(opName, "neq"))  {
        type = BinOpType::NEQ;
        returnsBoolean = true;
    }   else if(caseInsCompare(opName, "and"))  {
        type = BinOpType::AND;
        returnsBoolean = true;
    }   else if(caseInsCompare(opName, "or"))   {
        type = BinOpType::OR;
        returnsBoolean = true;
    }   else if(caseInsCompare(opName, "mult")) {
        type = BinOpType::MULT;
        returnsBoolean = false;
    }   else if(caseInsCompare(opName, "div"))  {
        type = BinOpType::DIV;
        returnsBoolean = false;
    }   else if(caseInsCompare(opName, "add"))  {
        type = BinOpType::ADD;
        returnsBoolean = false;
    }   else if(caseInsCompare(opName, "sub"))  {
        type = BinOpType::SUB;
        returnsBoolean = false;
    }   else                                    {
        throw std::logic_error("Unknown binary operator");
    }
}

BinaryExpression::BinaryExpression(BinOpType type,std::unique_ptr<Expression> lhs,std::unique_ptr<Expression> rhs)  :
        type(type),
        lhs(std::move(lhs)),
        rhs(std::move(rhs))
{}

string BinaryExpression::print() const {
    string op;
    switch (type) {
    case BinOpType::ADD:
            op = " + ";
            break;
    case BinOpType::AND:
            op = " & ";
            break;
    case BinOpType::DIV:
            op = " / ";
            break;
    case BinOpType::EQ:
            op = " = ";
            break;
    case BinOpType::GE:
            op = " >= ";
            break;
    case BinOpType::GT:
            op = " > ";
            break;
    case BinOpType::LE:
            op = " <= ";
            break;
    case BinOpType::LT:
            op = " < ";
            break;
    case BinOpType::MULT:
            op = " * ";
            break;
    case BinOpType::NEQ:
            op = " != ";
            break;
    case BinOpType::OR:
            op = " | ";
            break;
    case BinOpType::SUB:
            op = " - ";
            break;
    }
    return lhs->print() + op + rhs->print();
}

std::ostream& operator<<(std::ostream &strm, const Expression &e)   {
    return strm << e.print();
}

std::unique_ptr<Expression> BinaryExpression::clone() {
    auto leftCopy  = this->lhs->clone();
    auto rightCopy = this->rhs->clone();
    return std::make_unique<BinaryExpression>(this->type,std::move(leftCopy),std::move(rightCopy));
}

//std::ostream& operator<<(std::ostream &strm, const BoolConstant &c) {
//    return strm << c.getValue();
//}
//
//std::ostream& operator<<(std::ostream &strm, const IntConstant &c) {
//    return strm << c.getValue();
//}
//
//std::ostream& operator<<(std::ostream &strm, const FloatConstant &c) {
//    return strm << c.getValue();
//}
//
//std::ostream& operator<<(std::ostream &strm, const DoubleConstant &c) {
//    return strm << c.getValue();
//}
//
//std::ostream& operator<<(std::ostream &strm, const StringConstant &c) {
//    return strm << c.getValue();
//}
//
//std::ostream& operator<<(std::ostream &strm, const Argument &c) {
//    return strm << c.relationName << "." << c.getAttr().getName();
//}
//
//std::ostream& operator<<(std::ostream &strm, const BinaryExpression &c) {
//
//    string op;
//    switch(c.getOperatorType()) {
//        case BinOpType::ADD: op = " + "; break;
//        case BinOpType::AND: op = " & "; break;
//        case BinOpType::DIV: op = " / "; break;
//        case BinOpType::EQ: op = " = "; break;
//        case BinOpType::GE: op = " >= "; break;
//        case BinOpType::GT: op = " > "; break;
//        case BinOpType::LE: op = " <= "; break;
//        case BinOpType::LT: op = " < "; break;
//        case BinOpType::MULT: op = " * "; break;
//        case BinOpType::NEQ: op = " != "; break;
//        case BinOpType::OR: op = " | "; break;
//        case BinOpType::SUB: op = " - "; break;
//    }
//    return strm << c.lhs << op << c.rhs;
//}
