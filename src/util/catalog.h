//
// Created by manolee on 12/18/17.
//

#ifndef SANDDB_CATALOG_H
#define SANDDB_CATALOG_H

#include <schema/types.h>
#include "common/common.h"

//namespace fs = std::experimental::filesystem;
namespace fs = boost::filesystem;

class Catalog {
private:
    //relationName -> filePath
    std::map<std::string,std::string> paths_data;
    //relationName -> schema
    std::map<std::string,std::unique_ptr<Relation>> relations;
public:
    void registerDataFolder(const std::string& path_folder);
    const std::string& getFilePath(const std::string& relName);

    const std::map<string, unique_ptr<Relation>> &getRelations() const;

    void addRelation(const string& relName, unique_ptr<Relation> relation);
};


#endif //SANDDB_CATALOG_H
