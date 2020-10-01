#include "client.hpp"

net::Client::Client(const char* server_addr, void* data)
{
    setup(server_addr, DEFAULT_PORT, data);
}

net::Client::Client(const char* server_addr, int port, void* data)
{
    setup(server_addr, port, data);
}

net::Client::Client(const Client& orig)
{
}

net::Client::~Client()
{
	std::cout << "[CLIENT] [DESTRUCTOR] Destroying Client...\n";
	close(_connFd);
}

// Create a socket and sets the server address
// Don't modify _data
void 
net::Client::setup(const char* server_addr, int port, void* data)
{
    _data = data;
    // TODO: Complete
   _servAddr.sin_family = AF_INET;
   _servAddr.sin_port = htons(port);
   if (inet_pton(AF_INET, server_addr, &_servAddr.sin_addr) <= 0) {
      perror("Invalid addres\n");
      shutdown();
   }

}

void 
net::Client::initializeSocket()
{
	std::cout << "[CLIENT] initializing socket\n";
  if ((_connFd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
      perror("Socket creation failed \n");
      shutdown();
  }
}

// Shutdown the socket
void 
net::Client::shutdown()
{
	// TODO: Complete
  close(_connFd);
}

// Connect to the server. You need to call "newConnectionCallback" once the connection is established
void 
net::Client::doConnect()
{
    // TODO: Complete
    if (connect(_connFd, (struct sockaddr *)&_servAddr, sizeof(_servAddr)) < 0) {
        perror("Connection failed\n");
    }
    newConnectionCallback(this, _connFd, _data);
}

void 
net::Client::init()
{
    initializeSocket();
    doConnect();
}

void 
net::Client::onConnect(void(*ncc)(Client*, uint16_t, void*))
{
    newConnectionCallback = ncc;
}
