//
//  Player.hpp
//  Checkers
//
//  Created by rick gessner on 2/22/19.
//  Copyright © 2019 rick gessner. All rights reserved.
//

#ifndef Player_hpp
#define Player_hpp

#include "Piece.hpp"
#include <iostream>

namespace ECE141 {
  
  class Game; //forward declare...  
  
  class Player {
  public:
                      Player();
    virtual bool      takeTurn(Game &aGame, Orientation aDirection, std::ostream &aLog){return false;};
    const PieceColor  color;
    static int        pcount; //how many created so far?
  };

  class Bissett : public Player{
    public:
    Bissett(): Player(){};
  

    virtual bool   takeTurn(Game &aGame, Orientation aDirection, std::ostream &aLog);

    //*all of these functions are just to get the score. should probably make a seperate object.
    float getCurrentScore (Piece thePiece, Game &aGame, Orientation aDirection);
    bool canAttack        (Piece thePiece, Game &aGame, Orientation aDirection);
      bool isAttackDir    (Piece thePiece, Game &aGame, int fwd, int dirx);
    bool isInDanger       (Piece thePiece, Game &aGame, Orientation aDirection);
      bool dangerDir       (Piece thePiece, Game &aGame, int fwd, int dirx);

    bool canCover         (Piece thePiece, Game &aGame, Orientation aDirection);
    bool isCovered        (Piece thePiece, Game &aGame, Orientation aDirection);
    bool canMove          (Piece thePiece, Game &aGame, Orientation aDirection);
      bool isMoveInDir    (Piece thePiece, Game &aGame,int fwd , int dirx);
    
    bool isColorSameAt  (Game &aGame, PieceColor aColor, Location aLoc);
    protected:
    Location playerFocus;//while functions execute the player focuses on different locations.
  };
}

#endif /* Player_hpp */
