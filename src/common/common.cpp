//
// Created by manolee on 12/18/17.
//
#include "common/common.h"


/**
* Check if a file exists
* @return true if and only if the file exists, false else
*/
bool fileExists(const std::string& file) {
    struct stat buf;
    return (stat(file.c_str(), &buf) == 0);
}

bool caseInsCompare(const string& s1, const string& s2) {
    return((s1.size() == s2.size()) &&
           equal(std::begin(s1), std::end(s1), std::begin(s2), caseInsCharCompareN));
}