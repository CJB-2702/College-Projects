//
//  Player.cpp
//  Checkers
//
//  Created by rick gessner on 2/22/19.
//  Copyright © 2019 rick gessner. All rights reserved.
//

#include "Bissett.hpp"
#include "Game.hpp"
#include <vector>

namespace ECE141 {
  int selectIndex(const std::vector<float> &theScores);
  int Player::pcount = 0; //init our static member to track # of players...
  
  static PieceColor autoColor() { //auto assigns a color
    return (++Player::pcount % 2) ? PieceColor::blue : PieceColor::gold;
  }
  
  Player::Player() : color(autoColor()) {}
 
  bool Bissett::takeTurn(Game &aGame, Orientation aDirection, std::ostream &aLog) {
    std::vector<float> theScores;
    float aScore=-100;
    std::vector<Location> bestLocations;
    Location aLocation(0,0);
    //lmao guys really why is the orientation the same for both pieces
    if(aDirection==Orientation::north && color==PieceColor::gold){aDirection=Orientation::south;}

    size_t theCount=aGame.countAvailablePieces(color);
    if(theCount==0){return false;}
    for(int pos=0;pos<theCount;pos++) {
       if(const Piece *thePiece = aGame.getAvailablePiece(this->color, pos)) {
        aScore=getCurrentScore(*thePiece,aGame,aDirection);
        theScores.push_back(aScore);
        bestLocations.push_back(playerFocus);
       }
    }
    //make the move
    int bestOption=selectIndex(theScores);
    const Piece *thePiece = aGame.getAvailablePiece(this->color, bestOption);
    if(canAttack(*thePiece,aGame,aDirection)){
      while(canAttack(*thePiece,aGame,aDirection)){
        aGame.movePieceTo(*thePiece, playerFocus);
      }
      return true;
    }else{
      return aGame.movePieceTo(*thePiece, bestLocations[bestOption]);
    }
    return false;
  }

  int selectIndex(const std::vector<float> &theScores){
    int i=0, maxIndx=0;
    float lastMax=-100;
    for(auto it:theScores){
      if(it>lastMax){
        maxIndx=i;
        lastMax=it;
      }
      i++;
    }
    return maxIndx;
  }

  float Bissett::getCurrentScore(Piece thePiece, Game &aGame, Orientation aDirection){
    //if there is an attack available it must attack.
    if(canAttack(thePiece, aGame, aDirection)){return 10;}
    if(isInDanger(thePiece, aGame, aDirection)){
     if(canMove(thePiece, aGame, aDirection)){return 5;}
    }
    if(canCover(thePiece, aGame, aDirection)){return 3;}
    if(isCovered(thePiece, aGame, aDirection)){return -1;}
    if(canMove(thePiece, aGame, aDirection)){
      if(thePiece.getKind()==PieceKind::king){
        return 1;
      }else{return 0;}
    }
    return -2;
  }




















  bool Bissett::canAttack(Piece thePiece, Game &aGame, Orientation aDirection){
    int fwd=1,dirx=1;
    if(aDirection==Orientation::north){fwd=-1;}
    //get location to go to?
    if(isAttackDir(thePiece,aGame, fwd,dirx)){return true;}
    if(isAttackDir(thePiece,aGame, fwd,-dirx)){return true;}
    if(thePiece.getKind()==PieceKind::king){
      if(isAttackDir(thePiece,aGame, -fwd,dirx)){return true;}
      if(isAttackDir(thePiece,aGame, -fwd,-dirx)){return true;}
    }
    return false;
  }

  bool Bissett::isAttackDir(Piece thePiece, Game &aGame,int fwd, int dirx){
    Location current=thePiece.getLocation();
    Location temp(current.row+fwd,current.col+dirx);
    if(aGame.getTileAt(temp)!=nullptr){
      if(aGame.getTileAt(temp)->getPiece()!=nullptr){
        Piece tempPiece=*(aGame.getTileAt(temp)->getPiece());
        //if the piece is next to a piece of the opposite type,
        if(tempPiece.getColor()!=thePiece.getColor()){
          //check the location on the other side of it
          temp.row=temp.row+fwd;
          temp.col=temp.col+dirx;
          if(aGame.getTileAt(temp)!=nullptr){
            //if its an empty tile that means you can attack
            if(aGame.getTileAt(temp)->getPiece()==nullptr){
              //remember that location and return true
              playerFocus=temp;
              return true;
    } } } } } 
    return false;
  }

