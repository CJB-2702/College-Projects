//
//  Db Processor


#ifndef DbProcessor_hpp
#define DbProcessor_hpp

#include <stdio.h>

#include "CmdProcessor.hpp"




namespace ECE141 {

  class DbProcessor : public CmdProcessor {
  public:
    
    DbProcessor(std::ostream& anOutput, Database** pdb_ptr); 
    ~DbProcessor(){};
    
    //virtual StatusResult  handleInput(std::istream& anInput);
    virtual CmdProcessor* recognizes(Tokenizer& aTokenizer);
    virtual Statement* makeStatement(Tokenizer& aTokenizer);
    virtual StatusResult  run(Statement *aStmt);
    
    StatusResult CREATE_run(Statement* aStmt);
    StatusResult DROP_run(Statement* aStmt);
    StatusResult SHOW_run(Statement* aStmt);
    StatusResult USE_run(Statement* aStmt);
    StatusResult DUMP_run(Statement* aStmt);

  protected:
      Keywords toMakeStatement;
      Database** activeDBptr_ptr;
  };

}
#endif /* DBProcessor_hpp */
