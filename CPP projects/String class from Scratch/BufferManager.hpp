#include <exception>
#include <iostream>
namespace ECE141 {
template <typename T>
class BufferManager{
    protected:
    T* pBuffer;
    size_t length;//length of data in buffer
    size_t bufferSize;
    // init
    public:
    BufferManager():pBuffer(nullptr),length(0),bufferSize(0){};
    BufferManager(const T* aBuffer,size_t dataLength=0, size_t aPreSize=0):BufferManager(){
        length = dataLength;
        // if the buffer size requested is shorter than the data size
        // default to creating enough buffer for the data
        if(dataLength > aPreSize && dataLength>0){
            bufferSize= bufferSizeProtocol(dataLength);
            pBuffer= new T[bufferSize];
            this->copyFromToFor(aBuffer,pBuffer,length);
        }else{
            //if the buffer requested is bigger than the data in the buffer
            if(dataLength+1>0){
                bufferSize=aPreSize;
                pBuffer= new T[aPreSize];
                this->copyFromToFor(aBuffer,pBuffer,length);
            }else{//if no data in buffer
                this->clear();
            }
        }
    };
    BufferManager(const BufferManager &aBufferManager, size_t aPresize=0): BufferManager(aBufferManager.pBuffer,aBufferManager.length,aPresize) {};
    ~BufferManager(){
        delete [] pBuffer;
    };
    //accessors...
    size_t size() const{return length;};
    size_t getBufferSize()const{return bufferSize;}; 
    T&  operator[](int pos){
        if(pos>=int(length) || pos<0){std::cerr<<"\nERROR: OUT OF RANGE\n";}
        return pBuffer[pos];
        //CB: come back to this by 1/20/20. I think RG requested an exception be thrownspecifically
    };
    void clear(){
        delete [] pBuffer;
        pBuffer=nullptr;
        bufferSize=0;
        length=0;
        return;
    };
    protected:
    BufferManager& operator+=(const BufferManager &aBufferManager){
        //empty buffer case
        if(aBufferManager.pBuffer==nullptr || aBufferManager.length==0){return *this;}
        size_t totalLen=length+aBufferManager.length;
        
        if(totalLen>bufferSize){//if the new length isn't gonna fit in the current buffer
            //allocate new buffer
            size_t newBufferSize=bufferSizeProtocol(totalLen);
            T* ptemp=new T [newBufferSize];
            //copy data to buffer
            this->copyFromToFor(pBuffer,ptemp,length);
            this->copyFromToFor(aBufferManager.pBuffer,ptemp+length,aBufferManager.length);
            //delete old buffer
            delete [] pBuffer;
            //update information
            pBuffer=ptemp;
            length=totalLen;
            bufferSize=newBufferSize;
        }else{
            //copy data
            this->copyFromToFor(aBufferManager.pBuffer,pBuffer+length,aBufferManager.length);
            //update information
            length=totalLen;
        }
        return *this;
    };
    BufferManager& operator=(const BufferManager &aBufferManager){
        length=0;//"delete" current data
        //+= deals with buffer management
        *this+=aBufferManager;
        return *this;
    };
    BufferManager operator+(const BufferManager &aBufferManager){
        BufferManager output(*this);
        output+=aBufferManager;
        return output;
    };
 
    BufferManager& trim(size_t anIndex){
        if(anIndex==0){
            this->clear();
        }else if(anIndex<length){
            length=anIndex;
        }
        //todo add buffer resizing protocols;
        return *this;
    };
    BufferManager getBackSlice(size_t index){
        //make a buffer from the adjusted pointer and adjusted length
        if(index>=length){//throw an error
            //std::cerr<< std::out_of_range;
            BufferManager temp;
            temp.clear();
            return temp;
        }else{
            BufferManager temp(pBuffer+index,length-index);
            return temp;
        }
    };
    size_t bufferSizeProtocol(size_t minRequired){
        if(minRequired>0){
            size_t bufferLen=16;
            while(bufferLen<(minRequired+1)){bufferLen*=4;}
            return bufferLen;
        }else{
            return 0;
            //CB: error checking
        }      
    };
    void copyFromToFor(const T* pFrom,  T* pTo, size_t len){
        for(size_t i=0;i<len;i++){
            *(pTo+i)=*(pFrom+i);
        }
    };
};
}