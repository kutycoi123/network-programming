#include "client_handler.hpp"

void
net::printResponse(model::Response* res) {
    if(res->getRet() == RET_SUCCESS) {
        cout << "[CLIENT] SUCCESS" << endl;
        if(res->getOpt() == OPT_CRT_SET) {
            std::string uuid = res->getUuids()[0];
            cout << "[CLIENT] New set ID " << uuid << endl;
        } else if(res->getOpt() == OPT_GET_SETS) {
            cout << "[CLIENT] Existing Set IDs:" << endl;
            for(auto const& uuid : res->getUuids()) {
                cout << "[CLIENT] \t" << uuid << endl;
            }
        } else if(res->getOpt() == OPT_GET_ITEMS) {
            cout << "[CLIENT] Items:" << endl;
            for(auto const& item : res->getItems()) {
                cout << "[CLIENT] \t" << item << endl;
            }
        } else if(res->getOpt() == OPT_GET_SIZE) {
            cout << "[CLIENT] Set size is : " << res->getSetSize() << endl;
        }
    } else if (res->getRet() == RET_SET_NOT_EXIST) {
        cout << "[CLIENT] SET DOES NOT EXIST" << endl;
    } else if (res->getRet() == RET_ITEM_NOT_EXIST) {
        cout << "[CLIENT] ITEM DOES NOT EXIST" << endl;
    } else if (res->getRet() == RET_ITEM_ALREADY_EXIST) {
        cout << "[CLIENT] ITEM ALREADY EXISTS" << endl;
    } else {
        cout << "[CLIENT] Non-supported return type" << endl;
    }
}

// Read the response from the socket and construct the bytes based on response size.
// Return nullptr if there is an error.
char* 
net::readResponseBytes(uint16_t fd) 
{
    // TODO: Complete
    return nullptr;
}

// Send the request to the server and wait for a response.
// Once the response arrives, read the response bytes and pass them to a Response object.
void 
net::OnClientConnected(net::Client* client, uint16_t fd, void* data) 
{
    cout << "[CLIENT] Connected" << endl;
    
    model::Request* req = (model::Request*) data;
    // TODO: Send request here
    uint16_t sentBytes = 0;
    
    cout << "[CLIENT] " << sentBytes << " bytes were sent" << endl;
    
    char* responseBytes = readResponseBytes(fd);
    model::Response* res = new model::Response(req->getOpt(), responseBytes);
    printResponse(res);
}