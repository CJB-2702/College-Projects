//
//  Statement.cpp
//  Database
//
//  Created by rick gessner on 3/20/19.
//  Copyright © 2019 rick gessner. All rights reserved.
//

#include "Statement.hpp"
#include "Tokenizer.hpp"
#include "Attribute.hpp"


namespace ECE141 {
	Statement::Statement(Keywords aStatementType, Keywords aLevelType) : stmtType(aStatementType), levelType(aLevelType){}
	Statement::Statement(const Statement& aCopy) : stmtType(aCopy.stmtType) {}

	Statement::~Statement() {}

	StatusResult Statement::parse_SQL(Tokenizer& aTokenizer) {
		switch (stmtType) { //this was the kw we used earlier
		case Keywords::create_kw:
			create_Table_Statement(aTokenizer);
			break;
		case Keywords::show_kw:
			show_Table_Statement(aTokenizer);
			break;
		case Keywords::drop_kw:
			drop_Table_Statement(aTokenizer);
			break;
		case Keywords::describe_kw:
			describe_Table_Statement(aTokenizer);
			break;
		case Keywords::insert_kw:
			insert_into_Statement(aTokenizer);
		}
		return StatusResult{ Errors::noError };
	}


	//USE: ------------------- THESE ARE TO PARSE given a keyword
	//Tables
	bool Statement::create_Table_Statement(Tokenizer& aTokenizer) {
		size_t tokensStart = aTokenizer.index_cur();
	  	aTable_Command.tb_name = aTokenizer.tokenAt(tokensStart + 2).data;

		aTokenizer.skipToChar("(");
		//aTable_command.AttributeVector.push
		bool notFlag=false;
		Attribute aAttribute;
		while (aTokenizer.current().data != ";") {

			while 	( 	aTokenizer.current().data != "," && 
						aTokenizer.current().data != ")" &&
						aTokenizer.remaining()){

				//std::cout << aTokenizer.current().data << std::endl;
				
				if (aTokenizer.current().type == identifier_Token.type) {
					aAttribute.id = aTokenizer.current().data;
				}
				switch(aTokenizer.current().keyword){
					case Keywords::integer_kw:
						aAttribute.type = DataTypes::int_type;
						break;

					case Keywords::boolean_kw:
						aAttribute.type = DataTypes::bool_type;
						break;

					case Keywords::float_kw:
						aAttribute.type = DataTypes::float_type;
						break;

					case Keywords::current_timestamp_kw:
						aAttribute.type = DataTypes::datetime_type;
						break;
					
					case Keywords::varchar_kw:
						aAttribute.type = DataTypes::varchar_type;
						aTokenizer.next();
						aTokenizer.next();
						aAttribute.field_length = stoi(aTokenizer.current().data);
						aTokenizer.next();
						break;

					case Keywords::primary_kw:
						aAttribute.key = true;
						break;

					case Keywords::not_kw:
						notFlag = true;
						break;

					case Keywords::null_kw:
						aAttribute.nullable = true;
						if (notFlag) { aAttribute.nullable = false;}
						notFlag = false;
						break;

					case Keywords::auto_increment_kw:
						aAttribute.auto_increment = true;
						if (notFlag) aAttribute.auto_increment = false;
						notFlag = false;
						break;
				}

				// ITERATE THE STATMENT
				aTokenizer.next();
				if (!aTokenizer.remaining()) {break;}

			} //first while
			aTable_Command.AttributeVector.push_back(aAttribute);
			notFlag = false;
			aAttribute.clear();
			aTokenizer.next();
		}//second while
		aTokenizer.skipToSemi();
		auto a = aTable_Command.AttributeVector[0];
		//std::cout << a.id << a.field_length << a.auto_increment << a.key << a.nullable << std::endl;
		return true;
	}


	bool Statement::insert_into_Statement(Tokenizer& aTokenizer) {
		//First check that the table exists
		aTokenizer.skipTo(TokenType::identifier); //Skip to the name of the table
		aInsert_Command.tb_name = aTokenizer.current().data; //Puts the name of the file
		//aTokenizer.next(); //Now into the attribute portion of it
		
		while (aTokenizer.current().data != ")") { //This part gets the names of attributes to check later
			aTokenizer.next();
			//std::cout << "CURRENT " << aTokenizer.current().data << std::endl;
			if (aTokenizer.current().type == TokenType::identifier) {
				//std::cout << fields;
				aInsert_Command.fields.push_back(aTokenizer.current().data); //Pushes the field name back	
			}
		}

		aTokenizer.next();
		aTokenizer.next(); //Now you are at the ( of the new 
		while (aTokenizer.current().data != ";") {
		//	std::cout << "CURRENT " << aTokenizer.current().data << std::endl;
			if (aTokenizer.current().data != "(") {
				aTokenizer.next();
				continue;
			}
			std::vector<Cell> Values = extractValuesfromParen(aTokenizer);
			aInsert_Command.theValues.push_back(Values);
		}
		aTokenizer.skipToSemi();
		return true;
	}

