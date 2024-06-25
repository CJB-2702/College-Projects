//
//  TestAutomatic.hpp
//  RGAssignement2
//
//  Created by rick gessner on 2/26/21.
//

#ifndef TestAutomatic_h
#define TestAutomatic_h

#include "Application.hpp"
#include "Config.hpp"
#include <sstream>
#include "Errors.hpp"
#include <sstream>
#include <algorithm>
#include <map>
#include <vector>
#include <cctype>
#include <cstdlib>

namespace ECE141 {

  using CountList = std::vector<int>;

  void showErrors(ECE141::StatusResult &aResult, std::ostream &anOutput) {
    
    static std::map<ECE141::Errors, std::string> theMessages = {
      {ECE141::illegalIdentifier, "Illegal identifier"},
      {ECE141::unknownIdentifier, "Unknown identifier"},
      {ECE141::databaseExists, "Database exists"},
      {ECE141::tableExists, "Table Exists"},
      {ECE141::syntaxError, "Syntax Error"},
      {ECE141::unknownCommand, "Unknown command"},
      {ECE141::unknownDatabase,"Unknown database"},
      {ECE141::unknownTable,   "Unknown table"},
      {ECE141::unknownError,   "Unknown error"}
    };

    std::string theMessage="Unknown Error";
    if(theMessages.count(aResult.error)) {
      theMessage=theMessages[aResult.error];
    }
    anOutput << "Error (" << aResult.error << ") " << theMessage << "\n";
  }

  //------------------------------

  class TestAutomatic {
  protected:
    std::stringstream output;
  public:
    
    ~TestAutomatic() {
      std::cout << "Test Version 1.6\n";
      std::cout << "---- output ---------------\n";
      std::cout << output.str() << "\n";
    }

    bool doCompileTest() {return true;}

    bool doScriptTest(std::istream &anInput, std::ostream &anOutput) {
      ECE141::Application   theApp(anOutput);
      ECE141::StatusResult  theResult{};
      std::string theCommand;
      
      while(theResult && anInput) {
        std::getline (anInput, theCommand);
        std::stringstream theStream(theCommand);
        anOutput << theCommand << "\n";
        theResult=theApp.handleInput(theStream);
        if(theResult==ECE141::userTerminated) {
          theResult.error=Errors::noError;
          break;
        }
        else if(!theResult) {
          showErrors(theResult, anOutput);
        }
      }
      return theResult;
    }
    
    //----------------------------------------------

    bool doAppTest() {

      std::string theInput("version;\n help;\n quit;");
      std::stringstream theStream(theInput);

      bool theResult=doScriptTest(theStream, output);
      if(theResult) {
        const char* theLines[]={
          "Version 0.6.", "Help system ready.","DB::141 is shutting down."
        };
        
        std::string temp=output.str();
        //std::cout << temp << "\n";
        std::stringstream theStream(temp);
        std::string theLine;

        for(auto *theNext : theLines) {
          std::getline(theStream,theLine); //skip command...
          std::getline(theStream,theLine);
          if(theLine!=theNext) return false;
        }
        return true;
      }
      
      return false;
    }

    enum states {wasUnknown, wasCreate, wasDescribe,
        wasDrop, wasDump, wasInsert, wasSelect, wasShow, wasUse};
    
    //convert from string (word) to states enum...
    states getTerm(const std::string &aWord) {
      static std::map<std::string, states> gTerms = {
        {"create",states::wasCreate},
        {"describe",states::wasDescribe},
        {"drop",states::wasDrop},
        {"dump",states::wasDump},
        {"insert",states::wasInsert},
        {"select",states::wasSelect},
        {"show",states::wasShow},
        {"use",states::wasUse},
      };
      return gTerms.count(aWord) ? gTerms[aWord]
                                 : states::wasUnknown;
    }

    //extract count (numeric) from line
    int getQueryCount(const std::string &aLine) {
      std::stringstream temp(aLine);
      std::string theWord;
      int theNumber;
      temp >> theWord >> theWord >> theNumber;
      return theNumber;
    }
    
    //extract first word (l/c) from line...
    std::string getFirst(const std::string &aLine) {
      std::stringstream temp(aLine);
      std::string theFirst;
      temp >> theFirst;
      std::transform(theFirst.begin(),
                     theFirst.end(), theFirst.begin(),
          [](unsigned char c){ return std::tolower(c); });
      return theFirst;
    }
    
