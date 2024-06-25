//
//  Storage.cpp
//  RGAssignment2
//
//  Created by rick gessner on 2/27/21.
//



#include <cmath>
#include <cstdlib>
#include <optional>
#include <cstring>
#include "Storage.hpp"
#include "Config.hpp"

namespace ECE141 {
  // USE: ctor ---------------------------------------
  Storage::Storage(std::iostream &aStream) : BlockIO(aStream) {}

  // ACCESS FUNCTIONS ==========================================
  bool Storage::each(const BlockVisitor &aVisitor) {
    //implement this as a visitor pattern for storage blocks
    return true;
  }

  std::vector<int> Storage::blockLocationsFromTitle(std::string theTitle){
    //std::cout<<theTitle;
    //super inefficient
    std::vector<int> Blocks;
    Block aBlock;
    for(int i=0;i<getBlockCount();i++){
      readBlock(i,aBlock);
      //std::cout<<"\n"<<aBlock.header.title;
      if(aBlock.header.title==theTitle){
        //std::cout<<" true";
        Blocks.push_back(i);
      }
    }
    return Blocks;
  }
  
  //write logic to get the next available free block
  uint32_t Storage::getFreeBlock() {
   Block aBlock;
    for (int i = 0; i < getBlockCount(); i++){
      readBlock(i,aBlock);
      if(aBlock.header.type=='F'){return i;}
    }
    return (getBlockCount()); //This will return the tail of the database to write in
  }
  uint32_t getSecondFreeBlock(Storage* aStorage , int firstfree=0){
    if(firstfree=0){firstfree=aStorage->getFreeBlock();}

    int end=aStorage->getBlockCount();
    if(firstfree==end){
      return end+1;
    }
    Block aBlock;
    for (int i = firstfree; i < end; i++){
      aStorage->readBlock(i,aBlock);
      if(aBlock.header.type=='F'){return i;}
    }
    return end;
  }
  
  
  //write logic to mark a block as free...
  StatusResult Storage::markBlockAsFree(uint32_t aPos){
    Block FreeBlock;
    FreeBlock.header.type='F';
    writeBlock(aPos, FreeBlock);
    return StatusResult{Errors::noError};
  }

  // USE: for use with storable API...
  //   Write logic to mark a sequence of blocks as free)
  //   starting at given block number, following block sequence
  //   defined in the block headers...
  StatusResult Storage::releaseBlocks(std::string theTitle) {
    for(auto it: blockLocationsFromTitle(theTitle)){ markBlockAsFree(it); }
    return StatusResult{Errors::noError};
  }


  //=====================================================================
  //=====================================================================
                        // save// load //
  //=====================================================================
  //=====================================================================
  StatusResult Storage::save(std::vector<Block> BlockSet){
    int freeindex=getFreeBlock(); 
    for(auto aBlock : BlockSet){
      aBlock.header.next=getSecondFreeBlock(this,freeindex);
      writeBlock(freeindex, aBlock);
      freeindex=aBlock.header.next;
    }
    return StatusResult{Errors::noError};
  }

  std::vector<Block>  Storage::load(std::string theTitle){
    std::vector<Block> theBlocks;
    std::vector<int> indexes=blockLocationsFromTitle(theTitle);
    Block aBlock;
    for(auto it:indexes){
      readBlock(it,aBlock);
      theBlocks.push_back(aBlock);
    }
    return theBlocks;
  }


  ///============  traditional save and load     ==================

  StatusResult Storage::save(std::iostream &differentStream, StorageInfo &anInfo) {
    //get the number of blocks in the stream
    {
      differentStream.seekg (0, differentStream.end);
      int length = differentStream.tellg();
      differentStream.seekg (0, differentStream.beg);
      anInfo.size=length/kBlockSize;
    }
    //write the first block as a header.
    Block aBlock;
    differentStream.read(aBlock.payload,kPayloadSize);
    aBlock.header.type=(char)anInfo.type;
    aBlock.header.count=anInfo.size;
    strcpy(aBlock.header.title,anInfo.title);
    writeBlock(getFreeBlock(),aBlock);
    //todo implement something to track where next one is?
    //write all following blocks as datablocks
    aBlock.header.type='D';
    while(differentStream.read(aBlock.payload,kPayloadSize)){
      aBlock.header.pos+=1;
      writeBlock(getFreeBlock(),aBlock);
    }
    // incomplte needs a way to track how many characters is actually in the stream?
    // maybe not though because the objects will know when they end???
    return StatusResult{Errors::noError};
  }

  //Write logic to read an ordered sequence of N blocks, back into
  //a stream for your caller
  StatusResult Storage::load(std::iostream &differentStream, std::string theTitle) {
    std::vector<int> indexesToLoad(blockLocationsFromTitle(theTitle));
    Block aBlock;
    for(auto it: indexesToLoad){
      readBlock(it,aBlock);
      differentStream.write(aBlock.payload,kPayloadSize);
    }
    return StatusResult{Errors::noError};
  }

  
 
}

