#ifndef TB_VIEWER
#define TB_VIEWER
#include "View.hpp"
#include "Entity.hpp"

namespace ECE141{
 
 class TB_viewer:public View{
    public:
    TB_viewer(std::ostream &aStream, Entity* TBptr):View(aStream), pTBL(TBptr),limit(100){if(!pTBL){};};
    //display information about columns
    bool prettyPrint(std::vector<Attribute> attributes);
    bool prettyPrint(Attribute anAttribute);
    bool prettyPrint(Database* pdb);

    // display information about rows
    bool prettyPrint();
    bool prettyPrintSelection(std::vector<bool> isDispCol);
    bool prettyPrintSelection(std::vector<bool> isDispCol, std::vector<bool> isDispRow);

    void setlimit(int alimit) {limit = alimit;}

    void setOrderBy(std::string aOrderBy) {orderBy = aOrderBy;}
    
    protected:
    int limit;
    std::string orderBy;
        
    Entity* pTBL;
    
    
     
  };
}
#endif