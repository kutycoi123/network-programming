#include "server_handler.hpp"

// Read a request from a client.
// Once the request arrives, process the request, and reply back to the client with the response.
void 
net::OnNewClientConnected(net::Server* server, uint16_t fd, void* data) 
{
    model::SetManager* sm = (model::SetManager*) data;

    cout << "[SERVER] New Connection..." << endl;
    char* requestBytes = new char[MAX_BUFFER_SIZE];
    // TODO: Receive request here
    int readBytes = ReadBytes(fd, requestBytes, MAX_BUFFER_SIZE);
    
    cout << "[SERVER] " << readBytes << " bytes were received" << endl;

    model::Request* req = new model::Request(requestBytes);
    model::Response* res = sm->HandleRequest(req);

    // TODO: Send response here
    char* response = res->toBytes();
    uint16_t sentBytes = SendBytes(fd, response, res->getBytesCount()); 
    
    cout << "[SERVER] " << sentBytes << " bytes were sent" << endl;

    delete[] response; 
    delete[] requestBytes;
    if (req)
      delete req;
    if (res)
      delete res;
}
