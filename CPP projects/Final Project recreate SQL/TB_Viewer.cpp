#include "TB_Viewer.hpp"

namespace ECE141{
    // internal function prototypes for functions.
    
    //print a specific view for understanding attributes of a table
    bool TB_viewer::prettyPrint(std::vector<Attribute> attributes){
        //read from temporary entity to screen;
		out << "\n+---------------------+--------------+------+-----+---------+-----------------------------+" << std::endl;
		out <<   "| Field               | Type         | Null | Key | Default | Extra                       |" << std::endl;
		out <<   "+---------------------+--------------+------+-----+---------+-----------------------------+" << std::endl;
		for (auto it : pTBL->attributes) {		prettyPrint(it);			}
		out <<   "+---------------------+--------------+------+-----+---------+-----------------------------+" << std::endl;
		out << pTBL->attributes.size() << " rows in set";
        return true;
    }

    bool TB_viewer::prettyPrint(Attribute anAttribute){
        out << "| " << std::left << std::setw(20) << anAttribute.id <<"| " << std::left << std::setw(13);
        std::string Print_type =  anAttribute.type2String( anAttribute.type);
        
        if ( anAttribute.type == DataTypes::varchar_type) {
            Print_type.append("(");
            Print_type.append(std::to_string( anAttribute.field_length));
            Print_type.append(")");
        }
        out << Print_type<< "| "
        << ( anAttribute.nullable ? "YES  " : "NO   ") << "| "
        << std::left << std::setw(4) << ( anAttribute.key ? "YES " : "    ") 
        << std::left << std::setw(10) << "| NULL" << "| " 
        << std::left << ( anAttribute.auto_increment ? "auto_increment" : "") << ( anAttribute.key ? " primary key" : "") << "" << std::endl;
        return true;
    }

     bool TB_viewer::prettyPrint(Database* pdb){
        //doesnt really belong here move to DB_pointer.
        Block aBlock;
		int numrows = 0;
		out	    << "\n+----------------------+"
				<< "\n| Tables               |"
				<< "\n+----------------------+";
		for (int i = 0; i < pdb->storage.getBlockCount(); i++) {
			pdb->storage.readBlock(i, aBlock);
			if (aBlock.header.type == 'E') {
				numrows++;
				out << "\n| " << std::left << std::setw(21) << aBlock.header.title << "|";
			}
		}
		out << "\n+----------------------+" << std::endl;
		out << numrows << " rows in set";
        return true;
    };


    //print a display geared twards seeing the data in a table
    bool TB_viewer::prettyPrint(){
        std::vector<bool> allColumns(pTBL->attributes.size(),true);
        prettyPrintSelection(allColumns);
        return true;
    }//just print all attributes

    bool TB_viewer::prettyPrintSelection(std::vector<bool> isDispCol){
        std::vector<bool> selectAllRows(pTBL->theRowCollection.size(),true);
        prettyPrintSelection(isDispCol,selectAllRows);
        return true;
    }


    bool TB_viewer::prettyPrintSelection(std::vector<bool> isDispCol,std::vector<bool> isDispRow){
        //do safety check that the input vectors are at least as long as the rows and attributes;
        if(isDispCol.size()< pTBL->attributes.size() || isDispRow.size() < pTBL->theRowCollection.size())
            {return false;}
        //iterate through attributes to set the width of the data columns
        int i=0;
        std::string gridLine;
        std::vector<int> columnWidth(pTBL->attributes.size(),0);
        for(auto it: pTBL->attributes){
            if(isDispCol[i]){
                columnWidth[i]=it.id.size()+6;
                gridLine+="+";
                for(int j=0; j<columnWidth[i]*1.1;j++){gridLine+="-";}
            }
            i++;
        }
        //print grid lines
        out<<gridLine<<"+\n| ";
        //load attribute id's
        i=0;
        for(auto it: pTBL->attributes){
            if(isDispCol[i]){
                out<< std::left << std::setw(columnWidth[i]) << it.id <<"| " ;
            }
            i++;
        }
        out<<"\n";

        //load line again
        out<<gridLine<<"+\n";

        //---------------------------------------
        //load rows;
        i=0;
        int j=0 , nRows=0;
        for(auto aRow:pTBL->theRowCollection){
            if(isDispRow[i]){
                out<<"| ";
                for(auto aCell: aRow.cells){
                    if(isDispCol[j]){
                        out<< std::left << std::setw(columnWidth[j]);
                        aCell.print(out);
                        out<<"| ";
                    }
                    j++;
                }
                nRows++;
                out<<"\n";   
            }
            j=0;
            i++;  

            if (nRows == limit) {break;}//100 row limit built into viewer
        }
        //load line again
         out<<gridLine<<"+\n";
        out<<"\n"<<nRows<<" rows in set  "; 
        return true; 
    }
}