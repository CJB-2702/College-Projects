//
//  SQLProcessor.cpp
//  RGAssignment3
//
//  Created by rick gessner on 4/1/21.
//

#include "SQLProcessor.hpp"
#include "Application.hpp"
#include "Database.hpp"
#include "Helpers.hpp"
#include "TabularView.hpp"


namespace ECE141 {


	//STUDENT: Fully implement your SQLProcessor...

	SQLProcessor::SQLProcessor(std::ostream& anOutput, Database** pdb_ptr) : 
		CmdProcessor(anOutput),
		toMakeStatement(Keywords::unknown_kw),
		activeDBptr_ptr(pdb_ptr) 
		{}

	SQLProcessor::~SQLProcessor() {}


	CmdProcessor* SQLProcessor::recognizes(Tokenizer& aTokenizer) {
		
		// //switch to function map and keyword
		std::vector<Token> Create1{ CREATE_kw_Token, TABLE_kw_Token, identifier_Token, openParen_Token, closedParen_Token,  semicolon_Token }; //CREATE TABLE ming (id); //is the minimum
		if (tokens_exist_ordered(Create1, aTokenizer)) {
			toMakeStatement = Keywords::create_kw;
			return this;
		}

		std::vector<Token> Show1{ SHOW_kw_Token, TABLES_kw_Token, semicolon_Token }; //show tables;
		if (tokens_match(Show1, aTokenizer)) {
			toMakeStatement = Keywords::show_kw;
			return this;
		}

		std::vector<Token> Drop1{ DROP_kw_Token, TABLE_kw_Token, identifier_Token, semicolon_Token }; //drop table MING;
		if (tokens_match(Drop1, aTokenizer)) {
			toMakeStatement = Keywords::drop_kw;
			return this;
		}

		std::vector<Token> Describe1{ DESCRIBE_kw_Token, identifier_Token, semicolon_Token }; //create databases MING;
		if (tokens_match(Describe1, aTokenizer)) {
			toMakeStatement = Keywords::describe_kw;
			return this;
		}

		std::vector<Token> Insert1{ INSERT_kw_Token, INTO_kw_Token, identifier_Token, openParen_Token, closedParen_Token, VALUES_kw_Token, openParen_Token, closedParen_Token, semicolon_Token }; //insert into TABLENAME () VALUES ();
		if (tokens_exist_ordered(Insert1, aTokenizer)) {
			toMakeStatement = Keywords::insert_kw;
			return this;
		}

		return nullptr;
	}

	Statement* SQLProcessor::makeStatement(Tokenizer& aTokenizer){
		//std::cout << "\nSQL Processor MAKE statement" << std::endl;
		Statement* theStatement = new Statement(toMakeStatement); //Tells the statement should be
		theStatement->parse_SQL(aTokenizer); //Parse
		toMakeStatement = Keywords::unknown_kw; //resetting
		return theStatement;
	}

	StatusResult SQLProcessor::run(Statement* aStmt){
		// make sure a database is actually open first
		if( (*activeDBptr_ptr)== nullptr){
			std::cout << "\nINVALID COMMAND Database not activated.\n" << std::endl;
			return StatusResult(Errors::invalidArguments);
		}

		switch (aStmt->getType()) {
		case Keywords::create_kw:	return CREATE_run(aStmt);
		case Keywords::drop_kw:		return DROP_run(aStmt);
		case Keywords::show_kw:		return SHOW_run(aStmt);
		case Keywords::describe_kw:   	return DESCRIBE_run(aStmt);
		case Keywords::insert_kw:   	return INSERT_run(aStmt);
		}
		return StatusResult(Errors::invalidArguments); //Return an error if there is no database currently in use
	}

	//=====================================================================================
	//===================== CMDS ==========================================================
	//=====================================================================================
	//===================== CMDS ==========================================================
	//===================== CMDS ==========================================================
	//===================== CMDS ==========================================================

	StatusResult SQLProcessor::CREATE_run(Statement* aStmt) {
		aEntity.clear();
		//extract key entity data from statement
		for (int i = 0; i < aStmt->aTable_Command.AttributeVector.size(); i++) {
			aEntity.insert_attribute(aStmt->aTable_Command.AttributeVector[i]);
		}
		aEntity.tablename=aStmt->aTable_Command.tb_name;
		//Load the block into the database temporarily
		(*activeDBptr_ptr)->storage.save(aEntity.blockify());
		//return status;
		output << "Query OK, 0 rows affected";
		StatusResult exitStatus{ Errors::noError }; //Assign appropriate error code if needed""
		return exitStatus;
	}

	StatusResult SQLProcessor::DROP_run(Statement* aStmt) {
		Database* pdb=(*activeDBptr_ptr);
		std::string table_to_drop = aStmt->aTable_Command.tb_name;
		pdb->storage.releaseBlocks(table_to_drop);
		output << "Query OK, 3 rows affected";
		StatusResult exitStatus{ Errors::noError }; //Assign appropriate error code if needed""
		return exitStatus;
	}

	StatusResult SQLProcessor::SHOW_run(Statement* aStmt) {// todo move to view class
		Database* pdb=(*activeDBptr_ptr);
		TB_viewer aView(output,nullptr);
		aView.prettyPrint(pdb);
		StatusResult exitStatus{ Errors::noError }; //Assign appropriate error code if needed""
		return exitStatus;
	}


