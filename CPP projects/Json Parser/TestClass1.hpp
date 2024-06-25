//
//  TestClass1.hpp
//  Assignment3
//
//  Created by rick gessner on 2/6/21.
//

#ifndef TestClass1_hpp
#define TestClass1_hpp

#include <stdio.h>
#include <string>
#include "Storable.hpp"

namespace ECE141 {

  class TestClass1 : public Storable {
  public:
    
    TestClass1(const std::string &aName, bool aUsed,float anAmount, int aCount);
    TestClass1(){};
    virtual ~TestClass1();
    
    bool toJSON(std::ostream &aStream);
        
  protected:
    std::string name;
    int         count;
    float       amount;
    bool        used;    
  };

}

#endif /* TestClass1_hpp */
