//
//  MetaFactory.hpp
//  RGAssignment3
//
//  Created by rick gessner on 2/8/21.
//

#ifndef Builder_hpp
#define Builder_hpp

#include <iostream>
#include "JSONModel.hpp"
#include "TestDocument.hpp"

namespace ECE141 {
    
  class Builder {
  public:
    static TestDocument* create(JSONModel &aModel);
  };

}

#endif /* Builder_hpp */