	StatusResult SQLProcessor::DESCRIBE_run(Statement* aStmt) {// move to view class
		std::string table_to_describe = aStmt->aTable_Command.tb_name;
		//scan all blocks for entity markers
		Database* pDB=*activeDBptr_ptr;
		std::vector<Block> EntityBlocks=pDB->storage.load(table_to_describe);
		aEntity.deblockify(EntityBlocks);
		TB_viewer getViews(output,&aEntity);
		getViews.prettyPrint(aEntity.attributes);
		StatusResult exitStatus{ Errors::noError }; //Assign appropriate error code if needed""
		return exitStatus;
	}


	std::vector<int> id_index_returns_col_index_psudoMap(StringList id_s ,std::vector<Attribute> cols);


	StatusResult SQLProcessor::INSERT_run(Statement* aStmt){ 
		std::string table_to_insert = aStmt->aInsert_Command.tb_name;
		StringList fields = aStmt->aInsert_Command.fields;
		Database* pDB = *activeDBptr_ptr;

		load_table_by_title(table_to_insert);
		//std::cout<<"\n";
		//for(auto it: fields){std::cout<<it<<" ";}
		std::vector<int> col_indexes=id_index_returns_col_index_psudoMap(fields,aEntity.attributes); //This remaps the correct locations 
		if (col_indexes.size()!=fields.size()){  //To double check that every field was mapped to something
			StatusResult exitStatus{ Errors::invalidAttribute};
			return exitStatus;
		}

		// Add Rows to entity
		Block aBlock;
		int rowsAdded=0;
		int totalRows=aEntity.theRowCollection.size() + 1 + aStmt->aInsert_Command.theValues.size();
		Row aRow;
		aRow.cells.resize(aEntity.attributes.size()); //preallocate space for mapping, otherwise you wouldn'tbe able to push there 
		
		

		for (std::vector<Cell> Values : aStmt->aInsert_Command.theValues) { //This is done here because they needed to be mapped 
			//This is where we actually put it in
			for(int i=0;i<fields.size();i++){
				int anIndex=col_indexes[i];
				if(anIndex>-1){
					aRow.cells[anIndex]=Values[i];
					continue;
				}
			}
			aEntity.push_back(aRow);

			//save row to database--------------------------------------------------------------
			//temporary choice to just add to the db rather than clearing and saving whole thing
			//grab last row from rows and blockify... possibly make a function in ENTITY
			//row is reqiured to push_back and be retrieved from Entity due to validation reasons.
			aBlock=(aEntity.theRowCollection[aEntity.theRowCollection.size()-1]).Blockify(table_to_insert, totalRows, aEntity.theRowCollection.size());
			pDB->insert_block_next_free(aBlock);// todo add refrence to next block for chaining
			rowsAdded++;
		}
		//update the header block and attributes.
		int headlocation=pDB->get_TB_head_index(table_to_insert);
		Block aMeta=aEntity.getHeaderBlock();
		pDB->storage.writeBlock(headlocation,aMeta);
		
		output << "Query OK, " << rowsAdded << " rows affected";
		StatusResult exitStatus{ Errors::noError }; //Assign appropriate error code if needed""
		return exitStatus;
	}	

		
	//===================== MISC FUNCTIONS ================================================
	//=====================================================================================
	//=====================================================================================
	//=====================================================================================
	//=====================================================================================

	std::vector<int> id_index_returns_col_index_psudoMap(StringList id_s ,std::vector<Attribute> cols){
		int i=0,j=0, N_verified=0;
		//fill a vector that maps all 
		std::vector<int> goto_col(id_s.size(),-1);
		for (auto Given_attribute_Name: id_s) {
			//std::cout<<"\n"<<Given_attribute_Name<<"\n";
			for (auto Existing_attribute :cols) {
				//std::cout<<Existing_attribute.id;
				if (Given_attribute_Name == Existing_attribute.id) { 
					goto_col[i]=j;
					N_verified++;
				}
				j++;
			}
			i++;
			j=0;
		}
		return goto_col;
	}

	std::string getNameFromPayload(Block aBlock){//TODO DELETE FUNCTION, OUTDATED
		std::stringstream ss;
		ss.write(aBlock.payload,kPayloadSize);
		std::string result;
		ss>>result;
		return result;
	}
	
	int SQLProcessor::getTableMetaBlockIndex(std::string targetName) {
		Database* pdb=(*activeDBptr_ptr);
		Entity itEnt;
		for (int i = 0; i < pdb->storage.getBlockCount(); i++) {
			Block aBlock;
			pdb->storage.readBlock(i, aBlock);
			if (aBlock.header.type == 'E') {
				itEnt.loadFromHeaderBlock(aBlock);
				if (itEnt.tablename == targetName) {
					return i;
				}
			}
		}
		return -1;
	}

	Entity& SQLProcessor::load_table_by_title(std::string theName){
		Database* pDB=*activeDBptr_ptr;
		aEntity.clear();
		aEntity.deblockify( pDB->storage.load(theName));
		return aEntity;
	}


}