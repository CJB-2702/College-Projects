//
//  JSONModel.hpp
//  RGAssignment3
//
//  Created by rick gessner on 2/8/21.
//

#ifndef JSONModel_hpp
#define JSONModel_hpp

#include <stdio.h>
#include <string>
#include <iostream>
#include <iomanip>
#include <vector>


namespace ECE141 {

  enum class TokenType {
    openObject, openList, closeList, closeObject,
    constant, quoted, colon, unknown};

  enum class JSONType {object, list, element, constant, strConstant, unknown, main};

  
  //====PARENT==================================================
  class JSONPart {
    public:
    JSONPart(JSONType aType){type=aType;};
    JSONType type;
    virtual JSONPart& debugDump(std::ostream &anOutput, int indent=0)=0;
    virtual JSONPart& push_back(JSONPart* anObject){std::cout<<"\nerror:16\n";return *this;};
    virtual JSONPart& setKey(std::string){std::cout<<"\nerror:17\n";return *this;};


    virtual std::string getKey(){std::cout<<"error:18";std::string temp;return temp;};
    virtual std::string getValue(int i=0){std::cout<<"error:19";return nullptr;};
    virtual std::vector<JSONPart*> getCollection(){
       std::cout<<"error:20";
       std::vector<JSONPart*> empty; 
       return empty;
    };
    virtual ~JSONPart(){};
  };
  //-----core--------------------------
  class JSONElement : public JSONPart {
  public:
    JSONElement(const std::string &aKey, JSONPart *aValue=nullptr):JSONPart(JSONType::element){};
    JSONElement();
    std::string key;
    JSONPart* value;
    virtual ~JSONElement(){delete value;}

    virtual JSONPart& push_back(JSONPart* anObject){value=anObject;return *this;}
    virtual JSONPart& setKey(std::string aKey){key=aKey;return *this;};
    virtual std::string getKey(){return key;};
    virtual JSONPart& debugDump(std::ostream &anOutput, int indent=0){
      for(int i=0;i<indent;i++){anOutput<<"  ";}
      anOutput<<key<<"=";
      value->debugDump(anOutput,indent+1);
      return *this;
    };
    virtual std::vector<JSONPart*> getCollection(){
      std::vector<JSONPart*> temp;
      temp.push_back(value);
      return temp;
    }
    virtual int length(){return 1;};
  };
  //====BASE values==============================================
  //values that come in quotes
  class JSONStrConst : public JSONPart {
    public:
    JSONStrConst(const std::string &aValue):JSONPart(JSONType::constant), value(aValue) {value=aValue;};
    std::string value;
    //something is wrong with my constructor so using a hack
    virtual JSONPart& setKey(std::string aKey){value=aKey;return *this;};
    virtual JSONPart& debugDump(std::ostream &anOutput, int indent=0){anOutput<<value<<std::endl;return *this;}
    virtual ~JSONStrConst (){};
    virtual std::string getValue(int i=0){return value;}
  };
  //values that don't have quotes ex: true, 35, 1.5
  class JSONConst : public JSONPart {
    public:
    JSONConst(const std::string &aValue):JSONPart(JSONType::constant), value(aValue) {}
    std::string value;
    virtual JSONPart& setKey(std::string aKey){value=aKey;return *this;};
    virtual JSONPart& debugDump(std::ostream &anOutput, int indent=0){anOutput<<value<<"\n";return *this;}
    virtual ~JSONConst(){};
    virtual std::string getValue(int i=0){return value;}
  };
  //====MULTI ITEM TYPES========================

  //has a key associated with it,  ex: "KEY":[x,y,{foo,bar},...],
  //only holds objects or constants
  class JSONList : public JSONPart {
  public:
    JSONList():JSONPart(JSONType::list){};
    virtual JSONPart& push_back(JSONPart* aValue){ptrList.push_back(aValue);return *this;};
    virtual JSONPart& debugDump(std::ostream &anOutput, int indent=0){
      anOutput<<"(list):\n";
      for(auto it:ptrList){
        it->debugDump(anOutput,indent+1);
      }
      return *this;
    };
    virtual std::vector<JSONPart*> getCollection(){return ptrList;}
    virtual ~JSONList(){for(auto it:ptrList){delete it;}}
  protected:
    std::vector<JSONPart*> ptrList;
  };

  //no key associated but all items have keys  ex: {"key":"value","key2": 1.5}
  //everything inside has pairs of keys and objects/constants
  class JSONObject : public JSONPart{
  public:
    JSONObject():JSONPart(JSONType::object){};
    virtual JSONPart& push_back(JSONPart* anElement){theElements.push_back(anElement);return *this;};
    virtual JSONPart& debugDump(std::ostream &anOutput, int indent=0){
      anOutput<<" (object):\n";
      for(auto it:theElements){
        it->debugDump(anOutput,indent+1);
      }
      return *this;
    };
    virtual std::vector<JSONPart*> getCollection(){return theElements;}
    virtual ~JSONObject(){for(auto it:theElements){delete it;}}
  protected:
    std::vector<JSONPart*> theElements;
  };

  //==== THE MAIN OBJECT================================
  // A set of objects with no keys
  class JSONModel: public JSONObject {
  public:
    using JSONObject::JSONObject;   
  
    virtual JSONPart& debugDump(std::ostream &anOutput, int indent=0){
      for(auto it:theElements){
        it->debugDump(anOutput,0);
      }
      return *this;
    };//*/
  };
}
#endif /* JSONModel_hpp */
