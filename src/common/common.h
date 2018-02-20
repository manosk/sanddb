//
// Created by manolee on 11/2/17.
//

#ifndef SANDDB_COMMON_H
#define SANDDB_COMMON_H


#include <iostream>
#include <cstdlib>
#include <sys/stat.h>
//
#include <bitset>

#include <chrono>
#include <cstddef>
#include <memory>
#include <utility>
//
#include <string>
#include <sstream>
#include <cctype> //used in string comparison (for 'toupper')
#include <algorithm> //for equality
#include <list>
#include <vector>
#include <map>
#include <forward_list>
//'any' doesn't seem to support unique pointers
//#include <experimental/any>
#include <experimental/string_view>
//
#include <atomic>
#include <mutex>
#include <condition_variable>
//
#include <cassert>
#include <stdexcept>
//
//#include <experimental/filesystem>
#include <boost/filesystem.hpp>
//
//Cpp Guidelines Support Library: https://github.com/Microsoft/GSL
//consider using it for primitives such as 'span'
//#include <gsl/gsl>

using std::string;
using std::vector;
using std::unique_ptr;
using std::shared_ptr;
using std::make_unique;
using std::make_shared;

//having to release (i.e., move) the existing ptr is not ideal
//https://stackoverflow.com/questions/21174593/downcasting-unique-ptrbase-to-unique-ptrderived2
template<typename Derived, typename Base, typename Del>
std::unique_ptr<Derived, Del>
static_unique_ptr_cast( std::unique_ptr<Base, Del>&& p )
{
    auto d = static_cast<Derived *>(p.release());
    return std::unique_ptr<Derived, Del>(d, std::move(p.get_deleter()));
}

template<typename Derived, typename Base, typename Del>
std::unique_ptr<Derived, Del>
dynamic_unique_ptr_cast( std::unique_ptr<Base, Del>&& p )
{
    if(Derived *result = dynamic_cast<Derived *>(p.get())) {
        p.release();
        return std::unique_ptr<Derived, Del>(result, std::move(p.get_deleter()));
    }
    return std::unique_ptr<Derived, Del>(nullptr, p.get_deleter());
}

bool fileExists(const std::string& file);

inline bool caseInsCharCompareN(char a, char b) {
    return (toupper(a) == toupper(b));
}

/**
 * case-insensitive string comparison
 */
bool caseInsCompare(const string& s1, const string& s2);


#endif //SANDDB_COMMON_H
