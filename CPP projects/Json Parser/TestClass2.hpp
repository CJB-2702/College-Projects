//
//  TestClass2.hpp
//  Assignment3
//
//  Created by rick gessner on 2/6/21.
//

#ifndef TestClass2_hpp
#define TestClass2_hpp

#include <stdio.h>
#include <string>
#include "Storable.hpp"

namespace ECE141 {
  
  struct Metadata {
    char    prop1;
    uint8_t prop2;
    bool    prop3;
  };

  class TestClass2 : public Storable {
  public:
    
    TestClass2(const std::string &aName, size_t aSize,const Metadata &aMeta);
    TestClass2(){};
    virtual ~TestClass2();
    
    bool toJSON(std::ostream &aStream);

  protected:
    std::string name;
    size_t      size;
    Metadata    meta;
  };

}

#endif /* TestClass2_hpp */
