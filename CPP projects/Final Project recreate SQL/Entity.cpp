//
//  Entity.cpp
//
//  Created by rick gessner on 4/03/21.
//  Copyright © 2021 rick gessner. All rights reserved.
//

#include <sstream>
#include "Entity.hpp"


namespace ECE141 {

 //STUDENT: Implement the Entity class here...
  
  const int gMultiplier = 37;


  bool Entity::clear(){
    theRowCollection.clear();
    attributes.clear();
    tablename.clear();
    return true;
  }

  //=======================================================================
  //---------------------ENCODING DATA INTO STORAGE------------------------
  //-----------------------------------------------------------------------
    bool encodeHead(std::ostream &aWriter, Entity* aTable){
    aWriter.flush();
    aWriter << "Table: " << aTable->tablename 
            << " \nN_attributes: " << aTable->attributes.size() 
            << " \nN_rows: " << aTable->theRowCollection.size() << " \n " ;

    for(auto it : aTable->attributes){ it.encode(aWriter); }
    aWriter << " \n ";
    return true;
  }
  
  Block               Entity::getHeaderBlock(){
    Block aBlock;
    int i=0, n=theRowCollection.size()+1;
    //load header block
    aBlock.header.type='E';     //char version of block type
    aBlock.header.count=n;    //how many parts
    aBlock.header.pos=i;      //i of n...
    aBlock.header.refId=0;    //e.g. id of thing it refers to
    aBlock.header.id=0;       //use this anyway you like
    strcpy(aBlock.header.title,tablename.c_str());
    //load payload
    std::stringstream ss;
    encodeHead(ss,this);
    ss.read(aBlock.payload, kPayloadSize);
    return aBlock;
  };

  std::vector<Block> Entity::blockify(){
    std::vector<Block> theBlocks;
    Block aBlock;
    //load metadata
    theBlocks.push_back(getHeaderBlock());

    //load row data
    int i=0, n=theRowCollection.size()+1;
    for(auto aRow :theRowCollection){
      theBlocks.push_back(aRow.Blockify(tablename,n,++i));
    }
    return theBlocks;
  }



  StatusResult Entity::encode(std::ostream &aWriter) {
    aWriter.flush();
    encodeHead(aWriter, this);
    int i=1;
    for(auto it : theRowCollection){
      it.rowNumber=i;
      it.encode(aWriter);
      aWriter << " \n ";
    }
    aWriter << " \n##END##\n ";
    return StatusResult{noError};
  }



 
  //=======================================================================
  //-----------------------RETRIEVING DATA --------------------------------
  //-----------------------------------------------------------------------
  bool decodeHead(std::istream &aReader, Entity* aTable, int* rowcount= nullptr){
    aTable->clear();
    std::string metastr;
    aReader>>metastr>>aTable->tablename;
    if(metastr!="Table:"){
      std::cout<<"\ntable load error\n";
      return false;
    }

    int n_attributes,n_rows;
    aReader>> metastr >> n_attributes 
           >>metastr >>n_rows;
    
    Attribute it;
    for(int i=0; i<n_attributes ;i++){
      it.decode(aReader);
      aTable->attributes.push_back(it);
    }
    if(rowcount){*rowcount=n_rows;}
    return true;
  }


  Entity& Entity::deblockify(std::vector<Block> blocks){
    if(blocks.empty()){
      std::cout<<"Error, table not found\n";
      return *this;
    }
    clear();
    loadFromHeaderBlock(blocks[0]);
    //load rest of rows
    auto it= begin(blocks); it++;
    Row aRow;
    for( ; it!=end(blocks); it++){
      aRow.Deblockify(*it);
      theRowCollection.push_back(aRow);
    } 
    return *this;   
  }

  bool  Entity::loadFromHeaderBlock(Block &aHeader){
    std::stringstream ss;
    ss.write(aHeader.payload,kPayloadSize);
    decodeHead(ss, this, nullptr);
    return true;
  }


  StatusResult Entity::decode(std::istream &aReader) {
    int n_rows=0;
    std::string metastr;
    decodeHead(aReader, this, &n_rows);
    // decode rows;
    Row it ;
    for( int i=0; i< n_rows;i++){
      it.decode(aReader);
      theRowCollection.push_back(it);
    }
    aReader>>metastr;
    //check end 
    return StatusResult{noError};
  }


  //=======================================================================
  //----------------------- MISC            --------------------------------
  //-----------------------------------------------------------------------
   bool  Entity::push_back(Row&aRow){
      theRowCollection.push_back(validateRow(aRow));
      return true;
   }


  Row& Entity::validateRow(Row& aRow){
      //std::variant<int, bool, float,  std::time_t, std::string>;
      int numAtts = attributes.size();
      if (aRow.cells.empty()) {
          std::cout << "aRow is empty" << std::endl;
          return aRow;
      }
      for (int i = 0; i < numAtts; i++) {
        //validate that the cell has the right datatype
        //if its a number and wrong its ok
        if (aRow.cells[i].type() != (char)attributes[i].type) {
          //std::cout << "CELL DATATYPE DOES NOT MATCH ATTRIBUTE in validateRow" << std::endl;
          //usually just needs to be converted from string to an int or whatever
          if(aRow.cells[i].index()>3){//make into a convert function?
            switch((char)attributes[i].type){
              case 'B': aRow.cells[i]=(bool)std::stoi(std::get<std::string>(aRow.cells[i]));break;
              case 'I': aRow.cells[i]=(int)std::stoi(std::get<std::string>(aRow.cells[i]));break;
              case 'F': aRow.cells[i]=(float)std::stof(std::get<std::string>(aRow.cells[i]));break;
              case 'D': std::cout<< "\nTime error in cell validation\n";
            }
          }
          if(aRow.cells[i].type()=='T'){std::cout<< "\nTime error in cell validation\n";}
        }

        //if auto increment set to value
        if(attributes[i].auto_increment){aRow.cells[i]=++attributes[i].current_increment_value;}
      }
      return aRow;
    }

  bool Entity::insert_attribute(Attribute& aAttribute){
      attributes.push_back(aAttribute);
      return true;
  }

  AttributeList Entity::get_attributes(){
      return attributes;
  }


    
  bool Entity::order_by(std::string attributeName){
      int AttIndex = get_att_index(attributeName);
      if (AttIndex == -1) {
          //std::cout << "ATTRIBUTE DIDN'T EXIST" << std::endl;
          return false;
      }
      std::sort(theRowCollection.begin(), theRowCollection.end(), [&AttIndex](const Row& leftRow, const Row& rightRow) {
          return leftRow.cells[AttIndex] < rightRow.cells[AttIndex];
          });
      return false;
  }

  int Entity::get_att_index(std::string attributeName){ //Gets attribute's index
      int index = 0;
      for (auto aAtt : attributes) {
          if (aAtt.id == attributeName) {
              return index;
          }
          index++;
      }
      return -1;
  }

  uint32_t Entity::hashString(const char *str) {
    uint32_t h{0};
    unsigned char *p;
    for (p = (unsigned char*)str; *p != '\0'; p++)
      h = gMultiplier * h + *p;
    return h;
  }

  uint32_t Entity::hashString(const std::string &aStr) {
    return hashString(aStr.c_str());
  }  

}
