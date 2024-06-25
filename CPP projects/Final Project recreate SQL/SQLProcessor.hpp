//
//  SQLProcessor.hpp
//  RGAssignment3
//
//  Created by rick gessner on 4/1/21.
//

#ifndef SQLProcessor_hpp
#define SQLProcessor_hpp

#include <stdio.h>
#include "CmdProcessor.hpp"
#include "Tokenizer.hpp"
#include "Entity.hpp"
#include "TB_Viewer.hpp"
#include "DB_Viewer.hpp"

namespace ECE141 {



  class SQLProcessor : public CmdProcessor {
  public:
    
      SQLProcessor(std::ostream& anOutput, Database** ptr_ptr_db);
      ~SQLProcessor();
    //STUDENT: Declare OCF methods...
        
    CmdProcessor* recognizes(Tokenizer &aTokenizer) override;
    Statement*    makeStatement(Tokenizer &aTokenizer) override;
    StatusResult  run(Statement *aStmt) override;
    

    protected:
    Database*     get_pdb(){return *activeDBptr_ptr;};
    Entity aEntity;
    Database** activeDBptr_ptr;
    Keywords toMakeStatement;

    int getTableMetaBlockIndex(std::string targetName);
    Entity& load_table_by_title(std::string);

    private:
    //STUDENT add any other methods your processor needs...
    StatusResult CREATE_run(Statement* aStmt);
    StatusResult DROP_run(Statement* aStmt);
    StatusResult SHOW_run(Statement* aStmt);
    StatusResult DESCRIBE_run(Statement* aStmt);
    StatusResult INSERT_run(Statement* aStmt);
  };

}

#endif /* SQLProcessor_hpp */
