//
//  JSONParser.hpp
//  Assignment3
//
//  Created by rick gessner on 2/7/21.
//

#ifndef JSONParser_h
#define JSONParser_h

#include <iostream>
#include "JSONModel.hpp"

namespace ECE141 {

  using parseCallback = bool(char aChar);

  const char kColon = ':';
  const char kComma = ',';
  const char kQuote = '"';
  const char kBraceOpen  = '{';
  const char kBraceClose = '}';
  const char kBracketOpen = '[';
  const char kBracketClose = ']';


  class JSONParser {
  public:
      
      //----------------- simple parsing utilities ----------------------

    JSONParser(std::istream &anInput) : input(anInput) {}
      
    //consume/keep input as long as callback is true
    //if addterminal, the last char is added to result
    std::string readUntil(parseCallback aCallback, bool addTerminal) {
      std::string theResult;
      while (!input.eof() && !(*aCallback)(input.peek())) {
        theResult+=input.get();
      }
      if(addTerminal) {
        if(!input.eof()) {
          theResult+=input.get();
        }
      }
      return theResult;
    }

    //read chars as long as callback returns true
    std::string readWhile(parseCallback aCallback, bool addTerminal) {
      std::string theResult;

      while (!input.eof() && (*aCallback)(input.peek())) {
        theResult+=input.get();
      }
      if(addTerminal) {
        if(!input.eof()) {
          theResult+=input.get();
        }
      }
      return theResult;
    }
    
    //skip chars as long as callback returns true
    bool skipWhile(parseCallback aCallback) {
      while (!input.eof() && (*aCallback)(input.peek())) {
        input.get();
      }
      return true;
    }

    //if the char is next in stream, skip it...
    bool skipIfChar(char aChar) {
      return (aChar==input.peek()) ? aChar==input.get() : false;
    }
      
  protected:
      std::istream &input;
  };

}


#endif /* JSONParser_h */
