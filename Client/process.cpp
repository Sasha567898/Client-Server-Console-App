#include "process.h"
#include <cstring>
#ifdef _WIN32 
#include <windows.h>
#include <tlhelp32.h>
#include <cstdio> 
#include <vector>
#include <string>
#include <Winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")


std::vector<Processes> getProcesses() {

    std :: vector <Processes> processesArray;
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) {
        printf("CreateToolhelp32Snapshot failed with error %d\n", GetLastError());
        return processesArray;
    }

    PROCESSENTRY32 processEntry;
    processEntry.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(snapshot, &processEntry)) {
        do {
        
          Processes process;
             process.processesID = processEntry.th32ProcessID;
             int bufferSize = MultiByteToWideChar(CP_UTF8, 0, processEntry.szExeFile, -1, nullptr, 0);


                std::vector<wchar_t> wideBuffer(bufferSize);
                int result = MultiByteToWideChar(CP_UTF8, 0, processEntry.szExeFile, -1, wideBuffer.data(), bufferSize);
                bufferSize = WideCharToMultiByte(CP_UTF8, 0, wideBuffer.data(), -1, nullptr, 0, nullptr, nullptr);
                char* buffer = new char[bufferSize];
                result = WideCharToMultiByte(CP_UTF8, 0, wideBuffer.data(), -1, buffer, bufferSize, nullptr, nullptr);
                std::strncpy(process.command, buffer, sizeof(process.command) - 1);
                delete[] buffer;
                processesArray.push_back(process);
     
     } while (Process32Next(snapshot, &processEntry));
    } else {
        printf("Process32First failed with error %d\n", GetLastError());
    }

    CloseHandle(snapshot);

    return processesArray;
}


#else 
#include <dirent.h>
#include <cstdio> 
#include <cstdlib> 
#include <vector>++
#include <string>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>

std::vector<Processes> getProcesses() {

     std :: vector <Processes> processesArray;

    DIR *dir = opendir("/proc");
    if (!dir) {
        perror("opendir");
        return processesArray;
    }

    struct dirent *entry;
    while ((entry = readdir(dir))) {
        if (entry->d_type == DT_DIR) {
            int pid = atoi(entry->d_name);
            if (pid > 0) {
                char path[256];
                snprintf(path, sizeof(path), "/proc/%d/cmdline", pid);

                FILE *cmdline_file = fopen(path, "r");
                if (cmdline_file) {
                    char cmdline[256];
                    if (fgets(cmdline, sizeof(cmdline), cmdline_file)) {
                       
                     cmdline[strcspn(cmdline, "\n")] = '\0';

                     Processes process;
                     process.processesID = pid;
                     strcpy(process.command, cmdline);
                    
                     processesArray.push_back(process);

                    }
                    fclose(cmdline_file);
                }
            }
        }
    }

    closedir(dir);

   return processesArray;
}
#endif


void sendArrayToServer(int clientSocket, const std::vector<Processes>& processesList) {
    
    size_t arraySize = processesList.size();
    size_t arraySizeNetworkOrder = htonl(arraySize);  
    send(clientSocket, reinterpret_cast<char*>(&arraySizeNetworkOrder), sizeof(arraySizeNetworkOrder), 0);

    
  if (arraySize > 0) {
   
    const char* sendData = reinterpret_cast<const char*>(processesList.data());
    send(clientSocket, sendData, arraySize * sizeof(Processes), 0);
}
}

bool terminateProcessByID(int processID) {
#ifdef _WIN32
    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, processID);
    if (hProcess == NULL) {
        
        return false;
    }

    BOOL result = TerminateProcess(hProcess, 0);
    CloseHandle(hProcess);

    return result != 0;
#else
    if (kill(processID, SIGTERM) == 0) {
        return true;
    } else {
       return false;
    }
#endif
}