Name: TRUNGLAM NGUYEN
Email: tln3@sfu.ca

1. How to run the code:
   + Build source code:
       javac rdt/*.java
   + Run server:
       java rdt/TestServer 127.0.0.1 <client_port> <server_port>
   + Run client:
       java rdt/TestClient 127.0.0.1 <server_port> <client_port>
   or: java rdt/TestClientWithLongData 127.0.0.1 <server_port> <client_port>
   or: java rdt/TestClientWithMoreBufferSize 127.0.0.1 <server_port> <client_port>
   + Note: 
    1. You should open two terminals, one is to run server, the another is to run client.
    2. When client terminates, server will automatically reset and ready to accept new connection.
       Therefore, you can just run another client, no need to restart server. 

2. Challenges and issues
   + One of the main challenge part for this assignment is to implement a proper close process for
   both client and server. My implementation for close method is described below:
   + Phase 1: close() method is invoked. 
      Step 1: Client create a segment with flag as 2 (RDT.FLAGS_CLIENT_SHUTDOWN),
              then put this segment into send buffer(sndBuf). This segment is treated like normal segment.
      Step 2: Client waits for a short time(number of segments * RTT) before sending a shutdown segment.
              Then client sets a timer for this segment.
      Step 3: Server receives the client shutdown segment, put this segment into receiver buffer (rcvBuf)
      Step 4: Server then sends acknowledgement to client

   + Phase 2: Client finishes sending all the data and server have received all data
      Step 1: Once all the segments in send buffer are acknowledged (including the shutdown segment), 
              client closes the socket, forces the main thread and receiver thread to terminate
      Step 2: After receiving all the sent segments, server will be ready to reset the receiver buffer.
              Server reset the receriver buffer and ready for new client connection.
   + Above implementation allows client to shutdown properly and server to reset and ready for new connection, without having to
     restart the server.
   + There is an edge case when this implementation might fail is that the wait time for client before it sends out the shutdown segment would not be enough,
     therefore, the server might already shutdown before acknowledgements arrive at client. In case some acks are lost, then the client would not be able
     to get those acks since server already resets. 
