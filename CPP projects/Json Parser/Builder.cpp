//
//  MetaFactory.cpp
//  RGAssignment3
//
//  
// Bissett note: I forgot about switches pardon the if statments.

#include "Builder.hpp"
#include <cstring>
#include <string>

namespace ECE141 {
  bool isTestDocument(JSONModel &aModel);
  std::vector<JSONPart*> getDocumentMembers(JSONModel &aModel);
  float get_price(JSONModel &aModel);
  TestClass1 get_tc1_from_JSONPart_vect(std::vector<JSONPart*> tc1);
  TestClass2 get_child2(JSONModel &aModel);
  TestClass1 get_child1b(JSONModel &aModel);
  TestClass1 get_child1a(JSONModel &aModel);
  TestClass3 get_child3(JSONModel &aModel);

  TestDocument* Builder::create(JSONModel &aModel) {
    //STUDENT implement
    //Read the model, and construct Document and test child classes
    //return a newly created Document based on given model...
    if(isTestDocument(aModel)){
      float price=get_price(aModel);
      TestClass1  child1a=get_child1a(aModel);
      TestClass1  child1b=get_child1b(aModel);
      TestClass2  child2=get_child2(aModel);
      TestClass3  child3=get_child3(aModel);
      //child3.toJSON(std::cout);
      return new TestDocument(child1a, child1b, child2, child3,price);
    }
    return nullptr;
  
  }//*/

  bool isTestDocument(JSONModel &aModel){
    //first bracket should just be one object
    std::vector<JSONPart*> modelVect=aModel.getCollection();
    //should have two elements in it, "meta" and "members"
    std::vector<JSONPart*> metaNmembers=modelVect[0]->getCollection();
    //open the meta object and make sure the name is correct    
    if(metaNmembers[0]->getKey()=="meta"){
      //get the meta object
      std::vector<JSONPart*> metaObj=metaNmembers[0]->getCollection();
      //get the first key
      std::vector<JSONPart*> classNversion= metaObj[0]->getCollection();
      if(classNversion[0]->getKey()=="class"){
        std::vector<JSONPart*> classConst=classNversion[0]->getCollection();
        //if the item is "TestDocument"
        if(classConst[0]->getValue()=="TestDocument"){
          return true;
        }
      }
    }
    return false;
  }

  std::vector<JSONPart*> getDocumentMembers(JSONModel &aModel){
    //skip right to the actual elements in the document
    std::vector<JSONPart*> model_obj=aModel.getCollection();
    std::vector<JSONPart*> model_elements=model_obj[0]->getCollection();
    std::vector<JSONPart*> model_element_object=model_elements[1]->getCollection();
    return model_element_object[0]->getCollection();
  }

  float get_price(JSONModel &aModel){
    for(auto it : getDocumentMembers(aModel)){
      if(it->getKey()=="price"){
        std::vector<JSONPart*> priceValue=it->getCollection();
        return std::stof(priceValue[0]->getValue());
      }
    }
    return -1;
  }

  TestClass1 get_tc1_from_JSONPart_vect(std::vector<JSONPart*> tc1){
    std::string name("err");
    int         count=0;
    float       amount=0;
    bool        used=false;

    for(auto element: tc1){
      if(element->getKey()=="members"){
        std::vector<JSONPart*> element_obj=element->getCollection();
        std::vector<JSONPart*> tc1_members=element_obj[0]->getCollection();
        for(auto j:tc1_members){
          if(j->getKey()=="name"){
            std::vector<JSONPart*> tc1_name=j->getCollection();
            name=tc1_name[0]->getValue();
          }
          if(j->getKey()=="count"){
            std::vector<JSONPart*> tc1_count=j->getCollection();
            count=std::stoi(tc1_count[0]->getValue());
          }
          if(j->getKey()=="amount"){
            std::vector<JSONPart*> tc1_amount=j->getCollection();
            amount=std::stof(tc1_amount[0]->getValue());
          }
          if(j->getKey()=="used"){
            std::vector<JSONPart*> tc1_used=j->getCollection();
            used=(tc1_used[0]->getValue()=="true");
          }
        }
      }
    }
    TestClass1 aClass(name,used, amount, count);
    return aClass;
  }
  TestClass1 get_child1a(JSONModel &aModel){
    for(auto i : getDocumentMembers(aModel)){
      if(i->getKey()=="child1a"){
        std::vector<JSONPart*> obj=i->getCollection();
        return get_tc1_from_JSONPart_vect(obj[0]->getCollection());
      }
    }
    TestClass1 trash;
    return trash;
  }
  TestClass1 get_child1b(JSONModel &aModel){
    for(auto i : getDocumentMembers(aModel)){
      if(i->getKey()=="child1b"){
        std::vector<JSONPart*> obj=i->getCollection();
        return get_tc1_from_JSONPart_vect(obj[0]->getCollection());
      }
    }
    TestClass1 trash;
    return trash;
  }


