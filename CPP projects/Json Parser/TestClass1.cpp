//
//  TestClass1.cpp
//  Assignment3
//
//  Created by rick gessner on 2/6/21.
//

#include "TestClass1.hpp"

namespace ECE141 {

  TestClass1::TestClass1(const std::string &aName,bool aUsed, float anAmount, int aCount)
    : name(aName), used(aUsed), amount(anAmount), count(aCount) {}

  TestClass1::~TestClass1() {}

  bool TestClass1::toJSON(std::ostream &aStream) {
    aStream<<"{\n"
      <<"\t\"meta\" : {\n"
      <<"\t\t\"class\" : \"TestClass1\",\n"
      <<"\t\t\"version\" : 1.0\n"
      <<"\t\t},\n"
      <<"\t\"members\" : {\n"
      <<"\t\t\"name\" : \""<<name<<"\",\n"
      <<"\t\t\"count\" : "<<count<<",\n"
      <<"\t\t\"amount\" : "<<amount<<",\n"
      <<"\t\t\"used\" : "<<(used?"true":"false")<<"\n"
      <<"\t}\n"
      <<"}"
      <<std::flush;
    return true;
  }

}
