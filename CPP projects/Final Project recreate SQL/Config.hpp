//
//  Config.hpp
//  RGAssignment2
//
//  Created by rick gessner on 2/27/21.
//

#ifndef Config_h
#define Config_h
#include <sstream>
#include <filesystem>

struct Config {

  static const char* getDBExtension() {return ".db";}

  static std::string getStoragePath() {
      
    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
      //@ ming the autograder uses linux this line works for both of us don't change
      return getStorageString();
      
    #elif __APPLE__ || defined __linux__ || defined __unix__
      
      return "/tmp";  //MAC, UNIX, LINUX here...
    
    #endif
  }
  
  static std::string getStorageString(){
    std::filesystem::path theDir = std::filesystem::current_path();
    return theDir.string()+"\\tmp";
  }

  static std::string getDBPath(const std::string &aDBName) {
    std::ostringstream theStream; //I changed it back because the previous version wasn't working
    theStream << Config::getStoragePath() << R"(/)" << aDBName << ".db";
    return theStream.str();
  }
  
};

#endif /* Config_h */
