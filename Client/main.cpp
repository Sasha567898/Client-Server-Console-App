#include <iostream>
#include "client.h"
#include "process.h"
#include <vector>
using namespace std;
int main() {
    Client client;

    if (client.initializeClientSocket() == -1) {
        std::cerr << "Failed to initialize socket" << std::endl;
        return EXIT_FAILURE;
    }


    const char* server_ip = "192.168.1.105"; 
    int server_port = 16666; 
    
    if (client.connectToServer(server_ip, server_port) == -1) {
        std::cerr << "Failed to connect to the server" << std::endl;
        return EXIT_FAILURE;
    }

   std::vector<Processes> processesList = getProcesses();


   sendArrayToServer(client.client_socket, processesList);
   int recieveID;
   recv(client.client_socket, reinterpret_cast<char*>(&recieveID), sizeof(recieveID), 0);

   bool message = terminateProcessByID(recieveID);  
 int bytesSent = send(client.client_socket, reinterpret_cast<const char*>(&message), sizeof(message), 0);

 if (bytesSent == sizeof(message)) {
    std::cout << "Message sent successfully." << std::endl;
 } else {
    std::cerr << "Failed to send message." << std::endl;
 }

  
    
    // Close the connection
    client.closeConnection();
     
    return 0;
}