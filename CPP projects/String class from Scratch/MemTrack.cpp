//
//  SFMemTrack.cpp
//  live_demo
//
//  Created by rick gessner on 2/10/18.
//  Copyright © 2018 rick gessner. All rights reserved.
//

#include "MemTrack.hpp"
#include <iostream>

MemTrack MemTrack::list;

MemTrack::MemTrack() {
  head=nullptr;
  verbose=false;
  enabled=false;
  count=0;
}

size_t MemTrack::trackFile(std::string &aFile) {
  
    char sep = '/';
#ifdef _WIN32
    sep = '\\';
#endif
    size_t theLen = aFile.length();
    size_t thePos = aFile.rfind(sep, theLen);
    if (thePos != std::string::npos) {
      aFile=aFile.substr(thePos+1, theLen - thePos);
    }

  auto theIter = MemTrack::files.find(aFile);
  if(theIter==MemTrack::files.end()) {
    size_t theSize= MemTrack::files.size()+1;
    MemTrack::files[aFile]=theSize;
    return theSize;
  }
  return theIter->second;
}

void MemTrack::add(void *aPtr, size_t aSize, bool asArray, size_t aFilenum, int aLinenum) {
  if(enabled) {
    if(verbose) {
      static const char* suffixes[]={"","[]"}; //used for output...
      std::cout<<"    new" << suffixes[asArray] << "(" << aSize << ")=" << aPtr <<std::endl;
    }
    
    enabled=false; //don't track our own allocations...
    head=new MemNode(aPtr, aSize, nullptr, head, aFilenum, aLinenum);
    count++;
    enabled=true;
  }
}

void MemTrack::remove(void *aPtr, bool asArray /*false*/) {
  if(enabled) {
    if (verbose) {
      static const char* suffixes[]={"","[]"}; //used for output...
      std::cout<< "    delete " << suffixes[asArray] << aPtr << std::endl;
    }
    
    MemNode *thePrev=nullptr;
    MemNode *theCurr=head;
    
    while(theCurr) {
      if(theCurr->ptr==aPtr) {
        if(thePrev) {
          thePrev->next=theCurr->next;
        }
        else {
          head=theCurr->next;
        }
        enabled=false; //don't track deletion of node...
        delete theCurr;
        enabled=true;
        count--;
        return;
      }
      thePrev = theCurr;
      theCurr = theCurr->next;
    }
  }
}

void MemTrack::empty(const char* aMsg /*nullptr*/) {
  bool wasEnabled=enabled;
  enabled=false;
  if(MemNode *theCurr=head) {
    if(aMsg) {
      std::cout << aMsg << std::endl;
    }
    while(theCurr) {
      MemNode *theNext=theCurr->next;
      std::cout << "\t" << theCurr->ptr << " (" << theCurr->type << "*) ";
      if(theCurr->filenum+theCurr->linenum) {
        for(auto &thePair : MemTrack::files) {
          if(thePair.second==theCurr->filenum) {
            std::cout << "from " << thePair.first << ":" << theCurr->linenum ;
            break;
          }
        }
      }
      std::cout << "\n";
      delete theCurr;
      theCurr = theNext;
    }
    head=nullptr;
  }
  enabled=wasEnabled;
}


//------------------ Overloading Global new[] operator ------------------------

void* operator new(size_t aSize)  {
  void *thePtr = (void*)malloc(aSize);
  MemTrack::list.add(thePtr,aSize);
  return thePtr;
}

// Overloading Global new[] operator
void* operator new[](size_t aSize) {
  void *thePtr = (void*)malloc(aSize);
  MemTrack::list.add(thePtr,aSize,true);
  return thePtr;
}

// Overloading Global delete[] operator
void operator delete[]( void* aPtr, size_t aSize) throw()  {
  MemTrack::list.remove(aPtr,true);
  free(aPtr);
}

// Overloading Global delete[] operator
void operator delete( void* aPtr ) throw()  {
  MemTrack::list.remove(aPtr);
  free(aPtr);
}



