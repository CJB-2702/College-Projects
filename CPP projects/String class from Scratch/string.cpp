#include "String.hpp"

namespace ECE141{
    String& String::insert(size_t anIndex, const String &aString, size_t aMaxCopyLen){
        String temp(aString);
        temp.trim(aMaxCopyLen);
        temp+=getBackSlice(anIndex);
        trim(anIndex);
        *this+=temp;
        return *this;
    }
    String& String::insert(size_t anIndex, const char* aString, size_t aMaxCopyLen){
        String temp(aString,std::strlen(aString));
        insert(anIndex,temp,aMaxCopyLen);
        return *this;
    }
    String& String::replace(size_t anIndex, size_t aMaxCopyLen, const String &aSubString){
        String temp(aSubString);
        temp.trim(aMaxCopyLen+1);
        temp+=getBackSlice(anIndex+aMaxCopyLen);
        trim(anIndex);
        *this+=temp;
        return *this;
    }
    String&  String::replace(size_t anIndex, size_t aMaxCopyLen, const char *aSubString){
        String temp(aSubString,std::strlen(aSubString));
        replace(anIndex, aMaxCopyLen, temp);
        return *this;
    }

    String& String::erase(size_t anIndex, size_t aCount){
        //replacing with empty string is same as erasing
        String temp;
        replace(anIndex,aCount,temp);
        return *this;
    }

    int String::compare(const String& aString )const {
      if(length>0 && aString.length>0){
        return(std::strcmp(pBuffer,aString.pBuffer));
      }else if(length>0 && aString.length==0){
        return 1;
      }else if(length==0 && aString.length>0){
        return -1;
      }
      return 0;
    }//[-1 if <, 0 if ==, 1 if >]
}
//*/