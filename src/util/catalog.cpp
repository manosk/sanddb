//
// Created by manolee on 12/18/17.
//

#include "catalog.h"

void Catalog::registerDataFolder(const std::string& path_folder)    {

    std::cout << path_folder << std::endl;
//    for (auto& p : fs::directory_iterator(path_folder))   {
//
//    }
    fs::path p(path_folder);
    if (fs::exists(p))  {
        if(fs::is_directory(p)) {
            for(auto& df : fs::directory_iterator(p)) {
                auto& fname = df.path().filename().generic_string();
                auto& path = df.path().generic_string();
                //std::cout << df.path().filename().generic_string() << " and " << df.path().generic_string() << std::endl;
                auto pos_suffix = fname.find(".");
                auto len = fname.size();
                len = pos_suffix == string::npos ? len : pos_suffix;
                auto name_nosuffix = fname.substr(0,len);
                paths_data[name_nosuffix] = path;
            }
        } else  {
            throw std::logic_error("Expected folder path");
        }
    } else  {
        throw std::runtime_error("Could not open file");
    }
}

const std::string& Catalog::getFilePath(const std::string& relName)    {
    if(paths_data.find(relName) == paths_data.end())    {
        throw std::runtime_error("no registered path for relation");
    }
    return paths_data.at(relName);
}

const std::map<string, unique_ptr<Relation>> &Catalog::getRelations() const {
    return relations;
}

void Catalog::addRelation(const string& relName, std::unique_ptr<Relation> relation) {
    relations[relName] = std::move(relation);
}


