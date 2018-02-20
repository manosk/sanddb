//
// Created by manolee on 12/18/17.
//

#include "scan.h"

Scan::Scan(const string &relName, const Relation* const rel, const string &path) :
        relName(relName), rel(rel), path(path) {}

string Scan::print()    const  {
    std::stringstream ss;
    ss  << "Scan(";
    ss << (*rel).print();
    ss << ")\n";
    return ss.str();
}