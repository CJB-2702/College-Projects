//
//  main.cpp
//  Database2
//
//  Created by rick gessner on 3/17/19.
//  Copyright © 2019 rick gessner. All rights reserved.
//

#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include <cmath>
#include <functional>
#include <variant>

#include "TestManually.hpp"
#include "TestAutomatic.hpp"

int main(int argc, const char * argv[]) {
        
  const char* theStatus[]={"FAIL","PASS"};

  // insert code here...
 if(argc>1) { //if (1) {//
     
        
    ECE141::TestAutomatic theTests;
    
    srand(static_cast<uint32_t>(time(0)));

    std::string temp(argv[1]);//
    if("compile"==temp) {
      std::cout << "Compile test "
        << theStatus[theTests.doCompileTest()] << "\n";
    }
    else if("app"==temp) {
      std::cout << "App test " <<theStatus[theTests.doAppTest()]<<"\n";
    }
    else if("db"==temp) {
      std::cout << "DB test " <<
        theStatus[theTests.doDBTest()]<<"\n";
    }
    else if("tables"==temp) {
      std::cout << "Tables test " <<
        theStatus[theTests.doTablesTest()]<<"\n";
    }
    else if("insert"==temp) {
      std::cout << "Insert test " <<
        theStatus[theTests.doInsertTest()]<<"\n";
    }
    else if("select"==temp) {
      std::cout << "Select test " <<
        theStatus[theTests.doSelectTest()]<<"\n";
    }
    else if("change"==temp) {
      std::cout << "Change test " <<
        theStatus[theTests.doChangeTest()]<<"\n";
    }
    else if("index"==temp) {
      std::cout << "Index test " <<
        theStatus[theTests.doIndexTest()]<<"\n";
    }
    else if("join"==temp) {
      std::cout << "Join test " <<
        theStatus[theTests.doJoinTest()]<<"\n";
    }
    else if("cache"==temp) {
      std::cout << "Cache test " <<
        theStatus[theTests.doCacheTest()]<<"\n";
    }
  }
  else {
    doManualTesting();
  }
  return 0;
}
