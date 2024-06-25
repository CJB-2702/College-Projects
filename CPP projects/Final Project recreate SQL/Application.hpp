//
//  AppProcessor.hpp
//  Database5
//
//  Created by rick gessner on 4/4/20.
//  Copyright © 2020 rick gessner. All rights reserved.
//

#ifndef Application_hpp
#define Application_hpp

#include <stdio.h>
#include "CmdProcessor.hpp"
#include "DbProcessor.hpp"
#include "Database.hpp"
#include "SQLProcessor.hpp" //Added
#include "QRY_Processor.hpp"
#include "Statement.hpp"


namespace ECE141 {

    class Application : public CmdProcessor {
  public:
    
    Application(std::ostream &anOutput);
    virtual ~Application();

    virtual StatusResult  handleInput(std::istream &anInput);
    CmdProcessor* recognizes(Tokenizer &aTokenizer) override;
    Statement*    makeStatement(Tokenizer &aTokenizer) override;
    StatusResult  run(Statement *aStmt) override;
    
    friend class DbProcessor;
    friend class SQLProcessor;
  protected: 
      //todo: possibly public static db?
      Database* activeDB; //cb, DO NOT REMOVE this pointer. all other command processors use this db pointer
      //cb:would it make sense to make these pointers and hide all the other "#include (whatever)" in the .cpp
      DbProcessor Db_proc; 
      SQLProcessor SQL_proc; 
      QRY_Processor QRY_Proc;
  };

}

#endif /* Application_hpp */
