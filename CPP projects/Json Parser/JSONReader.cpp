//
//  JSONReader.cpp
//  Assignment3
//
//  Created by rick gessner on 2/6/21.
//

#include "JSONReader.hpp"
#include <cctype>
#include <map>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <cstring>
#include <iostream>


namespace ECE141 {

  using parseCallback = bool(char aChar);

  bool isWhitespace(char aChar) {
    static const char* theWS = " \t\r\n\b\xff";
    return strchr(theWS,aChar);
  }

  bool isColon(char aChar) {return kColon==aChar ? true  : false;}
  bool isComma(char aChar) {return kComma==aChar ? true  : false;}
  bool isQuote(char aChar) {return kQuote==aChar ? true  : false;}
  bool isContant(char aChar) {return isalnum(aChar) || '.'==aChar;}

  //

  //------------------------------------------------------

  JSONReader::JSONReader(JSONModel &aModel, std::istream &anInputStream) : JSONParser(anInputStream), model(aModel) {
    anInputStream >> std::skipws;
    JSONPart* start=&aModel;
    theStack.push(start);
  }
  
  //-----------------------------------------------------

  TokenType determineType(char aChar) {
    const char* kConstantChars="01234567890tf";
    switch(aChar) {
      case kQuote: return TokenType::quoted;
      case kColon: return TokenType::colon;
      case kBraceOpen: return TokenType::openObject;
      case kBracketOpen: return TokenType::openList;
      case kBraceClose: return TokenType::closeObject;
      case kBracketClose: return TokenType::closeList;
      default:
        return strchr(kConstantChars, aChar)
          ? TokenType::constant : TokenType::unknown;
    }
    return TokenType::unknown;
  }

  bool JSONReader::parseElements(char aChar) {
    //std::stack<JSONPart*> theStack;//a reader object
    //std::stack<TokenType> tokenStack;
    TokenType theType = determineType(aChar);
    std::string temp;

    switch(theType) {
      case TokenType::openObject:{
        //create an object in the reader that is ready to accept an element
        JSONPart *pObject=new JSONObject;
        theStack.top()->push_back(pObject);
        theStack.push(pObject);
        tokenStack.push(TokenType::openObject);
        break;
      }

      case TokenType::openList:{
        JSONPart *pList=new JSONList;
        theStack.top()->push_back(pList);
        theStack.push(pList);
        tokenStack.push(TokenType::openList);
        //create a vector ready to accept a series of objects/consts/strConsts
        break;
      }
      case TokenType::closeList:{
        //forget about the current object
        if(tokenStack.top()!=TokenType::openList){
          std::cout<<"close list mismatch\n\n error\n\n";
        }else{
          //close the list object
          tokenStack.pop();
          theStack.pop();
        }
        //gotta pop twice because a list is always paired with an
        if(theStack.top()->type==JSONType::element){
            theStack.pop();
            tokenStack.pop();
        }
        break;
      }
      case TokenType::closeObject:{
        if(tokenStack.top()!=TokenType::openObject){std::cout<<"close Object mismatch\n\n error\n\n";}
        tokenStack.pop();
        theStack.pop();
        //forget about the current object
        if(theStack.top()->type==JSONType::element){
            theStack.pop();
            tokenStack.pop();
        }
        break;
      }
      case TokenType::quoted:{
        temp = readUntil(isQuote, false);
        skipIfChar(kQuote);
        //std::cout<<temp<<"\n";
        //if the last token was a colon or a list bracket the string is a strconst value
        if(tokenStack.top()==TokenType::colon || tokenStack.top()==TokenType::openList){
          JSONPart* pStrCnst = new JSONStrConst(temp);
          theStack.top()->push_back(pStrCnst);
          //if its the value in a key value pair go back to the parent of the key value pair.
          if(tokenStack.top()==TokenType::colon){
            theStack.pop();
            tokenStack.pop();
          }//if its in a list just keep adding

        }else{//its a key in a key value pair
          JSONElement* pElement=new JSONElement(temp);
          pElement->setKey(temp);
          theStack.top()->push_back(pElement);
          theStack.push(pElement);
        }
        break;    
      } 
      case TokenType::constant:{
        temp=readWhile(isContant, false);
        temp.insert(0, 1, aChar);
        //always gonna be a value in a key value pair
        //std::cout<<temp;
        theStack.top()->push_back(new JSONConst(temp));
        if(tokenStack.top()==TokenType::colon){
          tokenStack.pop();
          theStack.pop();
        }
        break;
      }
      case TokenType::colon:{
        tokenStack.push(TokenType::colon);
        break; //likely between key/value 
      }               
      default: break;
    }
    return true;
  }
  
//--------------------------------------------------------------------

  bool JSONReader::willTokenize() {    
    return true;
  }

  //--------------------------------------------------------------------

  bool JSONReader::tokenize() {
    if(willTokenize()) {
      
      //bool isValid=true;// moved to be a component of the class
      isValid=true;
      while(isValid) {
        skipWhile(isWhitespace);
        if(!input.eof()) {
          char theChar=input.get();
          isValid = parseElements(theChar);
          skipWhile(isWhitespace);
          skipIfChar(kComma);
        }
        else break;
      }
      return didTokenize(isValid);
    }
    return true;
  }

  //-----------------------------------------------------------

  bool JSONReader::didTokenize(bool aState) {
    return aState;    
  }
  

}
