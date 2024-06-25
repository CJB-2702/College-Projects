#include <string>
#include <iostream>
#include "String.hpp"

int main(){
    //std::string theStringA("hello there world!");
    //std::string theStringB(theStringA);
    ECE141::String theStringA("hello there world!");
    ECE141::String theStringB(theStringA);
    theStringA+=theStringB;
    theStringB=theStringA;
    std::cout<<theStringA;
    std::cin>>theStringA;
    std::cout<<theStringA;
    std::cout<<theStringB;
    //*/
    return 1;
}