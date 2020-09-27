#include "response.hpp"

model::Response::Response(char opt, char ret)
:_opt(opt), _ret(ret)
{  

}

// Decode the byte array to Response members
model::Response::Response(char opt, const char* msg)
:_opt(opt)
{
    // TODO: Complete
}

model::Response::Response(const Response& orig)
{

}

model::Response::~Response()
{

}

// Return the number of bytes of the Response
int
model::Response::getBytesCount() 
{
    // TODO: Complete
    return 0;
}

// Encode the Response object to byte array (based on its content)
char* 
model::Response::toBytes() 
{
    int bytesCount = getBytesCount();
    char* msg = new char[bytesCount];
    
    // TODO: Complete
    
    return msg;    
}

const char 
model::Response::getOpt() 
{
    return _opt;
}

const char 
model::Response::getRet() 
{
    return _ret;
}

// Return the number of 4-byte words of the Response body
int 
model::Response::getLen()
{
    if(_ret != RET_SUCCESS) {
        return 0;
    }

    // TODO: Complete
    int bytesCount = 0;
    int wordCount = 0;

    return wordCount;
}

void
model::Response::addItem(int item) 
{
    _items.push_back(item);
}

void 
model::Response::addUuid(std::string uuid)
{
    _uuids.push_back(uuid);
}

void 
model::Response::setSetSize(int size)
{
    _set_size = size;
}

const std::vector<int>
model::Response::getItems() 
{
    return _items;
}

const std::vector<std::string>
model::Response::getUuids() 
{
    return _uuids;
}

int 
model::Response::getSetSize()
{
    return _set_size;
}
