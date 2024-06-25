#ifndef DB_VIEWER
#define DB_VIEWER
#include "View.hpp"
#include "Database.hpp"
#include <filesystem>

namespace ECE141{
 
 class DB_viewer:public View{
    public:
    DB_viewer(std::ostream &aStream):View(aStream){};
    //display information about columns
    bool prettyPrint();
    bool prettyPrint(Database* aDB);
    bool prettyPrint(std::string aFilePath);
  };
}
#endif