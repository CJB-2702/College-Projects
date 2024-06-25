#ifndef QRYStatement_hpp
#define QRYStatement_hpp
#include "Statement.hpp"
#include "Query.hpp"
namespace ECE141{
    class QRY_Statement: public Statement {
    public:
        //base functions that belong in all statements
        QRY_Statement():Statement(){};
        QRY_Statement(const Statement &aCopy);
        virtual std::string   getClassName(){return "QRY_Statement";};
        virtual StatusResult  parse(Tokenizer &aTokenizer);
        virtual StatusResult  dispatch() {return StatusResult{};}
        virtual bool          clear(){return aQuery.clear();};
        
        //Query Class holds all statment data
        Query aQuery;
    private:
        bool parse_where_constraints(Tokenizer& aTokenizer);
        bool parse_select_constraints(Tokenizer& aTokenizer);

        
    };
}
#endif