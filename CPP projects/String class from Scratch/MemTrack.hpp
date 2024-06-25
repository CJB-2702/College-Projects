//
//  SFMemTrack.hpp
//  ECE180-assignment2
//
//  Created by rick gessner on 2/11/18.
//  Copyright © 2018 rick gessner. All rights reserved.
//

#ifndef MemTrack_hpp
#define MemTrack_hpp

#include <stdlib.h>
#include <string>
#include <iostream>
#include <map>
#include <typeinfo>

//#include <cxxabi.h>


//---------------------

class MemTrack {
  
  struct MemNode {
    MemNode(void *aPtr, size_t aSize, MemNode *aPrev, MemNode *aNext, size_t aFileNum=0, size_t aLineNum=0) :
      ptr(aPtr), next(aNext), prev(aPrev), type("void"), 
      filenum(aFileNum), linenum(aLineNum), size(aSize) { }
    
    void        *ptr;
    MemNode     *next;
    MemNode     *prev;
    std::string type;
    size_t      filenum;
    size_t      linenum;
    size_t      size;
  };
  
public:

  static MemTrack list;

  MemTrack();
  
  void    add(void *aPtr, size_t aSize, bool asArray=false,
              size_t aFilenum=0, int aLinenum=0);
  void    remove(void *aPtr, bool asArray=false);
  void    empty(const char *aMsg=nullptr);
  void    enable(bool enable=false) {enabled=enable;}
  bool    leaked() {return nullptr!=head;}
  
  size_t  trackFile(std::string &aFile);
/*    
  template <typename T>
  T* gps(T* aPtr, const char *aFile=nullptr, int aLine=0) {
    bool wasEnabled=enabled;
    enabled=false;
    std::string temp(aFile);
    head->filenum=trackFile(temp);
    head->linenum=aLine;
    int status;
    head->type=abi::__cxa_demangle(typeid(T).name(),0,0,&status);
    enabled=wasEnabled;
    return aPtr;
  }
//*/
    
protected:
  std::map<std::string, size_t> files;
  MemNode *head;
  bool    verbose;
  bool    enabled;
  size_t  count;
};

template <typename T>
struct New {
  //------------------ newHere utility... ---------------------------

  New(const T &aValue, const char *aFile=nullptr,  int aLine=0)  {
    std::string theFile(aFile);
    if(aFile) {
      char sep = '/'; // '\\' on windoze...
      size_t thePos = theFile.rfind(sep, theFile.length());
      if (thePos != std::string::npos) {
        theFile=theFile.substr(thePos+1, theFile.length() - thePos);
      }
    }
    //MemTrack::list.setFileLine(theFile, aLine); //cache for use in ::add()
    ptr=new T{aValue};
  }

  inline New& operator,(const char *aFilename) {
    return *this;
  }

  inline New& operator,(int linenum) {
    return *this;
  }

  operator T*() {return ptr;}
  T *ptr;
};


#define AT __FILE__, __LINE__

#endif /* MemTrack_hpp */
