//
//  Database.cpp
//  RGAssignment2
//
//  Created by rick gessner on 2/27/21.
//

#include <string>
#include <iostream>
#include <map>
#include <fstream>
#include <cstring>

#include "Storage.hpp"
#include "Database.hpp"
#include "Config.hpp"

namespace ECE141 {
  
  Database::Database(const std::string aName, CreateDB)
    : name(aName), storage(stream) {
    std::string thePath = Config::getDBPath(name);
    stream.clear(); // Clear Flag, then create file...
    stream.open(thePath.c_str(), std::fstream::binary | std::fstream::in | std::fstream::out | std::fstream::trunc);
    stream.close();
    stream.open(thePath.c_str(), std::fstream::binary | std::fstream::in | std::fstream::out);

    //load in an empty meta block for now
    Block MetaBlock;
    MetaBlock.header.id = 0;
    MetaBlock.header.type = 'M';
    if(aName.size()<30-1){
      strncpy(MetaBlock.header.title,aName.c_str(),30);
    }
    
    std::string dbpayload("DB: ");
    dbpayload+=aName+" \n";
    strncpy(MetaBlock.payload, dbpayload.c_str(),30);
    storage.writeBlock(0, MetaBlock);
  }

  Database::Database(const std::string aName, OpenDB)
    : name(aName), storage(stream) {
      
      std::string thePath = Config::getDBPath(name);
      stream.open (thePath.c_str(), std::fstream::binary | std::fstream::in | std::fstream::out);
      //STUDENT: Load meta info from block 0
  }

  Database::~Database() { stream.close(); } //close stream

  bool Database::insert_block_next_free(Block& aBlock){
      int location_to_insert = storage.getFreeBlock();
      storage.writeBlock(location_to_insert, aBlock);
      return true;
  }

  int Database::get_TB_head_index(std::string theTitle){
    //super inefficient, transition to map
    Block aBlock;
    for(int i=0;i<storage.getBlockCount();i++){
      storage.readBlock(i,aBlock);
      if(aBlock.header.title==theTitle){
        if(aBlock.header.type=='E'){
          return i;
        }
      }
    }
    return 0;
  }

}
