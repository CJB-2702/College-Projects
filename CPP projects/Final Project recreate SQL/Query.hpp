//
//  Query.hpp
//  Assignment5
//
//  Created by rick gessner on 4/26/21.
//*

#ifndef Query_hpp
#define Query_hpp

#include <stdio.h>
#include <string>
#include "Attribute.hpp"
#include "Row.hpp"
#include "Entity.hpp"
#include "Tokenizer.hpp"

namespace ECE141 {
    
    struct RowConstraint {
        std::string attributeString;
        char op;
        Cell value;//change to string later
    };

    class Query {
    public:
        Query();
        ~Query();
        
       
        std::string tableName; //The table name
        StringList attributeTitleConstraints; //A vector of the column names to display
        std::vector<RowConstraint> theRowConstraints;
        int       offset;
        int       limit; //How many data rows to display
        std::string OrderBy;   //This is what metric to order by
        bool fromLowToHigh;
        bool allFlag; //If the asterisk was used


        


        bool selectAll();

        Query& setSelectAll(bool aState);
        Query& setSelect(const StringList& aFields);

        Entity* getFrom() const;

        Query& setFrom(Entity* anEntity);
        Query& set_tb_name(std::string& aName);
        Query& setOffset(int anOffset);
        bool   setLimit(int aLimit);
        bool   setOrderBy(std::string aOrderBy);
        bool   addRowConstraint(std::string aAtt, char aOp, Cell aCell);


        StringList getAttributes();
        std::string getTableName();
        int         getlimit() { return limit; }
        std::string getOrderBy() {
            return OrderBy;
        }
        bool    clear();
        //how will you handle where clause?
        //how will you handle group by?
        

    };

}
#endif /* Query_hpp */