//
//  StringTest.hpp
//  RGAssignment1
//
//  Created by rick gessner on 1/10/21.
//

#ifndef StringTest_h
#define StringTest_h

#include "String.hpp"
#include "MemTrack.hpp"
#include <string>
#include <iostream>
#include <sstream>

namespace ECE141 {
  
  bool compareStreams(std::stringstream &aBuf1, std::stringstream &aBuf2) {
    std::string theStr1(aBuf1.str());
    std::string theStr2(aBuf2.str());
    return theStr1==theStr2;
  }

  bool doOCFTest() {
    const char *theStr="hello world ";
          
    std::stringstream theOutputA;
    std::stringstream theOutputB;
    
    std::string theStringA1(theStr);
    theOutputA << theStringA1;
    std::string theStringA2(theStringA1);
    theOutputA << theStringA2;

    ECE141::String theStringB1(theStr);
    theOutputB << theStringB1;
    ECE141::String theStringB2(theStringB1);
    theOutputB << theStringB2;

    if(!compareStreams(theOutputA,theOutputB)) {
      std::cout << "construction error";
      return false;
    }
    
    MemTrack::list.enable(true);
    {
      ECE141::String theStringB3(theStr);
    }
        
    if(MemTrack::list.leaked()) {
      MemTrack::list.empty("oops, you leaked!");
      return false;
    }
    
    return true;
  }
  
  //--------------------------------------------

  template <typename StringType>
  void mutate(std::ostream &anOutput) {
    StringType temp("hello ");
    StringType theString( temp + "world ");
    theString += "I'm ready! ";
    theString.insert(6,"there ",6);
    theString.insert(0,"Well ",5);
    theString.replace(23,3,"I am");
    theString.erase(0,5);
    theString[(int)theString.size()-2]='?';
    anOutput << theString;
  }

  bool doMutationTest() {

    std::stringstream theOutputA;
    mutate<std::string>(theOutputA);

    std::stringstream theOutputB;
    mutate<ECE141::String>(theOutputB);
    
    return compareStreams(theOutputA, theOutputB);
  }

  //--------------------------------------------

  template <typename StringType>
  bool compare() {
    StringType theStringA("hello there world!");
    StringType theStringB(theStringA);
    if(theStringA==theStringB) {
      StringType theStringC("hello there everyone!");
      if(theStringC<theStringA) {
        return true;
      }
    }
    return false;
  }

  int doComparisonTest() {
    bool theCompare1=compare<std::string>();
    bool theCompare2=compare<ECE141::String>();
    return theCompare1 && theCompare2;
  }

}

#endif /* StringTest_h */
