

#ifndef QRY_Processor_hpp
#define QRY_Processor_hpp

#include <stdio.h>
#include "SQLProcessor.hpp"
#include "Tokenizer.hpp"
#include "Entity.hpp"
#include "QRY_Statement.hpp"

namespace ECE141 {


  using boolSelection= std::vector<bool>;
  class QRY_Processor : public SQLProcessor {
  public:
     //todo, make insert function part of this instead of sql
    //no, because a query is asking something from the table, so 

    QRY_Processor(std::ostream& anOutput, Database** ptr_ptr_db);
    ~QRY_Processor(){};
    //STUDENT: Declare OCF methods...
       
    CmdProcessor* recognizes(Tokenizer &aTokenizer) override;
    Statement*    makeStatement(Tokenizer &aTokenizer)override;
    StatusResult  run(Statement *aStmt) override;

    private:
    //STUDENT add any other methods your processor needs...
    bool Show_from_table_select_columns(Query &aQuery);
    bool select_from_w_constraints(Query &aQuery);
    boolSelection select_columns(const std::vector<std::string>& columnTitles);
    boolSelection constrain_rows(std::vector<RowConstraint> theRowConstraints, Entity* pTB);
    boolSelection selection_union(std::vector<bool>,std::vector<bool>);
    boolSelection selection_intersection(std::vector<bool>,std::vector<bool>);
    int get_column_num_from_attribute_name(std::string);
  };

}

#endif /* SQLProcessor_hpp */