  TestClass2 get_child2(JSONModel &aModel){
    /*struct Metadata {
    char    prop1;
    uint8_t prop2;
    bool    prop3;
    //};//*/
    std::string name;
    size_t      size;
    Metadata    meta;

    for(auto elements : getDocumentMembers(aModel)){
      if(elements->getKey()=="child2"){
        std::vector<JSONPart*> obj=elements->getCollection();
        std::vector<JSONPart*> meta_n_members=obj[0]->getCollection();
        for(auto it:meta_n_members){
          if(it->getKey()=="members"){
            std::vector<JSONPart*> tc2_obj=it->getCollection();
            std::vector<JSONPart*> tc2_elements=tc2_obj[0]->getCollection();
            for(auto j:tc2_elements){
              if(j->getKey()=="name"){
                std::vector<JSONPart*> val_vect=j->getCollection();
                name=val_vect[0]->getValue();
              }
              if(j->getKey()=="size"){
                std::vector<JSONPart*> val_vect=j->getCollection();
                size=std::stoi(val_vect[0]->getValue());
              }
              if(j->getKey()=="Metadata"){
                std::vector<JSONPart*> metadata_obj=j->getCollection();
                std::vector<JSONPart*> metadata_vect=metadata_obj[0]->getCollection();
                //has a struct must break down further
                for(auto k:metadata_vect){
                  if(k->getKey()=="prop1"){
                    std::vector<JSONPart*> k_obj=k->getCollection();
                    std::string ktrash=k_obj[0]->getValue();
                    meta.prop1=ktrash[0];//get first char in string
                  }
                  if(k->getKey()=="prop2"){
                    std::vector<JSONPart*> val_vect=k->getCollection();
                    meta.prop2=(val_vect[0]->getValue())[0];//uint8_t is just a char
                  }
                  if(k->getKey()=="prop3"){
                    std::vector<JSONPart*> val_vect=k->getCollection();
                    std::string ktrash=val_vect[0]->getValue();
                    meta.prop3=ktrash.find("true")!=std::string::npos;//get first char in string
                  }
                }
              }
            }
          }
        }
      }
    }
    TestClass2 aClass(name,size,meta);
    return aClass;
  }

  TestClass3 get_child3(JSONModel &aModel){
    std::string name("error");
    char        initials[4]="err";
    Color       color=Color::blue;
    for(auto i : getDocumentMembers(aModel)){
      if(i->getKey()=="child3"){
        std::vector<JSONPart*> cld3_obj=i->getCollection();
        std::vector<JSONPart*> meta_n_members=cld3_obj[0]->getCollection();
        for(auto j:meta_n_members){
          if(j->getKey()=="members"){
            std::vector<JSONPart*> meta_obj=j->getCollection();
            std::vector<JSONPart*> meta_ele=meta_obj[0]->getCollection();
            for(auto k:meta_ele){
              //std::cout<<k->getKey()<<std::endl;
              if(k->getKey()=="name"){
                std::vector<JSONPart*> kobj=k->getCollection();
                std::string tempk=kobj[0]->getValue();
                name=tempk;
              }
              if(k->getKey()=="initials"){
                std::vector<JSONPart*> value=k->getCollection();
                std::string tempk=value[0]->getValue();
                initials[0]=tempk[0];
                initials[1]=tempk[1];
                initials[2]=tempk[2];
                initials[3]='\0';
              }
              if(k->getKey()=="color"){
                std::vector<JSONPart*> value=k->getCollection();
                std::string tempk=value[0]->getValue();
                color=(tempk=="red" ? Color::red : (tempk=="green"? Color::green : Color::blue));
              }
            }
          }
        }
      }
    }
    TestClass3 aClass(name, color, initials);
    return aClass;
  }
}
