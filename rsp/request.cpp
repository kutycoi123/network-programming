#include "request.hpp"

model::Request::Request(char opt)
{  
    setupFlags(opt);
    setup(opt, "", 0);
}

model::Request::Request(char opt, std::string uuid)
{  
    setupFlags(opt);
    setup(opt, uuid, 0);
}

model::Request::Request(char opt, std::string uuid, int item)
{  
    setupFlags(opt);
    setup(opt, uuid, item);
}

// Decode the byte array to Request members (opt, uuid and item)
model::Request::Request(const char* msg)
{
    // TODO: Set the correct opt first
    char opt = 0;
    opt = msg[0];
    setupFlags(opt);
    
    // TODO: Complete
    char uuid_raw[32];
    char item_raw[4];
    std::string uuid;
    int item = 0;
    if (_has_uuid) {
      memcpy(uuid_raw, msg+1, 32);
      uuid = std::string(uuid_raw);
      if (_has_item) {
        //item = utils::bytesToInt(msg_str.substr(33, 4).c_str());
        memcpy(item_raw, msg+33, 4);
        item = utils::bytesToInt(item_raw);
      }
    }
    setup(opt, uuid, item);
}

void 
model::Request::setup(char opt, std::string uuid, int item)
{
    _opt = opt;
    _uuid = uuid;
    _item = item;
}

void 
model::Request::setupFlags(char opt)
{
    if(opt == OPT_CRT_SET || opt == OPT_GET_SETS) {
        _has_item = false;
        _has_uuid = false;
    } else {
        _has_item = false;
        _has_uuid = true;
        
        if (opt == OPT_ADD_ITEM || opt == OPT_REM_ITEM) {
            _has_item = true;
        }
    }
}

model::Request::Request(const Request& orig)
{
}

model::Request::~Request()
{
}

const char 
model::Request::getOpt() 
{
    return _opt;
}

std::string 
model::Request::getUuid() 
{
    return _uuid;
}

int 
model::Request::getItem() 
{
    return _item;
}

bool 
model::Request::hasUuid() 
{
    return _has_uuid;
}

bool 
model::Request::hasItem() 
{
    return _has_item;

}

// Return the number of bytes of the Request
int
model::Request::getBytesCount() 
{
    // TODO: Complete
    int sz = 1;
    if (_has_uuid) sz += 32;
    if (_has_item) sz += 4;
    return sz;
}

// Encode the Request object to byte array (based on its content)
char* 
model::Request::toBytes() 
{
    int bytesCount = getBytesCount();
    char* msg = new char[bytesCount];
    
    // TODO: Complete
    msg[0] = _opt;
    if (_has_uuid) {
      for (int i = 1; i <= 32; ++i) {
        msg[i] = _uuid[i-1];
      }
    }
    if (_has_item) {
      char* itemInBytes = utils::intToBytes(_item);
      for (int i = 0; i < 4; ++i) {
        msg[i+33] = itemInBytes[i];
      }
      delete[] itemInBytes;
    }
    return msg;    
}
