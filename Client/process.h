#ifndef PROCESS_H
#define PROCESS_H
#include<string>
#include <vector>




struct Processes {
	int processesID;
	char command[256];
};
bool terminateProcessByID(int processID);
std::vector<Processes> getProcesses();
void sendArrayToServer(int clientSocket, const std::vector<Processes>& processesList);
#endif //PROCESS_H