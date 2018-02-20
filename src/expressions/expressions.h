//
// Created by manolee on 12/18/17.
//

#ifndef SANDDB_EXPRESSIONS_H
#define SANDDB_EXPRESSIONS_H
#include "common/common.h"
#include "schema/types.h"

class Expression    {
public:
    /**
     * @return whether the expression creates new values,
     * or it just provide access to existing ones (args / constants)
     */
    virtual bool producesValues() = 0;
    virtual Type getType() const = 0;
    virtual string print() const = 0;
    virtual std::unique_ptr<Expression> clone() = 0;
};

enum class BinOpType    {
    GT = 0, GE, LT, LE, EQ, NEQ, AND, OR, MULT, DIV, ADD, SUB
};
class BinaryExpression  :   public Expression    {
private:
    bool returnsBoolean;
    BinOpType type;
    std::unique_ptr<Expression> lhs;
    std::unique_ptr<Expression> rhs;
public:
    bool producesValues() override { return true; }

    bool hasBooleanResult() const { return returnsBoolean; }
    BinOpType getOperatorType() const { return type; }
    Type getType() const override {
        if(this->hasBooleanResult())
            return Type::BOOL;
        else
            //TODO(manos) this is not always right.
            //For example 1 * 3.14 should be classified as a float
            //Correct type deduction is not of vital priority.
            return lhs->getType();
    }

    BinaryExpression(const string& opName,std::unique_ptr<Expression> lhs,std::unique_ptr<Expression> rhs);
    BinaryExpression(BinOpType type,std::unique_ptr<Expression> lhs,std::unique_ptr<Expression> rhs);

    string print() const override;
    std::unique_ptr<Expression> clone() override;
};

class AggregateExpression   :   public Expression  {
    bool producesValues() override { return true; }
    Type getType() const override {
        //TODO(manos)
        return Type::DOUBLE;
    }
    //TODO(manos)
    string print() const override { return "todo"; }
    //TODO(manos)
    std::unique_ptr<Expression> clone() override { return nullptr; }
};

class Argument  :   public Expression  {
private:
    string relationName;
    Attribute attr;
public:
    Argument(const string &relationName, const Attribute &attr) : relationName(relationName), attr(attr) {}
    //might as well create the attribute myself
    Argument(const string &relationName, const string &attrName, Type dt) :
            relationName(relationName), attr(Attribute(attrName,dt)) {}

    const string &getRelationName() const {
        return relationName;
    }

    const Attribute &getAttr() const {
        return attr;
    }

    bool producesValues() override { return false; }
    Type getType() const override {
        return attr.getType();
    }

    string print() const override { return string(relationName + "." + attr.getName()); }
    std::unique_ptr<Expression> clone() override { return std::make_unique<Argument>(relationName,attr); }
};

class Constant  :   public Expression  {
private:
    Type dataType;
public:
    explicit Constant(Type dataType) : dataType(dataType) {}
    bool producesValues() override { return false; }
};

class BoolConstant   :   public Constant {
private:
    bool value;
public:
    explicit BoolConstant(bool value) : Constant(Type::BOOL), value(value) {}
    explicit BoolConstant(const string& val) : Constant(Type::BOOL)    {
        if(!caseInsCompare(val,"true"))   {
            value = true;
        }   else if(!caseInsCompare(val,"true"))  {
            value = false;
        } else  {
            throw std::logic_error("invalid value for a boolean");
        }
    }
    Type getType() const override {
        return Type::BOOL;
    }

    bool getValue() const {
        return value;
    }

    string print() const override { return std::to_string(value); }
    std::unique_ptr<Expression> clone() override { return std::make_unique<BoolConstant>(value); }
};

class IntConstant   :   public Constant {
private:
    int value;
public:
    explicit IntConstant(int value) : Constant(Type::INT), value(value) {}
    explicit IntConstant(const string& val) : Constant(Type::INT) {
        value = std::stoi(val,nullptr);
    }
    Type getType() const override {
        return Type::INT;
    }

    int getValue() const {
        return value;
    }

    string print() const override { return std::to_string(value); }
    std::unique_ptr<Expression> clone() override { return std::make_unique<IntConstant>(value); }
};

class FloatConstant   :   public Constant {
private:
    float value;
public:
    explicit FloatConstant(float value) : Constant(Type::FLOAT), value(value) {}
    explicit FloatConstant(const string& val) : Constant(Type::FLOAT) {
        value = std::stof(val,nullptr);
    }
    Type getType() const override {
        return Type::FLOAT;
    }

    float getValue() const {
        return value;
    }

    string print() const override { return std::to_string(value); }
    std::unique_ptr<Expression> clone() override { return std::make_unique<FloatConstant>(value); }
};

class DoubleConstant   :   public Constant {
private:
    double value;
public:
    explicit DoubleConstant(double value) : Constant(Type::DOUBLE), value(value) {}
    explicit DoubleConstant(const string& val) : Constant(Type::DOUBLE) {
        value = std::stod(val,nullptr);
    }
    Type getType() const override {
        return Type::DOUBLE;
    }

    double getValue() const {
        return value;
    }

    string print() const override { return std::to_string(value); }
    std::unique_ptr<Expression> clone() override { return std::make_unique<DoubleConstant>(value); }
};

class StringConstant   :   public Constant {
private:
    string value;
public:
    explicit StringConstant(const string& value) : Constant(Type::STRING), value(value) {}
    Type getType() const override {
        return Type::STRING;
    }

    const string &getValue() const {
        return value;
    }

    string print() const override { return value; }
    std::unique_ptr<Expression> clone() override { return std::make_unique<StringConstant>(value); }
};

#endif //SANDDB_EXPRESSIONS_H
