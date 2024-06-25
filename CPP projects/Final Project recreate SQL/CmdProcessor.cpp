//
//  CommandProcessor.cpp
//  ECEDatabase
//
//  Created by rick gessner on 3/30/18.
//  Copyright © 2018 rick gessner. All rights reserved.
//

#include <iostream>
#include "CmdProcessor.hpp"
#include "Statement.hpp"
#include <memory>

namespace ECE141 {
  
  CmdProcessor::CmdProcessor(std::ostream &anOutput) : output(anOutput){}
  CmdProcessor::~CmdProcessor() {}
  //CmdProcessor* CmdProcessor::recognizes(Tokenizer &aTokenizer) {return nullptr;}


  bool  CmdProcessor::tokens_match(std::vector<Token> theTokens, Tokenizer aTokenizer) { //Compares the tokenizer with given vector of Tokens
		if (theTokens.size() <= aTokenizer.remaining()) {
			int i = 0;
			while (aTokenizer.remaining() && i != theTokens.size() - 1) {
				auto aToken = aTokenizer.current();
				auto compToken = theTokens[i];
				if (aToken.type == compToken.type && aToken.keyword == compToken.keyword) {
					aTokenizer.next();
					i++;
				}
				else { return false; }
			}
			return true;
		}
		return false;
	}
  
  
  bool CmdProcessor:: tokens_exist_ordered(std::vector<Token> expected_tokens, Tokenizer aTokenizer) { //If the tokenizer from the current point to the next semicomma contains the tokens listed
		int index = 0;
		while (aTokenizer.remaining()) {
			 //Goes through the current tokenizer and checks for the word
			auto aToken = aTokenizer.current();
			if (aToken.type == expected_tokens[index].type && (aToken.type == TokenType::keyword || aToken.type == TokenType::identifier)) { 
				//current token it matches the next expected token start looking for next one
				if (aToken.keyword == expected_tokens[index].keyword) {
					index++;
					if (index == expected_tokens.size()) {
						return true;
					}
					aTokenizer.next();
					continue;
				}
			}

			if (aToken.type == expected_tokens[index].type && aToken.type == TokenType::punctuation) { //If they're punctuation, it must be a specific type
				if (aToken.data == expected_tokens[index].data) {
					index++;
					if (index == expected_tokens.size()) {
						return true;
					}
					aTokenizer.next();
					continue;
				}
			}
			aTokenizer.next();
		}
		return false;
	}

}
