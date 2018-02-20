//
// Created by manolee on 11/24/17.
//

#include "csv_helper.h"

//Schema example -> ID:int,NAME:string,PRICE:double,DISCOUNT:double
CSVHelper::CSVHelper(const char* path_schema, const char* path_data) {

    using boost::iostreams::mapped_file_source;
    using std::experimental::string_view;

    /**
     * Handle schema
     */
    mapped_file_source file_schema(path_schema);
    if(!file_schema.is_open())   {
        throw std::logic_error("Schema path not found");
    }
    string_view schema_str(file_schema.data(),file_schema.size());

    vector<Attribute> atts;

    auto endPos = schema_str.find(",",0);
    decltype(endPos) startPos = 0;
    while(startPos < endPos)
    {

        //manipulate sth like ID:int
        auto colonPos = schema_str.find(":",startPos);
        string name(&schema_str.at(startPos),colonPos - startPos);
        //std::cout << "Extracted name: " << name << std::endl;
        string dt(&schema_str.at(colonPos + 1),endPos - colonPos - 1);
        //std::cout << "Extracted datatype: " << dt << std::endl;

        //NOTE:
        //Unlike std::basic_string::data() and string literals,
        //string_view::data() may return a pointer to a buffer that is not null-terminated.
        // Therefore it is typically a mistake to pass data() to a routine
        // that takes just a const CharT* and expects a null-terminated string.
        atts.emplace_back(std::move(name),std::move(dt));

        startPos = (endPos + 1);
        endPos = schema_str.find(",",startPos) ;
        endPos = endPos == schema_str.npos ? schema_str.size() : endPos;
        //std::cout << "Next loop " << endPos << " vs " << schema_str.npos << std::endl;
    }
    file_schema.close();
    this->rel = Relation(path_schema, atts);

    /**
     * Handle data
     */
    data = std::make_unique<Table>(path_data, atts);

    mapped_file_source file_data(path_data);
    if(!file_data.is_open())   {
        throw std::logic_error("Data path not found");
    }
    string_view data_str(file_data.data(),file_data.size());

    auto fileSize = file_data.size();
    decltype(fileSize) offset = 0;

    //http://en.cppreference.com/w/cpp/header/charconv would have been convenient to replace atoi / atof
    //C-like conversions for  now
    unsigned rowNo = 0;
    while(offset < fileSize)    {
        unsigned colNo = 0;
        for(auto& attr: atts)    {
            auto start = offset;
            offset = data_str.find_first_of(",\n",start);

            switch(attr.getType())  {
                case Type::BOOL:
                {
                    //assume it's 0 or 1
                    auto res = std::strtol(&data_str.at(start),nullptr,10);
                    bool val = res != 0 ;
                    data->setColumn(rowNo,colNo++,&val);
                    break;
                }
                case Type::DOUBLE:  {
                    double val = std::strtod(&data_str.at(start),nullptr);
                    data->setColumn(rowNo,colNo++,&val);
                    break;
                }
                case Type::FLOAT:   {
                    float val = std::strtof(&data_str.at(start),nullptr);
                    data->setColumn(rowNo,colNo++,&val);
                    break;
                }
                case Type::INT: {
                    //Assuming int32 datatype for now
                    int val = std::strtol(&data_str.at(start),nullptr,10);
                    data->setColumn(rowNo,colNo++,&val);
                    break;
                }
                case Type::STRING:  {
                    string val = std::string(data_str.substr(start,offset).data(),offset-start);
                    data->setColumn(rowNo,colNo++,&val);
                    break;
                }
            }
            offset++;
        }
        rowNo++;
    }
    file_data.close();
}
