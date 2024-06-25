/*
#include "TestDocument.hpp"
#include "Builder.hpp"
#include "JSONReader.hpp"

#include <fstream>
#include <string>
#include <iostream>


//using namespace std;
int main(){
    std::ofstream f("z.json");
    ECE141::TestDocument trash;
    trash.toJSON(f);
    f.close();

    std::ifstream in("z.json");

    ECE141::JSONModel aModel;
    ECE141::JSONReader aReader(aModel,in);
    aReader.tokenize();
    aModel.debugDump(std::cout);

    ECE141::TestDocument* td=ECE141::Builder::create(aModel);
    std::ofstream fcopy("zCopy.json",std::ofstream::out | std::ofstream::trunc);
    td->toJSON(fcopy);
    return 1;
}
//*/