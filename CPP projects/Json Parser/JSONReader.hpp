//
//  JSONReader.hpp
//  Assignment3
//
//  Created by rick gessner on 2/6/21.
//

#ifndef JSONReader_hpp
#define JSONReader_hpp

#include <stdio.h>
#include <iostream>
#include <stack>
#include "JSONParser.hpp"
#include "JSONModel.hpp"

namespace ECE141 {

  //--------------------------------------------

  class JSONReader : public JSONParser {
  public:
    JSONReader(JSONModel &aModel, std::istream &anInputStream);
    bool willTokenize();
    bool tokenize();
    bool didTokenize(bool aStatus);
    
  protected:
    bool parseElements(char aChar);
    JSONModel &model;
    std::stack<JSONPart*> theStack;
    std::stack<TokenType> tokenStack;
    bool isValid;
    
  };
  
}

#endif /* JSONReader_hpp */
