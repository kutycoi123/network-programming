Name: TRUNGLAM NGUYEN
Email: tln3@sfu.ca

1. How to run the code:
   + Build source code:
      javac rdt/*.java
   + Run server:
      java rdt/TestServer 127.0.0.1 <client_port> <server_port>
   + Run client:
      java rdt/TestClient 127.0.0.1 <server_port> <client_port>
   + Note: You should open two terminals, one is to run server, the another is to run client.
     When client terminates, you can just run another client, no need to restart server. 
2. Challenges and issues
   + One of the main challenge part for this assignment is to implement a proper close process for
   both client and server. My implementation for close method is described below:
   + Phase 1: close() method is invoked. 
      Step 1: Client sends a segment and set the flag to be 2 (RDTSegment.FLAGS_CLIENT_SHUTDOWN).
      Step 2: Client put this segment into send buffer(sndBuf) and start a timer for this segment.
              This segment is treated like normal segment but sets flag to be 2
      Step 3: Server receives the client shutdown segment, put this segment into receiver buffer (rcvBuf)
      Step 4: Server then sends acknowledgement to client
   + Phase 2: Client finishes sending all the data, ready to close the connection
      Step 1: Once all the segments in send buffer are acknowledged (including the shutdown segment), 
              client will be ready to close the connection.
              Client close the socket, force the main thread and receiver thread to terminate
      Step 2: After receiving all the sent segments, server will be ready to reset the receiver buffer.
              Server reset the receriver buffer and ready for new client connection.
   + Above implementation allows client to shutdown properly and server to reset and ready for new connection, without having to
     restart the server.
