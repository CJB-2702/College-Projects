//
//  Storage.hpp
//  RGAssignment2
//
//  Created by rick gessner on 2/27/21.
//

#ifndef Storage_hpp
#define Storage_hpp

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <deque>
#include <functional>
#include "BlockIO.hpp"
#include "Errors.hpp"

namespace ECE141 {
      
  struct CreateDB {}; //tags for db-open modes...
  struct OpenDB {};

  const int32_t kNewBlock=-1;

  class Storable {
  public:
    virtual StatusResult  encode(std::ostream &anOutput)=0;
    virtual StatusResult  decode(std::istream &anInput)=0;
  };

  struct StorageInfo {
    StorageInfo(std::string theTitle, size_t theSize, int32_t aStartPos=0, BlockType aType=BlockType::data_block )
                : type(aType), start(aStartPos), size(theSize) {strcpy(title,theTitle.c_str());}
    BlockType type;
    int32_t   start; //block#
    size_t    size;
    char title[30];   //size_t    refId; formerly refid
  };
  
  using BlockVisitor = std::function<bool(const Block&, uint32_t)>;
  using BlockList = std::deque<uint32_t>;

  // USE: Our storage class (for stream IO)
  class Storage : public BlockIO {
  public:
    Storage(std::iostream &aStream);
    ~Storage(){};
    StatusResult save(std::iostream &aStream, StorageInfo &anInfo);
    StatusResult save(std::vector<Block> aSet);

    StatusResult        load(std::iostream &aStream, std::string theTitle);
    std::vector<Block>  load(std::string theTitle);

    StatusResult markBlockAsFree(uint32_t aPos);
    bool         each(const BlockVisitor &aVisitor);
    
    //StatusResult releaseBlocks(uint32_t aPos, bool aInclusive=false);//not useful
    StatusResult releaseBlocks(std::string theTitle);
    uint32_t     getFreeBlock(); //pos of next free (or new)... 
    std::vector<int> blockLocationsFromTitle(std::string theTitle);


    BlockList    available;
  };

}


#endif /* Storage_hpp */
