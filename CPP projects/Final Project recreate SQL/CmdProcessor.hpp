//
//  CmdProcessor.hpp
//  Database
//
//  Created by rick gessner on 3/17/19.
//  Copyright © 2019 rick gessner. All rights reserved.
//

#ifndef CmdProcessor_hpp
#define CmdProcessor_hpp

#include <stdio.h>
#include <string>
#include <cstdio>

#include "Statement.hpp"
#include "Database.hpp"
#include "Config.hpp"
#include "Tokenizer.hpp"
#include "Database.hpp"
#include "Config.hpp"
#include <filesystem>

#include "DB_viewer.hpp"


namespace ECE141 {


  class CmdProcessor { //processor interface
  public:
    CmdProcessor(std::ostream &anOutput);
    virtual ~CmdProcessor();      
    virtual CmdProcessor* recognizes(Tokenizer &aTokenizer)=0;
    virtual Statement*    makeStatement(Tokenizer &aTokenizer)=0;
    virtual StatusResult  run(Statement *aStmt)=0;
    
    bool tokens_match(std::vector<Token> aTokens, Tokenizer aTokenizer);
    bool tokens_exist_ordered(std::vector<Token> aTokens, Tokenizer aTokenizer);

	std::ostream &output;
  protected:
      std::string Current_Database_name; // get rid in future;
  };
   
}

#endif /* CmdProcessor */
