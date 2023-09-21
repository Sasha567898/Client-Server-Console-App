#include "server.h"
#include <stdio.h>
#include <iostream>
int main() {
    if (initializeServer() == -1) {
        printf("Server initialization failed.\n");
        return 1;
    }

    int clientSocket = acceptClient();
    if (clientSocket == -1) {
        printf("Failed to accept client.\n");
        cleanupServer();
        return 1;
    }
   
   
    int ID = getRandomProcessID(receiveArrayFromClient(clientSocket));
   
    send(clientSocket, reinterpret_cast<char*>(&ID), sizeof(ID), 0);
    
    std:: vector<int>  one;

     bool receivedMessage;
 int bytesReceived = recv(clientSocket, reinterpret_cast<char*>(&receivedMessage), sizeof(receivedMessage), 0);

 if (bytesReceived == sizeof(receivedMessage)) {
    std::cout << "Received  message: " << std::endl;
 } else {
    std::cerr << "Failed to receive  message." << std::endl;
 }

   if(receivedMessage) std::cout<<"Process with ID: " << ID << " closed sucsesfuly"<<std::endl;
   else std::cout<<"Failed to close process with ID: " << ID<<std::endl;

    cleanupServer();

    return 0;
}