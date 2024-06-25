

#include "QRY_Statement.hpp"
#include "Tokenizer.hpp"
namespace ECE141{



	StatusResult  QRY_Statement::parse(Tokenizer &aTokenizer){
		aQuery.clear();
		size_t tokensStart = aTokenizer.index_cur();
		StringList fields;
		
		while (aTokenizer.current().data != ";") { //starting at select token
			
			if (aTokenizer.current().keyword == Keywords::select_kw) {
				aTokenizer.next();
				parse_select_constraints(aTokenizer);
			}

			aTokenizer.next();

			if (aTokenizer.current().keyword == Keywords::where_kw) {
				aTokenizer.next();
				parse_where_constraints(aTokenizer);
			}

			if (aTokenizer.current().data == ",") {
				continue;
			}

			if (aTokenizer.current().data == "*") { //This is to flag all
				//std::cout << "ALLFLAG" << aTokenizer.current().data;
				aQuery.selectAll();
				continue;
			}

			if (aTokenizer.current().keyword == Keywords::from_kw) {
				aTokenizer.next();
				if (aTokenizer.current().type == TokenType::identifier) {
					aQuery.set_tb_name(aTokenizer.current().data);
				}
				continue;
			}

			if (aTokenizer.current().keyword == Keywords::limit_kw) {
				aTokenizer.next();
				if (aTokenizer.current().type == TokenType::number) {
					aQuery.setLimit(std::stoi(aTokenizer.current().data));
				}
				continue;
			}

			if (aTokenizer.current().keyword == Keywords::order_kw) {
				aTokenizer.next();
				aTokenizer.next();
				aQuery.setOrderBy(aTokenizer.current().data);
				continue;
			}
		}

		return StatusResult{ Errors::noError };
	}

	bool QRY_Statement::parse_where_constraints(Tokenizer& aTokenizer) {
		if (aTokenizer.current().type == TokenType::identifier) {
			while (aTokenizer.current().type == TokenType::identifier) {
					
					std::string aAtt = aTokenizer.current().data;
					aTokenizer.next();
					char aOp = aTokenizer.current().data.c_str()[0];
					aTokenizer.next();
					Cell aCell = aTokenizer.current().data; //The Cell won't know which kind of data is in it
					aQuery.addRowConstraint(aAtt, aOp, aCell);
					aTokenizer.next();
					
					if (aTokenizer.current().keyword == Keywords::and_kw) { // something=4 and something=3
						aTokenizer.next();
					}

					//std::cout << aTokenizer.current().data << std::endl;
			}
			return true;
		}
		return false;
	}

	bool QRY_Statement::parse_select_constraints(Tokenizer& aTokenizer){
		StringList fields;
		if (aTokenizer.current().type == TokenType::identifier) {
			while (aTokenizer.current().type == TokenType::identifier || aTokenizer.current().type == TokenType::punctuation) {
				if (aTokenizer.current().type == TokenType::identifier) { fields.push_back(aTokenizer.current().data); }
				if (aTokenizer.peek().keyword == Keywords::from_kw) {
					aQuery.setSelect(fields);
					break;
				}
				else {
					aTokenizer.next();
				}
			}
		}
		return true;
	}

	StatusResult  dispatch() {return StatusResult{};}

}