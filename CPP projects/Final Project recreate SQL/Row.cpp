//
//  Row.cpp
//  Assignment4
//
//  Created by rick gessner on 4/19/20.
//  Copyright © 2020 rick gessner. All rights reserved.
//

#include "Row.hpp"
#include "Database.hpp"

namespace ECE141 {
    


    Block Row::Blockify(std::string name, int count, int pos){
        rowNumber=pos;
        Block aBlock;
        //load header block
        aBlock.header.type='D';     //char version of block type
        aBlock.header.count=count;    //how many parts
        aBlock.header.pos=rowNumber;      //i of n...
        aBlock.header.refId=0;    //e.g. id of thing it refers to
        aBlock.header.id=0;       //use this anyway you like
        strcpy(aBlock.header.title,name.c_str());
        //load payload
        std::stringstream ss;
        encode(ss);
        ss.read(aBlock.payload, kPayloadSize);
        return aBlock;
    }
    
    StatusResult  Row::encode(std::ostream& aWriter) {
        //note 4/30/21
        //std::variant<int, bool, float,  std::time_t, std::string>;
        aWriter<< "##ROW## index: "<< rowNumber <<" n_cells: "<<cells.size()<< " ";
        for (auto it : cells) { it.encode(aWriter); }
        aWriter << " ##END## ";
        StatusResult exitStatus{ Errors::noError };
        return exitStatus;
    }


    bool Row::Deblockify(Block& aBlock){
        std::stringstream ss;
        ss.write(aBlock.payload,kPayloadSize);
        decode(ss);
        return true;
    }

    StatusResult Row::decode(std::istream& aReader)
    {
        this->cells.clear();
        std::string meta;
        aReader>>meta;
        if(meta!="##ROW##"){
            std::cout<<"ERROR! Expected \"##ROW##\" Recieved "<<meta<<std::endl;
            return StatusResult{keywordExpected};
        }
        int n_cells;
        aReader>>meta>>rowNumber>>meta>>n_cells;
        Cell it;
        for(int i=0;i<n_cells;i++){ 
            it.decode(aReader);
            this->cells.push_back(it);
        }
        aReader >>meta;
        if(meta!="##END##"){std::cout<<"possible error";}//say some error};

        return StatusResult();
    }

    //==========================================================================================
    //==========================================================================================
    //==========================================================================================
    //==========================================================================================
    //==========================================================================================
    //==========================================================================================
    //==========================================================================================
    //==========================================================================================
    //                                  CELL START
    //==========================================================================================
    //==========================================================================================
    //==========================================================================================
    //==========================================================================================
    //==========================================================================================
    //==========================================================================================
    //==========================================================================================
    //==========================================================================================

    // TODO JUST MAKE CELL HAVE ITS OWN CPP AND HPP FILES THIS IS GETTING OUT OF HAND.


    StatusResult   Cell::encode(std::ostream &aWriter){
        //for refrence VariantData = std::variant< bool,int, float,  std::time_t, std::string>;
        switch (this->index()) {  
            case 0: aWriter << "b" << " " <<  (isnull?"1":"0") << " " << std::get<0>(*this) ; break;
            case 1: aWriter << "i" << " " <<  (isnull?"1":"0") << " " << std::get<1>(*this) ; break;
            case 2: aWriter << "f" << " " <<  (isnull?"1":"0") << " " << std::get<2>(*this) ; break;
            case 3: aWriter << "t" << " " <<  (isnull?"1":"0") << " " << std::get<3>(*this) ; break;
            case 4:
                aWriter << "s" << " " <<  (isnull?"1":"0") << " " <<std::get<4>(*this).size();
                if(std::get<4>(*this).size()>0){
                    aWriter <<" "<< std::get<4>(*this).c_str()<<char(27); break;
                }else{
                    aWriter <<" "<< "NULL"<<char(27); break;
                }
        }
        aWriter << "|";
        return StatusResult(Errors::noError);
    }

