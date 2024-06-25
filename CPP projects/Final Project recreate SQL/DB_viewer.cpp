 #include "DB_viewer.hpp"
 
namespace ECE141{

	bool DB_viewer::prettyPrint() { out << "undefined action, pass DB or filesystem items\n"; return false; }
    bool DB_viewer::prettyPrint(Database* aDB){
        int num = 0;
        out << "+----------------+--------+---------------+" << std::endl;
		out << "| Type           | Id     | Extra         |" << std::endl;
		out << "+----------------+--------+---------------+" << std::endl;
		Block tmpBlk;
		for (int i = 0; i < aDB->storage.getBlockCount() ; i++) {
			aDB->storage.readBlock(i,tmpBlk);
			num++;
			out << "| " << std::left << std::setw(15) << tmpBlk.prettyType()<< "| " << std::setw(7) << tmpBlk.header.id << "|" << std::setw(16) << tmpBlk.header.title <<"|";
			out << "\n+----------------+--------+---------------+"<< std::endl;
		}
		out << " " << num << " rows in set";
        return true;
    };
    bool DB_viewer::prettyPrint(std::string aFilePath){
        int numRow=0;
        out << "+-------------------------+" << std::endl;
		out << "| Databases               |" << std::endl;
		out << "+-------------------------+" << std::endl;
		for (const auto& entry : std::filesystem::directory_iterator(aFilePath)) {
			std::string temp = entry.path().filename().string();
			if(temp[temp.size()-1]=='b'){
				temp.pop_back();//possibly check substrings for ".db" before displaying files.
				temp.pop_back();
				temp.pop_back();
				numRow++;
				out << "| " << std::left << std::setw(24) << temp << "|" << std::endl;
			}
		}
		out << "+-------------------------+" << std::endl;
		out << " " << numRow << " rows in set";
        return true;
    };
    
}