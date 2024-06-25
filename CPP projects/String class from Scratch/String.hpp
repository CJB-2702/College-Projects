


#include "BufferManager.hpp"
#include <iostream>
#include <cstring>

//mutation methods...====30
//Mutations with names2==50
//Comparision methods..==60
//Misc ==================80

namespace ECE141{
class String: public BufferManager<char>{
    /* members from buffer manager
    protected:
    T* pBuffer;
    size_t length;//length of data in buffer
    size_t bufferSize;
    */
    public:
    String():BufferManager(){};
    //basically normal buffer manager initialisation but with null character
    String(const char* aBuffer, size_t aPresize=0):BufferManager(aBuffer,std::strlen(aBuffer),aPresize){addNullCharacter();};
    String(const String &aString, size_t aPresize=0):BufferManager(aString.pBuffer,aString.length,aPresize){addNullCharacter();};
    String(const BufferManager &aBM):BufferManager(aBM){addNullCharacter();};//placed for polymorphism. possibly slow?
    //mutation methods...====================================================30
    virtual BufferManager& operator=(const String &aCopy){
      BufferManager::operator=(aCopy);//do buffer manager version of function
      addNullCharacter();
      return *this;//buffermanager pointer=String pointer
    };
    virtual BufferManager& operator+=(const String &aString){
      BufferManager::operator+=(aString);
      addNullCharacter();
      return *this;
    };
    String  operator+(const String &aString){
      String temp(*this);
      temp+=aString;
      return temp;
    };
    String  operator+(const char* aBuffer){
      String temp(aBuffer,std::strlen(aBuffer));//throw char* into string then do string version
      return *this+temp;
    };
    String& operator+=(const char* aBuffer){
      String temp(aBuffer,std::strlen(aBuffer));
      *this+=temp;
      return *this;
    };
    String& operator=(const char* aBuffer){
      String temp(aBuffer,std::strlen(aBuffer));
      *this=temp;
      return *this;
    };

    //Mutations with names====================================================50
    String& insert(size_t anIndex, const String &aString, size_t aMaxCopyLen);
    String& insert(size_t anIndex, const char* aString, size_t aMaxCopyLen);  

    String& replace(size_t anIndex, size_t aMaxCopyLen, const String &aSubString);
    String& replace(size_t anIndex, size_t aMaxCopyLen, const char *aSubString);  

    String& erase(size_t anIndex, size_t aCount); //from given position, remove N chars...("cut")
    ///

    //Comparision methods..===================================================60
    int compare(const String& aString )const;//[-1 if <, 0 if ==, 1 if >]
    bool operator==(String &aString){return compare(aString)==0;};
    bool operator<(String &aString){return compare(aString)<0;};
    bool operator==(const char* aBuffer) {
      String temp(aBuffer,std::strlen(aBuffer));
      return (*this)==temp;
    };
    bool operator<(const char* aBuffer){
      String temp(aBuffer,std::strlen(aBuffer));
      return (*this)<temp;
    };//*/ 

    //Misc  =================================================================80
      //Search...
    int find( const String &aSubString, size_t anOffset = 0 )const{
      if(length>0 && aSubString.length>0){return(std::strcmp(pBuffer,aSubString.pBuffer));}
      return -1;//else throw error
    };   
      //stream...
    friend std::ostream& operator << (std::ostream &out, String &aStr){
      for(size_t i=0;i<aStr.length;i++){out << aStr[i];}
      return out;
    };
    friend std::istream& operator >> (std::istream &in,String &aStr){
      char streambuff[1000];
      in.get(streambuff,999);
      aStr=streambuff;
      return in;
    };
    ///
    
       
    private:
    String& addNullCharacter(){
      //buffer must be designed to always have one extra byte
      //better fix just make size()=length-1 and have '\0' part of data
      if (length>0){ *(pBuffer+length)='\0';}
      return *this;
    };
};
}
