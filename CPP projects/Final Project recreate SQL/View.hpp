//
//  View.hpp
//  RGAssignment2
//
//  Created by rick gessner on 2/27/21.
//

#ifndef View_h
#define View_h

#include <iostream>
#include <filesystem>
#include "Config.hpp"
#include <string>

    


namespace ECE141 {

  //completely generic view, which you will subclass to show information
  class View {
    public:
    View(std::ostream &aStream):out(aStream){};
    ~View(){};
    virtual bool    prettyPrint()=0;
    protected:
    std::ostream &out;
  };
 
}

#endif /* View_h */
