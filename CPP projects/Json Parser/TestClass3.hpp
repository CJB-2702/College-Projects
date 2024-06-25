//
//  TestClass3.hpp
//  Assignment3
//
//  Created by rick gessner on 2/6/21.
//

#ifndef TestClass3_hpp
#define TestClass3_hpp

#include <stdio.h>
#include <string>
#include "Storable.hpp"

namespace ECE141 {

  enum class Color {red, green, blue};

  class TestClass3 : public Storable {
  public:
    
    TestClass3(const std::string &aName, Color aColor, const char *aBuffer);
    TestClass3(const TestClass3& atest){*this=atest;};
    TestClass3(){};
    virtual ~TestClass3();
    
    bool toJSON(std::ostream &aStream);

    TestClass3& operator=(const TestClass3& aClass){
      name=aClass.name;
      color=aClass.color;
      initials[0]=aClass.initials[0];
      initials[1]=aClass.initials[1];
      initials[2]=aClass.initials[2];
      initials[3]=aClass.initials[3];
      return *this;
    }
    

  protected:
    std::string name;
    char        initials[4];
    Color       color;
  };

}


#endif /* TestClass3_hpp */
