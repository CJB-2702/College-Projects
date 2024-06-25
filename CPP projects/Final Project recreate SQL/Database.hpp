//
//  Database.hpp
//  RGAssignment2
//
//  Created by rick gessner on 2/27/21.
//

#ifndef Database_hpp
#define Database_hpp

#include <stdio.h>
#include <fstream> 
#include "Storage.hpp"

namespace ECE141 {

  class Database : public Storable {
  public:
    
    Database(const std::string aPath, CreateDB);
    Database(const std::string aPath, OpenDB);
    virtual ~Database();
    StatusResult          encode(std::ostream &aWriter) override {return StatusResult{Errors::noError};};
    StatusResult          decode(std::istream &aReader) override {return StatusResult{Errors::noError};};

    bool            insert_block_next_free(Block &aBlock);
    int             get_TB_head_index(std::string aTitle);
  
    std::string     name;
    Storage         storage;
    private:
    std::fstream    stream; //cb:@ming why is this here. possible errors to come
  };

}
#endif /* Database_hpp */
