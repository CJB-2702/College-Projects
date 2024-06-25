//
//  CommandProcessor.cpp
//  ECEDatabase
//
//  Created by rick gessner on 3/30/18.
//  Copyright © 2018 rick gessner. All rights reserved.
//

#include <iostream>
#include "Application.hpp"
#include "Tokenizer.hpp"
#include <memory>
#include <vector>
#include <algorithm> //For autograder
#include <ctime>

namespace ECE141 {
  
  Application::Application(std::ostream &anOutput)
    : CmdProcessor(anOutput), Db_proc(anOutput,&activeDB), 
      SQL_proc(anOutput,&activeDB), QRY_Proc(anOutput,&activeDB){activeDB=nullptr;}
  
  Application::~Application() {delete activeDB;}

  // USE: -----------------------------------------------------
  
  bool isKnown(Keywords aKeyword) {
    static Keywords theKnown[]={Keywords::quit_kw, Keywords::help_kw, Keywords::version_kw};  //Add whatever keywords you want to recognize
    auto theIt = std::find(std::begin(theKnown), std::end(theKnown), aKeyword);
    return theIt!=std::end(theKnown);
  }

  CmdProcessor* Application::recognizes(Tokenizer &aTokenizer) { 
    if(isKnown(aTokenizer.current().keyword)) {
      return this;
    }
 
    if (auto *Db_proc_ptr=Db_proc.recognizes(aTokenizer)) { //If Db can recognize, return a non nullptr, if not, return nullptr
      return Db_proc_ptr;
    }

    if (auto* SQL_proc_ptr = SQL_proc.recognizes(aTokenizer)) { //If SQL can recognize, return a non nullptr, if not, return nullptr
      return SQL_proc_ptr;
    }
    if (auto * QRY_proc_ptr=QRY_Proc.recognizes(aTokenizer)){
      return QRY_proc_ptr;
    }
    return nullptr; //ask other preprocessors later
  }

  StatusResult shutdown(std::ostream &anOutput) {
    anOutput << "DB::141 is shutting down."<< std::endl;;
    return StatusResult(ECE141::userTerminated);
  }

  StatusResult help(std::ostream& anOutput) { //help_kw
      anOutput << "Help system ready." << std::endl;
      return StatusResult(ECE141::noError);
  }

  StatusResult version(std::ostream& anOutput) { //version_kw
      anOutput << "Version 0.6." << std::endl;
      return StatusResult(ECE141::noError);
  }



  StatusResult Application::run(Statement* aStatement) {
    
    //STUDENT: Add support for HELP and VERSION commands...

    switch(aStatement->getType()) { //Have to specifify what kw we are able to recognize in line 26
      case Keywords::quit_kw: 
        delete activeDB;
        return shutdown(output);
      case Keywords::help_kw: return help(output);
      case Keywords::version_kw: return version(output);

      default: break;
    }
    return StatusResult{Errors::noError};
  }
    
  // USE: retrieve a statement based on given text input...
  Statement* Application::makeStatement(Tokenizer &aTokenizer) {
    Token theToken=aTokenizer.current();
    if (isKnown(theToken.keyword)) {
      aTokenizer.next(); //skip ahead...
      return new Statement(theToken.keyword);
    }
    return nullptr;
  }

  //build a tokenizer, tokenize input, ask processors to handle...
  StatusResult Application::handleInput(std::istream &anInput){
    //is this leaking?
    clock_t begin = std::clock();

    Tokenizer theTokenizer(anInput);
    ECE141::StatusResult theResult=theTokenizer.tokenize();
    while (theResult && theTokenizer.remaining()) { //Changed to theTokenizer.remaining() instead of .more(), might break things later idk
      if(auto *theProc=recognizes(theTokenizer)) { //Determines correct processor pointer to use
        if(auto *theCmd=theProc->makeStatement(theTokenizer)) {
          theResult=theProc->run(theCmd);
          if(theResult) theTokenizer.skipIf(';');
          delete theCmd;
          
          //all other processors are timed commands
          if(theProc!= this){
            clock_t end = std::clock();
            double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
            output<<" ("<<elapsed_secs<<" secs)"<<std::endl;
          }
          //delete the Proc;
        }
      }
      else theTokenizer.next(); //force skip ahead...
    }
    return theResult;
  }

}
