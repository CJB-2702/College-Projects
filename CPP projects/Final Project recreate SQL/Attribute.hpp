//
//  Attribute.hpp
//
//  Created by rick gessner on 4/02/21.
//  Copyright © 2021 rick gessner. All rights reserved.
//

#ifndef Attribute_hpp
#define Attribute_hpp

#include <stdio.h>
#include <string>
#include "Storage.hpp"
#include "keywords.hpp"
#include "BasicTypes.hpp"
#include <any>
#include <iostream>

namespace ECE141 {
  class Attribute : public Storable {
    public:
    std::string   id;
    DataTypes     type;
    int           field_length;
    bool          auto_increment;
    int           current_increment_value;
    bool          key;
    bool          nullable;

    const std::string attribute_text_flag="#attribute#";// make static?
    Attribute();
    ~Attribute() {};
    Attribute& clear();

    StatusResult        decode(std::istream& aReader) ;
    StatusResult        encode(std::ostream &aWriter);
    
    std::string         type2String(DataTypes& type);
    int                 type2CellIndex();
      
  };  
}
#endif /* Attribute_hpp */
