//
//  Entity.hpp
//
//  Created by rick gessner on 4/03/21.
//  Copyright © 2021 rick gessner. All rights reserved.
//


#ifndef Entity_hpp
#define Entity_hpp

#include <stdio.h>
#include <vector>
#include <optional>
#include <memory>
#include <string>
#include <algorithm>

#include "Attribute.hpp"
#include "BasicTypes.hpp"
#include "Errors.hpp"
#include "Storage.hpp" 


//cb-----------------
#include "Row.hpp"  
#include "Database.hpp"
#include "View.hpp"

namespace ECE141 {
  
  using AttributeOpt = std::optional<Attribute>;
  using AttributeList = std::vector<Attribute>;

  //class Entity : public Storable  {
  class Entity : public Storable { // cb
  public:
    Entity(){};
    ~Entity(){};
    bool                  clear();

    StatusResult          encode(std::ostream &aWriter) override;
    StatusResult          decode(std::istream &aReader) override;

    std::vector<Block>    blockify();
    Block                 getHeaderBlock();
    Entity&               deblockify(std::vector<Block>);
    bool                  loadFromHeaderBlock(Block &aHeader);
    
    RowCollection         theRowCollection; 
    bool                  order_by(std::string attributeName);
    bool                  push_back(Row&aRow);


    //sets and gets
    AttributeList         attributes;
    int                   get_att_index(std::string attributeName);
    bool                  insert_attribute(Attribute &aAttribute);
    AttributeList         get_attributes();


    std::string     getTableName(){return tablename;};
    std::string     setTableName(std::string aName){tablename=aName;};
    std::string     tablename; //todo set to private and retrofit code.

    private:
    Row&                  validateRow(Row& aRow);
    friend class TB_viewer;
    uint32_t       hashString(const char* str); //Moved this here
    uint32_t       hashString(const std::string& aStr);
  };

  
  
}
#endif /* Entity_hpp */