	std::vector<Cell> Statement::extractValuesfromParen(Tokenizer& aTokenizer){ //Starts at (, ends at )
		std::vector<Cell> Values;
		if (aTokenizer.current().data != "(") {
			std::cout << "ERROR IN EXTRACT VALUES FROM PAREN" << std::endl;
		}
		while (aTokenizer.peek().data != ";") {
			aTokenizer.next();
			if (aTokenizer.current().data == ")") { break;}
			
			if(aTokenizer.current().type != TokenType::punctuation) {
				Cell singleValue;
				singleValue = aTokenizer.current().data; //EVERYTHING IS PUT IN AS A STRING FIRST, THEN WE USE ATTRIBUTES TO VALIDATE
				Values.push_back(singleValue);
			}
		}
		return Values;
	}

	
  bool Statement::show_Table_Statement(Tokenizer& aTokenizer) {
	  aTokenizer.skipToSemi(); //add this to end
	  return false;
  }

  bool Statement::drop_Table_Statement(Tokenizer& aTokenizer) {
	  size_t punc = aTokenizer.index_cur();
	  aTable_Command.tb_name = aTokenizer.tokenAt(punc + 2).data;
	  aTokenizer.skipToSemi(); //add this to end
	  return false;
  }

  bool Statement::describe_Table_Statement(Tokenizer& aTokenizer) {
	  size_t punc = aTokenizer.index_cur();
	  aTable_Command.tb_name = aTokenizer.tokenAt(punc + 1).data;
	  aTokenizer.skipToSemi(); //add this to end
	  return false;
  }



  //Database
  bool Statement::create_DB_Statement(Tokenizer& aTokenizer) { //create database name; drop database name;
	  //std::cout << "--CREATE recognized" << std::endl;
	  size_t punc = aTokenizer.index_cur();
	  aDb_command.db_name = aTokenizer.tokenAt(punc + 2).data;
	  aTokenizer.skipToSemi();

	  return true;
  }

  bool Statement::drop_DB_Statement(Tokenizer& aTokenizer) {
	  //std::cout << "--DROP recognized" << std::endl;
	  size_t punc = aTokenizer.index_cur();
	  aDb_command.db_name = aTokenizer.tokenAt(punc + 2).data;
	  aTokenizer.skipToSemi();

	  return true;
  }

  bool Statement::show_DB_Statement(Tokenizer& aTokenizer) {
	 // std::cout << "--SHOW recognized" << std::endl;
	  size_t punc = aTokenizer.index_cur();
	  aTokenizer.skipToSemi();

	  return true;
  }

  bool Statement::use_DB_Statement(Tokenizer& aTokenizer) {
	 // std::cout << "--USE recognized" << std::endl;
	  size_t punc = aTokenizer.index_cur();
	  aDb_command.db_name = aTokenizer.tokenAt(punc + 1).data;
	  aTokenizer.skipToSemi();

	  return true;
  }

  bool Statement::dump_DB_Statement(Tokenizer& aTokenizer) {
	//  std::cout << "--DUMP recognized" << std::endl;
	  size_t punc = aTokenizer.index_cur();
	  aDb_command.db_name = aTokenizer.tokenAt(punc + 2).data;
	  aTokenizer.skipToSemi();

	  return true;
  }



  
  StatusResult Statement::parse_DB(Tokenizer &aTokenizer) {
	switch (stmtType) { //this was the kw we used earlier
		case Keywords::create_kw:
			create_DB_Statement(aTokenizer);
			break;
		case Keywords::drop_kw:
			drop_DB_Statement(aTokenizer);
			break;
		case Keywords::show_kw:
			show_DB_Statement(aTokenizer);
			break;
		case Keywords::use_kw:
			use_DB_Statement(aTokenizer);
			break;
		case Keywords::dump_kw:
			dump_DB_Statement(aTokenizer);
			break;
	}
    return StatusResult{Errors::noError};
  }


  
}
