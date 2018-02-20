//
// Created by manolee on 11/24/17.
//

#ifndef SANDDB_CSV_HELPER_H
#define SANDDB_CSV_HELPER_H

#include "common/common.h"
#include <schema/types.h>
#include <boost/iostreams/device/mapped_file.hpp>
#include <datum/table.h>

class CSVHelper {
public:
    CSVHelper(const char* path_schema, const char* path_data);

private:
    Relation rel;
    //std::unique_ptr<char> csv_schema;
    std::unique_ptr<char> csv_data;

    std::unique_ptr<Table> data;
};


#endif //SANDDB_CSV_HELPER_H
