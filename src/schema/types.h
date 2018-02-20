//
// Created by manolee on 11/24/17.
//

#ifndef SANDDB_TYPES_H
#define SANDDB_TYPES_H

#include "common/common.h"

enum class Type {
    BOOL = 0, INT, FLOAT, DOUBLE, STRING
};

/**
 * TODO Move definitions to .cpp
 *
 * TODO define a parsing exception
 */
class Attribute    {
public:
    Attribute(string name, Type type)
            :   name{std::move(name)},  type(type)
    {}

//    Attribute(string name, string type_raw)
//            :   name{std::move(name)},  type(parseType(std::move(type_raw)))
//    {
//        std::cout<<"The moving constructor"<<std::endl;
//    }

    Attribute(const string& name, const string& type_raw)
            :   name{name},  type(parseType(type_raw))
    {}

    const string &getName() const {
        return name;
    }

    Type getType() const {
        return type;
    }

    //Copy
    Attribute(const Attribute& obj) noexcept {
        this->name = obj.name;
        this->type = obj.type;
    }

    Attribute& operator=(const Attribute& rhs) noexcept {
        if(this != &rhs) {
            this->name = rhs.name;
            this->type = rhs.type;
        }
        return *this;
    }

    //Move
    Attribute(Attribute&& obj) noexcept {
        this->name = std::move(obj.name);
        this->type = obj.type;
    }

    Attribute& operator=(Attribute&& rhs) noexcept {
        if(this != &rhs) {
            this->name = std::move(rhs.name);
            this->type = rhs.type;
        }
        return *this;
    }

    static Type parseType(const string& type_raw)    {
        if(caseInsCompare(type_raw,"bool"))   {
            return Type::BOOL;
        }
        else if(caseInsCompare(type_raw,"int") || caseInsCompare(type_raw,"integer"))   {
            return Type::INT;
        }
        else if(caseInsCompare(type_raw,"float"))   {
            return Type::FLOAT;
        }
        else if(caseInsCompare(type_raw,"double"))   {
            return Type::DOUBLE;
        }
        else if(caseInsCompare(type_raw,"string") || caseInsCompare(type_raw,"varchar")) {
            return Type::STRING;
        }
        else    {
            throw std::logic_error("Unknown datatype");
        }
    }

    string print()  const  {
        return name + ": " + flushType();
    }

private:
    string name;
    Type type;

    string flushType()  const  {
        string output;
        switch(type) {
            case Type::BOOL:
                output = "bool";
                break;
            case Type::INT:
                output = "int";
                break;
            case Type::FLOAT:
                output = "float";
                break;
            case Type::DOUBLE:
                output = "double";
                break;
            case Type::STRING:
                output = "string";
                break;
        }
        return output;
    }
};

class Relation    {
public:

    Relation()
            : name("unknown")
    {}

    Relation(string name, vector<Attribute> atts)
            : name{std::move(name)}, atts{std::move(atts)}
    {}

    const string &getName() const {
        return name;
    }

    const vector<Attribute> &getAtts() const {
        return atts;
    }

    string print()  const  {
        std::stringstream ss;
        ss << name << "[ ";
        int count = atts.size();
        for(auto& att : atts)   {
            ss << att.print();
            if (--count != 0)   {
                ss << ", ";
            }
        }
        ss << " ]";
        return ss.str();
    }

    ~Relation() {
        std::cout << "Some relation object went out of scope" << std::endl;
    }

private:
    string name;
    vector<Attribute> atts;
};

#endif //SANDDB_TYPES_H
