//
//  Storable.hpp
//  Assignment3
//
//  Created by rick gessner on 2/6/21.
//

#ifndef Storable_h
#define Storable_h
#include <iostream>

namespace ECE141 {
  
  class Storable {
  public:
    virtual bool toJSON(std::ostream &aStream)=0;
};

}

#endif /* Storable_h */
