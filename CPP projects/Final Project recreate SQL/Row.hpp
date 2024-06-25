//
//  Row.hpp
//  Assignment4
//
//  Created by rick gessner on 4/19/20.
//  Copyright © 2020 rick gessner. All rights reserved.
//
#ifndef ECE141ROW
#define ECE141ROW

#include <stdio.h>
#include <string>
#include <utility>
#include <variant>
#include <vector>
#include "Storage.hpp"
#include "Attribute.hpp"


#include <memory>


class Database;

namespace ECE141 {

  using VariantData = std::variant< bool,int, float, std::time_t, std::string>;
  class Row;
  using RowCollection = std::vector<Row>;


  class Cell : public VariantData{
  public:
    Cell():isnull(false),VariantData(){};
    using VariantData::VariantData;//SEXY EXTENSION STUFF
    bool  isnull; 
    char  type();
    StatusResult        encode(std::ostream &aWriter);
    Cell&               print(std::ostream &aWriter);
    Cell&               decode(std::istream &aReader);
    std::string         toStr();
    Cell&               fromStr(std::string input,int index);

    bool operator>(Cell& anotherCell);
    bool operator==(Cell& anotherCell);
    bool operator<(Cell& anotherCell);
  };

  class Row : public Storable {
  public:
    Row(){};
    ~Row(){};
    StatusResult        encode(std::ostream &aWriter) override;
    StatusResult        decode(std::istream &aReader) override;
    Block               Blockify(std::string name, int count, int pos);
    bool                Deblockify(Block& aBlock);

    std::vector<Cell>  cells;
    uint32_t            rowNumber;
  };
}

#endif /* Row_hpp */