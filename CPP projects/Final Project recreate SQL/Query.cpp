//
//  Query.cpp
//  Assignment5
//
//  Created by rick gessner on 4/26/21.
//

#include "Query.hpp"

namespace ECE141 {

	//implement your query class here...

	Query::Query() :offset(-1), limit(-1) {
		clear();
	}

	Query::~Query()
	{
	}

	bool Query::selectAll() {
		allFlag = true;
		return false;
	}

	Query& Query::setSelectAll(bool aState) {
		return *this;
	}

	Query& Query::setSelect(const StringList& aFields) {
		attributeTitleConstraints = aFields;
		return *this;
	}

	Entity* Query::getFrom() const {
		return nullptr;
	}

	Query& Query::setFrom(Entity* anEntity)
	{
		//from = anEntity;
		return *this;
	}

	Query& Query::set_tb_name(std::string& aName)
	{
		tableName = aName;
		return *this;
	}

	Query& Query::setOffset(int anOffset)
	{
		offset = anOffset;
		return *this;
		// TODO: insert return statement here
	}

	bool Query::setLimit(int aLimit){
		limit = aLimit;
		return true;
	}

	bool Query::setOrderBy(std::string aOrderBy){
		OrderBy = aOrderBy;
		return false;
	}

	bool Query::addRowConstraint(std::string aAtt, char aOp, Cell aCell){
		RowConstraint aRowConstraint{ aAtt,aOp,aCell };
		theRowConstraints.push_back(aRowConstraint);
		return true;
	}

	StringList Query::getAttributes(){
		return attributeTitleConstraints;
	}

	std::string Query::getTableName(){
		return tableName;
	}

	bool Query::clear(){
		allFlag = false;
		tableName = "";
		attributeTitleConstraints.clear();
		theRowConstraints.clear();
		offset = 0;
		limit = -1;
		return false;
	}

}