    //validates output of assignment2
    bool hwIsValid(std::istream &aStream, CountList &aCounts) {
      bool theResult=true;
      states theState=states::wasUnknown;
      
      std::string theLine;
      while(theResult && aStream) {
        std::getline(aStream, theLine);
        if(theLine.size()>2) {
          //std::cout << theLine << "\n";
          std::string theWord=getFirst(theLine);
          switch(theState) {
            case wasUnknown: theState=getTerm(theWord); break;
            case wasDrop:
              if(theWord=="query") {
                theState=wasUnknown;
              }
              else return false;
              break;
            case wasUse:
              if(theWord=="database") {
                theState=wasUnknown;
              }
              break;
            case wasCreate:
            case wasInsert:
              if(theWord=="query") {
                aCounts.push_back(getQueryCount(theLine));
                theState=wasUnknown;
              }
              else return false;
              break;
            case wasDescribe:
            case wasDump:
            case wasSelect:
            case wasShow:
              if(std::isdigit(theWord[0])) {
                aCounts.push_back(stoi(theWord));
                theState=wasUnknown;
              }
              else if(wasUnknown!=getTerm(theWord)) {
                return false; //unexpected kw...
              }
              break;
            default: break;
          }
        }
      }
      return theResult;
    }
    
    using FileList = std::vector<std::string>;
    
    bool hasFiles(FileList &aFilelist) {
      for(auto theFile : aFilelist) {
        std::string thePath=Config::getDBPath(theFile);
        std::ifstream theStream(thePath);
        if(!theStream) return false;
      }
      return true;
    }
    
    //----------------------------------------------

    bool doDBTest() {
      
      std::vector<std::string> theFiles;
      theFiles.push_back("testdb"+std::to_string(rand()%9999));
      theFiles.push_back("testdb"+std::to_string(rand()%9999));
      theFiles.push_back("testdb"+std::to_string(rand()%9999));

      std::stringstream theStream1;
      theStream1 << "CREATE DATABASE "+theFiles[0] << ";\n";
      theStream1 << "create database "+theFiles[1] << ";\n";
      theStream1 << "CrEaTe dAtABaSe "+theFiles[2] << ";\n";
      theStream1 << "use "+theFiles[0] << ";\n";
      theStream1 << "shoW databaseS;\n";
   
      std::string temp(theStream1.str());
      std::stringstream theInput(temp);
      bool theResult=doScriptTest(theStream1,output) && hasFiles(theFiles);
      if(theResult) {
        std::stringstream theStream2;
        theStream2 << "DRop dataBASE "+theFiles[1] << ";\n";
        theStream2 << "shoW databaseS;\n";
        theStream2 << "dump database "+theFiles[0] << ";\n";
        
        if((theResult=doScriptTest(theStream2,output))) {

          std::string tempStr=output.str();
          std::stringstream theOutput(tempStr);
          CountList theCounts;
          theResult=hwIsValid(theOutput,theCounts);
          if(theResult) {
            theResult=(theCounts[0]==theCounts[1]) &&   (theCounts[0])==theCounts[2];
            if(theResult) {
              theResult=theCounts[3]==theCounts[4]+1;
            }
          }
          //remove test dbs...
          std::stringstream theStream3;
          theStream3 << "drop database "+theFiles[0] << ";\n";
          theStream3 << "drop database "+theFiles[2] << ";\n";
          std::stringstream temp; //ignore output now...
          doScriptTest(theStream3,temp);
        }
      }
      return theResult;
    }
    
        
    bool doTablesTest() {
      
      std::string theDBName("db_"+std::to_string(rand()%9999));

      std::stringstream theStream1;
      theStream1 << "create database " << theDBName << ";\n";
      theStream1 << "use " << theDBName << ";\n";
      theStream1 << "create table Users (";
      theStream1 << " id int NOT NULL auto_increment primary key,";
      theStream1 << " first_name varchar(50) NOT NULL,";
      theStream1 << " last_name varchar(50));\n";
      
      theStream1 << "create table Accounts (";
      theStream1 << " id int NOT NULL auto_increment primary key,";
      theStream1 << " account_type varchar(25) NOT NULL,";
      theStream1 << " amount int);\n";
      
      theStream1 << "create table Payments (";
      theStream1 << " id int NOT NULL auto_increment primary key,";
      theStream1 << " user_id int,";
      theStream1 << " paydate int,";
      theStream1 << " amount int);\n";
      
      theStream1 << "show tables;\n";
      theStream1 << "describe Accounts;\n";
      theStream1 << "drop table Accounts;\n";
      theStream1 << "show tables;\n";
      theStream1 << "drop database " << theDBName << ";\n";
      theStream1 << "quit;\n";

      std::string temp(theStream1.str());
      std::stringstream theInput(temp);
      bool theResult=doScriptTest(theInput,output);
      if(theResult) {
        std::string tempStr=output.str();
        std::stringstream theOutput(tempStr);
        CountList theCounts;
        if((theResult=hwIsValid(theOutput,theCounts))) {
          static CountList theExpected{1,0,0,0,3,3,2};
          theResult=theCounts==theExpected;
        }
      }
      return theResult;
    }

