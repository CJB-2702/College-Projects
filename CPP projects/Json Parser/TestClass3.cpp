//
//  TestClass3.cpp
//  Assignment3
//
//  Created by rick gessner on 2/6/21.
//

#include <cstring>
#include "TestClass3.hpp"
#include <ostream>

namespace ECE141 {

  TestClass3::TestClass3(const std::string &aName,Color aColor, const char *aBuf){ 
      name=aName;
      color=aColor;     
      std::strncpy(initials, aBuf,3);
      initials[3]='\0';
  }

  TestClass3::~TestClass3() {}

  bool TestClass3::toJSON(std::ostream &aStream) {
     aStream<<"{\n"
      <<"\t\"meta\" : {\n"
      <<"\t\t\"class\" : \"TestClass3\",\n"
      <<"\t\t\"version\" : 1.0\n"
      <<"\t\t},\n"
      <<"\t\"members\" : {\n"
      <<"\t\t\"name\" : \""<<name<<"\",\n"
      <<"\t\t\"initials\" : \""<<initials<<"\",\n"
      <<"\t\t\"color\" : \""<<(color==Color::red?"red":(color==Color::green?"green":( color==Color::blue?"blue":"error")))<<"\"\n"
      <<"\t}\n"
      <<"}"
      <<std::flush;
    return true;
  }


}
