//
//
//
//

#include "DbProcessor.hpp"
#include "Config.hpp"
#include <filesystem>


namespace ECE141 {
	DbProcessor::DbProcessor(std::ostream& anOutput, Database** pdb_ptr):
		CmdProcessor(anOutput), toMakeStatement(Keywords::unknown_kw), activeDBptr_ptr(pdb_ptr){}


	bool isKnown_Db(Keywords aKeyword) { //Slightly altered isKnown
		static Keywords theKnown[] = { Keywords::create_kw, Keywords::drop_kw, Keywords::use_kw, Keywords::show_kw, Keywords::dump_kw };  //Add whatever keywords you want to recognize
		auto theIt = std::find(std::begin(theKnown), std::end(theKnown), aKeyword);
		return theIt != std::end(theKnown);
	}

// ============================================================================================
//================== executing commands ======================================================
//============================================================================================
	StatusResult DbProcessor::CREATE_run(Statement* aStmt) { //ADD in stuff here to run it
		std::string db_name = aStmt->aDb_command.db_name; //use this to get the name
		//std::cout << "RUNNING CREATE on " << db_name << std::endl;
		delete (*activeDBptr_ptr);
		(*activeDBptr_ptr)= new Database(db_name, CreateDB{});

		output << "Query OK, 1 row affected";
		StatusResult exitStatus{ Errors::noError}; //Assign appropriate error code if needed
		return exitStatus;
	}

	StatusResult DbProcessor::USE_run(Statement* aStmt) {
		//std::cout << "RUNNING CREATE" << std::endl;
		std::string db_name = aStmt->aDb_command.db_name; //use this to get the name
		Current_Database_name = db_name;
		delete (*activeDBptr_ptr);
		(*activeDBptr_ptr)= new Database(db_name, OpenDB{});

		output << "Database Changed";
		StatusResult exitStatus{ Errors::noError }; //Assign appropriate error code if needed
		return exitStatus;
	}

	StatusResult DbProcessor::DROP_run(Statement* aStmt) {
		//std::cout << "RUNNING drop" << std::endl;
		std::string db_name = aStmt->aDb_command.db_name; //use this to get the name

		//if there is an active database 
		if( *activeDBptr_ptr ){
			//and that databases name matches the one we are about to delete
			if( (*activeDBptr_ptr)->name == db_name){
				//clear the active database
				delete (*activeDBptr_ptr);
				(*activeDBptr_ptr)=nullptr;
			}
		}

		//if (std::filesystem::remove(Config::getDBPath(db_name))) {
		if (std::filesystem::remove(Config::getDBPath(db_name))) {

			output << "Query OK, 0 rows affected";
		}
		StatusResult exitStatus{ Errors::noError }; //Assign appropriate error code if needed
		return exitStatus;
	}

	StatusResult DbProcessor::SHOW_run(Statement* aStmt) {
		Config aCon;
		int numRow = 0;
		std::string path = aCon.getStoragePath(); //Changed this to work on the autograder
		//std::cout<< path;
		DB_viewer aViewMaker(output);
		aViewMaker.prettyPrint(path);
		StatusResult exitStatus{ Errors::noError }; //Assign appropriate error code if needed
		return exitStatus;
	}



	StatusResult DbProcessor:: DUMP_run(Statement* aStmt) {
		//std::cout << "RUNNING CREATE" << std::endl;
	
		std::string db_name = aStmt->aDb_command.db_name; //use this to get the name
		OpenDB OpenTag;
		Database aDatabase(db_name, OpenTag);
		DB_viewer aViewMaker(output);
		aViewMaker.prettyPrint(&aDatabase);
		
		StatusResult exitStatus{ Errors::noError }; //Assign appropriate error code if needed""
		return exitStatus;
	}


	//===============================================================================================
	//============================= END OF COMMANDS =================================================

