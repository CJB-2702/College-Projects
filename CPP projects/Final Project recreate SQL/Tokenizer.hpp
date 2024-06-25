//
//  Tokenizer.hpp
//  Database
//
//  Created by rick gessner on 3/19/19.
//  Copyright © 2019 rick gessner. All rights reserved.
//

#ifndef Tokenizer_hpp
#define Tokenizer_hpp

#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include "keywords.hpp"
#include "Parser.hpp"

namespace ECE141 {

  using parseCallback = bool(char aChar);
  
  enum class TokenType {
    function, identifier, keyword, number, operators, timedate, punctuation, string, unknown
  };
  
  //-----------------
  
  struct Token {
    
    Token& operator=(const Token &aCopy) {
      type=aCopy.type;
      keyword=aCopy.keyword;
      data=aCopy.data;
      return *this;
    }
    
    TokenType   type;
    Keywords    keyword;
    Operators   op;
    std::string data;
  };
 
  //-----------------
  
  class Tokenizer : public Parser {
  public:
    Tokenizer(std::istream &anInputStream);
    
    StatusResult  tokenize();
    Token&        tokenAt(size_t anOffset);

    Token&        current();
    bool          more() {return index<size();}
    bool          next(int anOffset=1);
    Token&        peek(int anOffset=1);
    void          restart() {index=0;}
    size_t        size() {return tokens.size();}
    size_t        remaining() {return index<size() ? size()-index :0;}
    size_t        index_cur();
    
    bool          skipToSemi(); //This will move tokenizer to the semicolon
    bool          skipToChar(std::string aChar);

                          //these might consume a token...    
    bool          skipTo(Keywords aKeyword);
    bool          skipTo(TokenType aTokenType);

    bool          skipIf(Keywords aKeyword);
    bool          skipIf(Operators anOperator);
    bool          skipIf(TokenType aTokenType);
    bool          skipIf(char aChar);

    void          dump(); //utility

  protected:

    std::vector<Token>    tokens;    
    size_t                index;


  };
  

  Token const CREATE_kw_Token{ TokenType::keyword, Keywords::create_kw, Operators::unknown_op, "CREATE" };
  Token const DROP_kw_Token{ TokenType::keyword, Keywords::drop_kw, Operators::unknown_op, "DROP" };
  Token const SHOW_kw_Token{ TokenType::keyword, Keywords::show_kw, Operators::unknown_op, "SHOW" };
  Token const USE_kw_Token{ TokenType::keyword, Keywords::use_kw, Operators::unknown_op, "USE" };
  Token const DUMP_kw_Token{ TokenType::keyword, Keywords::dump_kw, Operators::unknown_op, "DUMP" };
  Token const DATABASE_kw_Token{ TokenType::keyword, Keywords::database_kw, Operators::unknown_op, "DATABASE" };
  Token const DATABASES_kw_Token{ TokenType::keyword, Keywords::databases_kw, Operators::unknown_op, "DATABASE" };

  //SQL SPECIFIC
  Token const TABLE_kw_Token{ TokenType::keyword, Keywords::table_kw, Operators::unknown_op, "TABLE" };
  Token const TABLES_kw_Token{ TokenType::keyword, Keywords::tables_kw, Operators::unknown_op, "TABLE" };
  Token const INT_kw_Token{ TokenType::keyword, Keywords::integer_kw, Operators::unknown_op, "INT" };
  Token const AUTO_INCREMENT_kw_Token{ TokenType::keyword, Keywords::auto_increment_kw, Operators::unknown_op, "AUTO_INCREMENT" };
  Token const PRIMARY_kw_Token{ TokenType::keyword, Keywords::primary_kw, Operators::unknown_op, "PRIMARY" };
  Token const KEY_kw_Token{ TokenType::keyword, Keywords::key_kw, Operators::unknown_op, "KEY" };
  Token const VARCHAR_kw_Token{ TokenType::keyword, Keywords::varchar_kw, Operators::unknown_op, "VARCHAR" };
  Token const NOT_kw_Token{ TokenType::keyword, Keywords::not_kw, Operators::unknown_op, "NOT" };
  Token const NULL_kw_Token{ TokenType::keyword, Keywords::null_kw, Operators::unknown_op, "NULL" };
  Token const FLOAT_kw_Token{ TokenType::keyword, Keywords::float_kw, Operators::unknown_op, "FLOAT" };
  Token const BOOLEAN_kw_Token{ TokenType::keyword, Keywords::boolean_kw, Operators::unknown_op, "BOOLEAN" };
  Token const DESCRIBE_kw_Token{ TokenType::keyword, Keywords::describe_kw, Operators::unknown_op, "DESCRIBE" };
  Token const TIMESTAMP_kw_Token{ TokenType::keyword, Keywords::current_timestamp_kw, Operators::unknown_op, "TIMESTAMP" };
  Token const INSERT_kw_Token{ TokenType::keyword, Keywords::insert_kw, Operators::unknown_op, "INSERT" };
  Token const INTO_kw_Token{ TokenType::keyword, Keywords::into_kw, Operators::unknown_op, "INTO" };
  Token const VALUES_kw_Token{ TokenType::keyword, Keywords::values_kw, Operators::unknown_op, "VALUES" };

  //QRY SPECIFIC
  Token const SELECT_kw_Token{ TokenType::keyword, Keywords::select_kw, Operators::unknown_op, "SELECT" };
  Token const FROM_kw_Token{ TokenType::keyword, Keywords::from_kw, Operators::unknown_op, "from" };




  //IDENTIFIER SPECIFIC
  Token const number_Token{ TokenType::number, Keywords::unknown_kw, Operators::unknown_op, "UNKNOWN" };
  Token const identifier_Token{ TokenType::identifier, Keywords::unknown_kw, Operators::unknown_op, "UNKNOWN" };
  Token const semicolon_Token{ TokenType::punctuation, Keywords::unknown_kw, Operators::unknown_op, ";" };

  Token const openBrack_Token{ TokenType::punctuation, Keywords::unknown_kw, Operators::unknown_op, "{" };
  Token const closedBrack_Token{ TokenType::punctuation, Keywords::unknown_kw, Operators::unknown_op, "}" };
  Token const openParen_Token{ TokenType::punctuation, Keywords::unknown_kw, Operators::unknown_op, "(" };
  Token const closedParen_Token{ TokenType::punctuation, Keywords::unknown_kw, Operators::unknown_op, ")" };

  Token const punc_Token{ TokenType::punctuation, Keywords::unknown_kw, Operators::unknown_op, "PUNC" };





}

#endif /* Tokenizer_hpp */

