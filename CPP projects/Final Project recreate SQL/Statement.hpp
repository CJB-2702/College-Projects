//
//  Statement.hpp
//  Database
//
//  Created by rick gessner on 3/20/19.
//  Copyright © 2019 rick gessner. All rights reserved.
//

#ifndef Statement_hpp
#define Statement_hpp

#include <iostream>
#include <string>
#include "keywords.hpp"

#include "Attribute.hpp"

#include "Row.hpp" //Might break it
namespace ECE141 {
  
    struct Db_commands {
        std::string db_name = "EMPTY db name ERROR PLEASE CALL SUPPORT";
    };

    struct Table_commands {
        std::string tb_name;
        std::vector<ECE141::Attribute> AttributeVector;
    };

    struct Insert_commands {
        bool        commandIsGood;
        //change the below members into a table instead?
        std::string tb_name;
        StringList fields;
        std::vector<std::vector<ECE141::Cell>> theValues; 
    };

  class Tokenizer;
  class CmdProcessor;
  
  class Statement {
  public:
    //base functions that belong in all statements
    Statement(Keywords aStatementType=Keywords::unknown_kw,Keywords aLevelType=Keywords::unknown_kw);
    Statement(const Statement &aCopy);
    virtual               ~Statement();
    virtual std::string   getClassName(){return "Statement";};
    virtual StatusResult  parse(Tokenizer &aTokenizer){return StatusResult{};};
    virtual StatusResult  dispatch() {return StatusResult{};}
    virtual bool clear(){return true;};

  
    //change name to regular parse
    StatusResult  parse_DB(Tokenizer &aTokenizer);
    StatusResult  parse_SQL(Tokenizer& aTokenizer);

    
    //ideally no more structs
    Db_commands aDb_command;
    Table_commands aTable_Command;
    Insert_commands aInsert_Command;
    Keywords              getType() const {return stmtType;}

  private:
    //protected DB functions
    bool create_DB_Statement(Tokenizer& aTokenizer);
    bool drop_DB_Statement(Tokenizer& aTokenizer);
    bool show_DB_Statement(Tokenizer& aTokenizer);
    bool use_DB_Statement(Tokenizer& aTokenizer);
    bool dump_DB_Statement(Tokenizer& aTokenizer);
    //protected SQL functions
    bool create_Table_Statement(Tokenizer& aTokenizer);
    bool show_Table_Statement(Tokenizer& aTokenizer);
    bool drop_Table_Statement(Tokenizer& aTokenizer);
    bool describe_Table_Statement(Tokenizer& aTokenizer);
    bool insert_into_Statement(Tokenizer& aTokenizer);
    
    Keywords   stmtType,levelType;//eliminate?
    std::vector<Cell> extractValuesfromParen(Tokenizer& aTokenizer);//move inside struct?
  };

  /* // COMING SOON
  class DB_Statement: public Statement{

  };
  class SQl_Statement: public Statement{

  };
  class QRY_Statement:public SQl_Statement{

  }*/
  
}

#endif /* Statement_hpp */