	CmdProcessor* DbProcessor::recognizes(Tokenizer& aTokenizer) { //TODO parse statements, this is an incorrect way of doing it
		//std::cout << "DB PARSING...";
		if (!isKnown_Db(aTokenizer.current().keyword)) { //If entire statement is understood
		//	std::cout << "FAILED" << std::endl;
			return nullptr;
		}		
		Token CREATE_kw_Token{ 	TokenType::keyword, Keywords::create_kw, 	Operators::unknown_op, "CREATE" };
		Token DROP_kw_Token{ 	TokenType::keyword, Keywords::drop_kw, 		Operators::unknown_op, "DROP" };
		Token SHOW_kw_Token{	TokenType::keyword, Keywords::show_kw, 		Operators::unknown_op, "SHOW" };
		Token USE_kw_Token{ 	TokenType::keyword, Keywords::use_kw, 		Operators::unknown_op, "USE" };
		Token DUMP_kw_Token{ 	TokenType::keyword, Keywords::dump_kw, 		Operators::unknown_op, "DUMP" };
		Token DATABASE_kw_Token{TokenType::keyword, Keywords::database_kw, 	Operators::unknown_op, "DATABASE" };
		Token DATABASES_kw_Token{ TokenType::keyword, Keywords::databases_kw, Operators::unknown_op, "DATABASE" };

		Token identifier_Token{ TokenType::identifier, Keywords::unknown_kw, Operators::unknown_op, "UNKNOWN" };
		Token punc_Token{ TokenType::punctuation, Keywords::unknown_kw, Operators::unknown_op, ";" };



		std::vector<Token> Create1{ CREATE_kw_Token, DATABASE_kw_Token, identifier_Token, semicolon_Token }; //create databases MING;
		if (tokens_match(Create1, aTokenizer)) {
			toMakeStatement = Keywords::create_kw;
			return this;
		}

		std::vector<Token> Drop1{ DROP_kw_Token, DATABASE_kw_Token, identifier_Token, semicolon_Token }; //drop databases MING;
		if (tokens_match(Drop1, aTokenizer)) {
			toMakeStatement = Keywords::drop_kw;
			return this;
		}

		std::vector<Token> Show1{ SHOW_kw_Token, DATABASES_kw_Token, semicolon_Token }; //show databases; //database is dif from databases
		if (tokens_match(Show1, aTokenizer)) {
			toMakeStatement = Keywords::show_kw;
			return this;
		}

		std::vector<Token> Use1{ USE_kw_Token, identifier_Token, semicolon_Token }; //use MING;
		if (tokens_match(Use1, aTokenizer)) {
			toMakeStatement = Keywords::use_kw;
			return this;
		}
		
		std::vector<Token> Dump1{ DUMP_kw_Token, DATABASE_kw_Token, identifier_Token, semicolon_Token }; //dump database ming;
		std::vector<Token> Dump2{ DUMP_kw_Token, DATABASES_kw_Token, identifier_Token, semicolon_Token }; //dump databases;
		if (tokens_match(Dump1, aTokenizer) || tokens_match(Dump2, aTokenizer)) {
			toMakeStatement = Keywords::dump_kw;
			return this;
		}
		return nullptr; //Not recognized
	}



	Statement* DbProcessor::makeStatement(Tokenizer& aTokenizer) { 
		//std::cout << "\nDb Processor MAKE statement" << std::endl;
		Statement* theStatement = new Statement(toMakeStatement); //Tells the statement should be
		theStatement->parse_DB(aTokenizer); //Parse
		toMakeStatement = Keywords::unknown_kw; //resetting
		return theStatement;
	}

	StatusResult DbProcessor::run(Statement* aStmt) { //Run Statements
		switch (aStmt->getType()) {
		case Keywords::create_kw:	return CREATE_run(aStmt);
		case Keywords::drop_kw:		return DROP_run(aStmt);
		case Keywords::show_kw:		return SHOW_run(aStmt);
		case Keywords::use_kw:   	return USE_run(aStmt);
		case Keywords::dump_kw:  	return DUMP_run(aStmt);
		}
		return StatusResult(Errors::noError);
	}
}