    Cell&     Cell::decode(std::istream &aReader){
        std::string line;
        std::getline(aReader,line,'|');
        //std::cout<<"\nCELL:"<<line<<"endofline";
        std::stringstream ss(line);

        std::string meta_char;
        ss>>meta_char>>isnull;
            //not as generic as I would like but forcing into a temporary var enforces type to be recorded
            /// idk if there is a better certain way? probably read from the attributes?
        switch (meta_char[0]) {
            case 'b': 
                bool aBool;
                ss >> aBool ;
                *this = aBool; 
                
                break;
            case 'i': int anInt;
                ss >> anInt;
                *this= anInt; 
               
                break;
            case 'f': float aFloat;
                ss >> aFloat ;
                *this=aFloat; 
                break;
            case 't': std::time_t aTime;
                ss >> aTime ;
                *this=aTime; 
                break;
            //write string  and use alt+254 character as delim character
            //hacky??? ASK RICK DURING REVIEW
            case 's':
                int size;
                ss>>size;
                char trash [3];
                ss.read(trash,1);
                std::string theString;
                std::getline(ss,theString,char(27));
                //std::cout<<"\n in decode\nsize:"<<size<<" str:"<<theString<<"\n";
                *this=theString;
                break;
        }
        return *this;
    }


    Cell&  Cell::fromStr(std::string input,int index){
        switch(index){
            case 0: *this= (input[0]=='T'||input[0]=='t'?true:false);
            case 1: *this= std::stoi(input);
            case 2: *this= std::stof(input);
            case 3: *this= std::stoi(input);//solve later;
            default:return *this;
        }
        return *this;
    }

    Cell&  Cell::print(std::ostream &aWriter){
        switch (this->index()) {
            case 0: aWriter <<std::get<0>(*this) ; break;
            case 1: aWriter <<std::get<1>(*this) ; break;
            case 2: aWriter <<std::get<2>(*this) ; break;
            case 3: aWriter <<std::get<3>(*this) ; break;
            case 4: aWriter <<std::get<4>(*this) ; break;
        }
        return *this;
    }
    std::string  Cell::toStr(){
        std::stringstream ss;
        print(ss);
        std::string val;
        ss>>val;
        return val;
    }

    char Cell::type(){
        //for refrence VariantData = std::variant< bool,int, float,  std::time_t, std::string>;
        if(isnull){return 'N';}
        switch (this->index()) {
            case 0: return 'B';
            case 1: return 'I';
            case 2: return 'F';
            case 3: return 'D';
            case 4: return 'V';//possible bug later
        }
    }

    bool Cell::operator>(Cell& anotherCell){
        int ourtype=this->index();
        int theirtype=anotherCell.index();
        if(ourtype<3 && theirtype<3){
            switch(theirtype){
                case 0: return std::get<bool>(*this) > std::get<bool>(anotherCell);
                case 1: return std::get<int>(*this) > std::get<int>(anotherCell);
                case 2: return std::get<float>(*this) > std::get<float>(anotherCell);
                case 3: return std::get<3>(*this) > std::get<3>(anotherCell);
                case 4:return std::get<std::string>(*this).compare(std::get<std::string>(anotherCell))>0;
                default:return false;
            }   
        }
        return false;
    };
    bool Cell::operator==(Cell& anotherCell){
         int ourtype=this->index();
        int theirtype=anotherCell.index();
         if(ourtype<3 && theirtype<3){
            switch(theirtype){
                case 0: return std::get<bool>(*this) == std::get<bool>(anotherCell);
                case 1: return std::get<int>(*this) == std::get<int>(anotherCell);
                case 2: return std::get<float>(*this) == std::get<float>(anotherCell);
                case 3: return std::get<3>(*this) == std::get<3>(anotherCell);
                case 4: {
                    int isequal= std::get<std::string>(*this).compare(std::get<std::string>(anotherCell));
                    return  isequal  ==  0;
                }
                default: return false;
            }   
        }
        return false;
    };
    bool Cell::operator<(Cell& anotherCell){
        return !(*this>anotherCell || *this==anotherCell);
    };

}
