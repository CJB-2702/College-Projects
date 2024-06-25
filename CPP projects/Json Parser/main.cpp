//
//  main.cpp
//  RGAssignment3
//
//  Created by rick gessner on 2/6/21.
//

#include <iostream>
#include "Testing.hpp"
#include <ctime>

          
int main(int argc, const char * argv[]) {    

  const char* theStatus[]={"FAIL","PASS"};
      
  // insert code here...
  if(argc>1) {
        
    ECE141::Testing theTests;
    
    srand(time(0));

    std::string temp(argv[1]);
    
    if("compile"==temp) {
      std::cout << "Compile test " << theStatus[theTests.doCompileTest()] << ".\n";
    }
    else if("write"==temp) {
      std::cout << "Write test "
        << theStatus[theTests.doWriteTest()] << ".\n";
    }
    else if ("read"==temp) {
      std::cout << "Read test "
        << theStatus[theTests.doReadTest()] << ".\n";
    }
    else if ("round"==temp) {
      std::cout << "Round test "
        << theStatus[theTests.doRoundTripTest()] << "\n";
    }
  }
  
  return 0;
}
