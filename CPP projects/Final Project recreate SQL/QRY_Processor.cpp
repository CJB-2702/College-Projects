#include "QRY_Processor.hpp"
#include <variant>



namespace ECE141{
    QRY_Processor::QRY_Processor(std::ostream& anOutput,Database** ptr_ptr_db):SQLProcessor(anOutput,ptr_ptr_db){}

    CmdProcessor* QRY_Processor::recognizes(Tokenizer &aTokenizer){
        std::vector<Token> Select_w_constraints{ SELECT_kw_Token, FROM_kw_Token, identifier_Token,FROM_kw_Token,semicolon_Token };
        if (tokens_exist_ordered(Select_w_constraints, aTokenizer)) {
            toMakeStatement = Keywords::select_w_constraints;
            return this;
        }
        
        std::vector<Token> Select1{ SELECT_kw_Token, FROM_kw_Token, identifier_Token, semicolon_Token }; //insert into TABLENAME () VALUES ();
        if (tokens_exist_ordered(Select1, aTokenizer)) {
            //std::cout << "INSERT, must implement";
            toMakeStatement = Keywords::select_kw;
            return this;
        }
        return nullptr;
    }

    Statement*    QRY_Processor::makeStatement(Tokenizer &aTokenizer){
        Statement* pST=new QRY_Statement;
        pST->parse(aTokenizer);
        return pST;
    }

    StatusResult  QRY_Processor::run(Statement *aStmt){
        if ((*activeDBptr_ptr) == nullptr) {
            std::cout << "\nINVALID COMMAND Database not activated.\n" << std::endl;
            return StatusResult(Errors::invalidArguments);
        }

        if(aStmt->getClassName()=="QRY_Statement"){ //This is to check if we can dynamic cast back into a qry statement
            QRY_Statement* pQRY= dynamic_cast<QRY_Statement*>(aStmt);
            //if the statment has constraints execute with constraints
            if(pQRY->aQuery.theRowConstraints.size()>0){
                select_from_w_constraints(pQRY->aQuery);
            }else{
                Show_from_table_select_columns(pQRY->aQuery);
            }
            
            return StatusResult{Errors::noError};
        }
        return StatusResult{Errors::statementExpected} ;       
    }

    //accepts table name and vector of column titles or nothing, nothing is assumed to be all;
    

    bool QRY_Processor::Show_from_table_select_columns(Query &aQuery){
        auto tableName = aQuery.getTableName();
        auto columnTitles = aQuery.getAttributes();
        auto limit = aQuery.getlimit();
        auto orderBy = aQuery.getOrderBy();
        if (tableName.empty()) {
            std::cout << "TABLE NAME EMPTY" << std::endl;
            return false;
        }


         load_table_by_title(tableName); 

         if (!orderBy.empty()) {
             aEntity.order_by(orderBy);
         }  //This is where it orders the rows

        TB_viewer aView(output,&aEntity);
        
        aView.setlimit(limit); //sets the limit
        

        if(columnTitles.empty() ){
            aView.prettyPrint();
            return true;
        }

        aView.prettyPrintSelection(select_columns(columnTitles));
        return true;
    }
    
    boolSelection QRY_Processor::select_columns(const std::vector<std::string>& columnTitles){
        //select all case,
        if(columnTitles.empty()){
            boolSelection isDisplayColumn(aEntity.attributes.size(),true);
            return isDisplayColumn;
        }
        if(columnTitles[0]=="*"){
            boolSelection isDisplayColumn(aEntity.attributes.size(),true);
            return isDisplayColumn;
        }
        //select some case,
        bool isInAttributes;
        boolSelection isDisplayColumn;
        for(auto tableAtt: aEntity.attributes){
            isInAttributes=false;
            for(auto title: columnTitles){
                if(tableAtt.id==title){isInAttributes=true;}
            }
            isDisplayColumn.push_back(isInAttributes);
        }
        return isDisplayColumn;
    }

    bool QRY_Processor::select_from_w_constraints(Query& aQuery){
        //load table
        auto tableName = aQuery.getTableName();
        auto columnTitles = aQuery.getAttributes();
        auto limit = aQuery.getlimit();
        auto orderBy = aQuery.getOrderBy();
        if (aQuery.tableName.empty()) {
            std::cout << "TABLE NAME EMPTY" << std::endl;
            return false;
        }
        load_table_by_title(aQuery.tableName);

        if (!orderBy.empty()) {
            aEntity.order_by(orderBy);
        }  //This is where it orders the rows

        boolSelection theCols=select_columns(aQuery.attributeTitleConstraints);
        boolSelection theRows=constrain_rows(aQuery.theRowConstraints, &aEntity);
        //boolSelection theRows(aEntity.theRowCollection.size(),true);
        TB_viewer aView(output,&aEntity);
        if(aQuery.limit>0){aView.setlimit(aQuery.limit);}
        aView.prettyPrintSelection(theCols,theRows);
        
        return true;

    }


    bool cellWithinConstraints(Cell aCell,RowConstraint aCmd){
        int medex=aCell.index();
        int youdex=aCmd.value.index();
        switch(aCmd.op){
            case '>':
            return aCell>aCmd.value;
            case '=':
            return aCell == aCmd.value;
            case '<':
            return aCell<aCmd.value;
        }
    }

    boolSelection QRY_Processor::constrain_rows(std::vector<RowConstraint> theRowConstraints, Entity* pTB){
        int column=0;
        int attributeDatatype=0;
        Cell TB_cell;
        boolSelection isRowWithinConstraints;
        //iterate through constraints
        for(auto it :theRowConstraints){
            column = get_column_num_from_attribute_name(it.attributeString);
            if(column>=0){
                //get the expected datatype in the column and convert the constraint value into the proper type
                attributeDatatype=aEntity.attributes[column].type2CellIndex();
                //all cells are initiate as strings regardless of type and need to be converted using information
                //from the attribute row column
                it.value.fromStr(std::get<std::string>(it.value),attributeDatatype);

                //iterate through all rows and execute function.
                for(auto jt:aEntity.theRowCollection){
                    isRowWithinConstraints.push_back(cellWithinConstraints(jt.cells[column],it));
                }
            }else{}//throw error
        }
        return isRowWithinConstraints;
    }

    int QRY_Processor::get_column_num_from_attribute_name(std::string name){
        int i=0;
        for(auto it: aEntity.attributes){
            if(it.id==name){return i;}
            i++;
        }
        return -1;
    }
    
    
}