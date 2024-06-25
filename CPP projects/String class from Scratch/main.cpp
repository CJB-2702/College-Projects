//
//  main.cpp
//  RGAssignment1
//
//  Created by rick gessner on 1/10/21.
//

#include <iostream>
#include "StringTest.hpp"

int main(int argc, const char * argv[]) {
  const char* theStatus[]={"FAIL","PASS"};
  // insert code here...
  if(argc>1) {
    std::string temp(argv[1]);
    if("compile"==temp) {
      std::cout << "Compile test PASS.";
    }
    else if("OCF"==temp) {
      std::cout << "OCF test " << theStatus[ECE141::doOCFTest()] << ".\n";
    }
    else if ("mutate"==temp) {
      std::cout << "Mutate test " << theStatus[ECE141::doMutationTest()] << ".\n";
    }
    else if ("compare"==temp) {
      std::cout << "Compare test "
        << theStatus[ECE141::doComparisonTest()] << ".\n";
    }
  }
  
  return 0;
}