    bool doInsertTest() {
 
      std::string theDBName("db_"+std::to_string(rand()%9999));

      std::stringstream theStream1;
      theStream1 << "create database " << theDBName << ";\n";
      theStream1 << "use " << theDBName << ";\n";
      theStream1 << "create table Users (";
      theStream1 << " id int NOT NULL auto_increment primary key,";
      theStream1 << " first_name varchar(50) NOT NULL,";
      theStream1 << " last_name varchar(50));\n";
      
      theStream1 << "INSERT INTO Users (first_name, last_name) VALUES ";
      theStream1 << " (\"anirudh\",\"swaminathan\"), ";
      theStream1 << " (\"pu\",\"cheng\");\n";

      theStream1 << "show tables;\n";
      theStream1 << "dump database " << theDBName << ";\n";
      theStream1 << "drop database " << theDBName << ";\n";
      theStream1 << "quit;\n";
      
      std::string temp(theStream1.str());
      std::stringstream theInput(temp);
      bool theResult=doScriptTest(theInput,output);
      if(theResult) {
        std::string tempStr=output.str();
        //std::cout << "output \n" << tempStr << "\n";
        std::stringstream theOutput(tempStr);
        CountList theCounts;
        static CountList theExpected{1,0,2,1,4};
        if((theResult=hwIsValid(theOutput,theCounts))) {
          theResult=theCounts==theExpected;
        }
      }
      return theResult;
    }

    bool doSelectTest() {
 
      std::string theDBName("db_"+std::to_string(rand()%9999));

      std::stringstream theStream1;
      theStream1 << "create database " << theDBName << ";\n";
      theStream1 << "use " << theDBName << ";\n";
      theStream1 << "create table Users (";
      theStream1 << " id int NOT NULL auto_increment primary key,";
      theStream1 << " fname varchar(50) NOT NULL,";
      theStream1 << " lname varchar(50),";
      theStream1 << " zipcode int);\n";

      theStream1 << "INSERT INTO Users (fname, lname, zipcode) VALUES ";
      theStream1 << " (\"anirudh\",\"swaminathan\",92124), ";
      theStream1 << " (\"chandini\",\"grandhi\",92120), ";
      theStream1 << " (\"bryant\",\"liu\",92125), ";
      theStream1 << " (\"kenny\",\"situ\",92120), ";
      theStream1 << " (\"pu\",\"cheng\",92123);\n";

      theStream1 << "select * from Users where zipcode>92120 order by zipcode limit 2;\n";
 
      theStream1 << "show tables;\n";
      theStream1 << "dump database " << theDBName << ";\n";
      theStream1 << "drop database " << theDBName << ";\n";
      theStream1 << "quit;\n";
      
      std::string temp(theStream1.str());
      std::stringstream theInput(temp);
      bool theResult=doScriptTest(theInput,output);
      if(theResult) {
        std::string tempStr=output.str();
        //std::cout << "output \n" << tempStr << "\n";
        std::stringstream theOutput(tempStr);
        CountList theCounts;
        if((theResult=hwIsValid(theOutput,theCounts))) {
          static CountList theOpt1{1,0,5,2,1,7};
          static CountList theOpt2{1,0,5,2,1,8};
          theResult=(theCounts==theOpt1) || (theCounts==theOpt2);
        }
      }
      return theResult;
    }

    bool doChangeTest() {
      bool theResult=false;
      return theResult;
    }

    bool doIndexTest() {
      bool theResult=false;
      return theResult;
    }
    
    bool doJoinTest() {
      bool theResult=false;
      return theResult;
    }

    bool doCacheTest() {
      bool theResult=false;
      return theResult;
    }
  };

}

#endif /* TestAutomatic_h */
