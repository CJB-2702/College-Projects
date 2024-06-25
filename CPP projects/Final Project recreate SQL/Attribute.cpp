//
//  Attribute.hpp
//
//  Created by rick gessner on 4/02/21.
//  Copyright � 2021 rick gessner. All rights reserved.
//


#include "Attribute.hpp"
#include <filesystem>

namespace ECE141 {
    Attribute::Attribute(){
       clear();
    }
    Attribute& Attribute::clear(){
        type = DataTypes::no_type;
        field_length = 0;
        current_increment_value=0;
		auto_increment = false;
		key = false;
		nullable = true;
        return *this;
    }

    StatusResult        Attribute::encode(std::ostream & aWriter) {
        aWriter <<" "<< attribute_text_flag <<" "
                << id << " " 
                << (int)type << " " 
                << auto_increment << " "
                << current_increment_value<< " "
                << key << " " 
                << nullable<< " " 
                << field_length << " ##END## "; 
        StatusResult exitStatus{ Errors::noError };
        return exitStatus; 
    }

    StatusResult        Attribute::decode(std::istream & aReader) {
        int aType;
        std::string isAtt;
        aReader>> isAtt;
        if(isAtt!=attribute_text_flag){std::cout<<"\nAttribute linup Error, Check inputs\n";}

        aReader >> id >> aType >> auto_increment >> current_increment_value >> key
                >> nullable >> field_length>>isAtt;
        type=static_cast<DataTypes>(aType);
        StatusResult exitStatus{ Errors::noError };
        return exitStatus;
    }


    // canidate for view class. Make friends and reimplement



    // canidate for view class. Make friends and reimplement
    std::string Attribute::type2String(DataTypes& type) {
        switch (type) {
        case DataTypes::bool_type:        return "boolean";
        case DataTypes::datetime_type:    return "datetime";
        case DataTypes::float_type:       return "float";
        case DataTypes::int_type:         return "integer";
        case DataTypes::varchar_type:     return "varchar";
        default: return "N/A";
        }
    }
    int Attribute::type2CellIndex(){
        //using VariantData = std::variant< bool,int, float, std::time_t, std::string>;
        switch (type) {
        case DataTypes::bool_type:        return 0;
        case DataTypes::datetime_type:    return 3;
        case DataTypes::float_type:       return 2;
        case DataTypes::int_type:         return 1;
        case DataTypes::varchar_type:     return 4;
        default: return 4;
        }
    };

    
}