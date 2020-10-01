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
    //std::string msg_str(msg);
    _ret = msg[0];
    char len[4];
    char uuid[32];
    char item[4];
    char size[4];
    memcpy(len, msg + 1, 4);
    //_len = utils::bytesToInt(msg_str.substr(1, 4).c_str());
    _len = utils::bytesToInt(len);
    if (_ret != RET_SUCCESS || _opt == OPT_ADD_ITEM || _opt == OPT_ADD_ITEM
        || _opt == OPT_CLR_SET || _opt == OPT_REM_SET) 
      return;
    //int len = msg_str.length();
    int totalBytes = _len * 4;
    if (_opt == OPT_CRT_SET || _opt == OPT_GET_SETS) {
      for (int i = 5; i < totalBytes; i+=32) {
         //_uuids.push_back(msg_str.substr(i, 32)); 
         memcpy(uuid, msg+i, 32);
         _uuids.push_back(std::string(uuid));
      }
    }else if (_opt == OPT_GET_ITEMS){
      for (int i = 5; i < totalBytes; i+=4) {
         //_items.push_back(utils::bytesToInt(msg_str.substr(i, 4).c_str()));
         memcpy(item, msg+i, 4);
         _items.push_back(utils::bytesToInt(item));
      }
    }else if (_opt == OPT_GET_SIZE) {
      //_set_size = utils::bytesToInt(msg_str.substr(5).c_str());
      memcpy(size, msg + 5, 4);
      _set_size = utils::bytesToInt(size);
    }
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
    int count = 5;
    count += _uuids.size() * 32;
    count += _items.size() * 4; 
    if (_opt == OPT_GET_SIZE ) 
      count += 4;
    return count;
}

// Encode the Response object to byte array (based on its content)
char* 
model::Response::toBytes() 
{
    int bytesCount = getBytesCount();
    char* msg = new char[bytesCount];
    
    // TODO: Complete
    msg[0] = _ret;
    char* _lenInBytes = utils::intToBytes(getLen());
    //strcpy(msg+i, _lenInBytes);
    memcpy(msg+1, _lenInBytes, 4);
    int i = 5;
    for (const std::string& id : _uuids) {
      const char* uuidCString = id.c_str();
      //strcpy(msg+i, uuidCString);
      //i += strlen(uuidCString);
      memcpy(msg+i, uuidCString, 32);
      i += 32;
    }
    for (const int& item : _items) {
      char* itemInBytes = utils::intToBytes(item); 
      //strcpy(msg+i, itemInBytes);
      //i += strlen(itemInBytes);
      memcpy(msg+i, itemInBytes, 4);
      i += 4;
      delete[] itemInBytes;
    }
    if (_opt == OPT_GET_SIZE) {
      char* _setSizeInBytes = utils::intToBytes(_set_size);
      //strcpy(msg+i, _setSizeInBytes);
      memcpy(msg+i, _setSizeInBytes, 4);
      delete[] _setSizeInBytes;
    }
    delete[] _lenInBytes;
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
    //int bytesCount = 0;
    //int wordCount = 0;

    //return wordCount;
    //return _len;
    int bytesCount = 0;
    bytesCount += _uuids.size() * 32;
    bytesCount += _items.size() * 4;
    if (_opt == OPT_GET_SIZE) 
       bytesCount += 4;
    return bytesCount / 4;
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