  bool Bissett::isInDanger(Piece thePiece, Game &aGame, Orientation aDirection){
    Location current=thePiece.getLocation();
    PieceKind theKind=thePiece.getKind();
    int fwd=1,dirx=1;
    if(aDirection==Orientation::north){fwd=-1;}
    if(0 < dangerDir( thePiece, aGame,  fwd, dirx)){return true;}
    if(0 < dangerDir( thePiece, aGame,  fwd, -dirx)){return true;}
    return false;
  }

  bool Bissett::dangerDir(Piece thePiece, Game &aGame, int fwd, int dirx){
    //check if opponent is in direction and if in danger
    Location current=thePiece.getLocation();
    Location temp(current.row+fwd,current.col+dirx);
    if(aGame.getTileAt(temp)!=nullptr){
      if(aGame.getTileAt(temp)->getPiece()!=nullptr){
        Piece tempPiece=*(aGame.getTileAt(temp)->getPiece());
        if(tempPiece.getColor()!=thePiece.getColor()){
          //then check behind self
          temp.row=current.row-fwd;
          temp.col=current.col-dirx;
          if(aGame.getTileAt(temp)!=nullptr){
            if(aGame.getTileAt(temp)->getPiece()==nullptr){
              return true;
    } } } } }
    return false;
  }

  bool Bissett::isColorSameAt(Game &aGame, PieceColor aColor, Location aLoc){
    if(aGame.getTileAt(aLoc)!=nullptr){
      if(aGame.getTileAt(aLoc)->getPiece()!=nullptr){
        if(aGame.getTileAt(aLoc)->getPiece()->getColor()==aColor){
          return true;
    } } }
    return false;
  }

  bool Bissett::canCover(Piece thePiece, Game &aGame, Orientation aDirection){
    Location current=thePiece.getLocation();
    int fwd=1,dirx=1;
    if(aDirection==Orientation::north){fwd=-1;}
    bool result=false;
    if(isMoveInDir(thePiece, aGame,fwd,dirx)){
      Location temp(current.row+fwd,current.col+dirx);
      playerFocus=temp;
      //check for friendly pieces at next location,
      temp.col=current.col;
      temp.row=current.row+2*fwd;
      if(isColorSameAt(aGame, thePiece.getColor(), temp)){return true;}
      temp.col=temp.col+2*dirx;
      if(isColorSameAt(aGame, thePiece.getColor(), temp)){return true;}
    }
    if(isMoveInDir(thePiece, aGame,fwd,-dirx)){
      Location temp(current.row+fwd,current.col-dirx);
      playerFocus=temp;
      //check for friendly peices ahead
      temp.col=current.col;
      temp.row=current.row+2*fwd;
      if(isColorSameAt(aGame, thePiece.getColor(), temp)){return true;}
      temp.col=temp.col-2*dirx;
      if(isColorSameAt(aGame, thePiece.getColor(), temp)){return true;}
    }
    return false;
  }

  bool Bissett::isCovered(Piece thePiece, Game &aGame, Orientation aDirection){
    if(aDirection==Orientation::north){
      return canMove(thePiece, aGame, Orientation::south);
    }
    return false;
  }
  
  bool Bissett::canMove(Piece thePiece, Game &aGame, Orientation aDirection){
    int fwd=1,dirx=1;
    Location temp(playerFocus);
    if(aDirection==Orientation::north){fwd=-1;}
    bool result=false;
    if(isMoveInDir(thePiece, aGame,fwd,dirx)){    temp=playerFocus; result=true;}
    if(isMoveInDir(thePiece, aGame,fwd,-dirx)){   temp=playerFocus; result=true;}
    if(thePiece.getKind()==PieceKind::king){
      if(isMoveInDir(thePiece, aGame,-fwd,dirx)){ temp=playerFocus; result=true;}
      if(isMoveInDir(thePiece, aGame,-fwd,-dirx)){temp=playerFocus; result=true;}
    }
    playerFocus=temp;
    return result;
  }

  bool Bissett::isMoveInDir(Piece thePiece, Game &aGame,int fwd, int dirx){
    Location current=thePiece.getLocation();
    Location temp(current.row+fwd,current.col+dirx);
    if(aGame.getTileAt(temp)!=nullptr){
      if(aGame.getTileAt(temp)->getPiece()==nullptr){
        playerFocus=temp;
        return true;
      }
    }
    return false;
  }

}
  

