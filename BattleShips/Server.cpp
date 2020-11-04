#include "Server.h"
#include "TerminalHelpers.h"
#include "Json.h"
Server* gs_app;

int Server::Init()
{
    WSADATA wsaData;
    int iResult;

    struct addrinfo* result = NULL;
    struct addrinfo hints;

    int iSendResult;
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Create a SOCKET for connecting to server
    Server::Get()->listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (Server::Get()->listenSocket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    // Setup the TCP listening socket
    iResult = bind(Server::Get()->listenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(Server::Get()->listenSocket);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(result);

    iResult = listen(Server::Get()->listenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(Server::Get()->listenSocket);
        WSACleanup();
        return 1;
    }

    // Accept a client socket
    for (size_t i = 0; i < Server::Get()->numClients; i++)
    {
        Server::Get()->clientSocket[i] = accept(Server::Get()->listenSocket, NULL, NULL);
        if (Server::Get()->clientSocket[i] == INVALID_SOCKET) {
            printf("accept failed with error: %d\n", WSAGetLastError());
            closesocket(Server::Get()->listenSocket);
            WSACleanup();
            return 1;
        }
    }
    // No longer need server socket
    closesocket(Server::Get()->listenSocket);
    return 1;
}

void Server::IncomingData(char* data, const int socketIndex)
{
    for (size_t i = 0; i < strlen(data); i++)
    {
        std::cout << data[i];
    }
    std::cout << std::endl;
    
}

Server* Server::Get()
{
    return gs_app;
}

void __stdcall Server::OnListen()
{
    Server::Get()->Listen();
}

void Server::Run()
{
    std::cout << "Server starting..." << std::endl;
    listenTh = new std::thread(Server::OnListen);
    std::cout << "Server Running..." << std::endl;
    thelp::This_Thread_Sleep(1000);
}

int Server::SendAll(char* data)
{
    for (size_t i = 0; i < numClients; i++)
    {
        int iResult = send(clientSocket[i], data, strlen(data), 0);
        if (iResult == SOCKET_ERROR) {
            printf("send failed with error: %d\n", WSAGetLastError());
            closesocket(clientSocket[i]);
            WSACleanup();
            return 0;
        }
        printf("Bytes sent: %d\n", iResult);
    }
    return 1;
}

int Server::Send(const char* data, const int socketIndex)
{
    std::mutex mtx;
    mtx.lock();
    int iResult = send(Server::Get()->clientSocket[socketIndex], data, strlen(data), 0);
    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(Server::Get()->clientSocket[socketIndex]);
        WSACleanup();
        mtx.unlock();
        return 0;
    }
    mtx.unlock();
}

int Server::Recieve(char* data, const int socketIndex)
{
    //std::mutex mtx;
   // mtx.lock();
    while (true)
    {
        int iResult = recv(Server::Get()->clientSocket[socketIndex], data, DEFAULT_BUFLEN, 0);
        if (iResult > 1)
        {
           // mtx.unlock();
            return 1;
        }
        else if (iResult == 0)
        {
            continue;
        }
        else
        {
            //mtx.unlock();
            return -1;
        }
    }
        
   // mtx.unlock();
    return true;
}

int Server::WaitEvent(int event, const size_t waitMS, Json& eventData, const size_t socketID)
{
    const auto timeStart = std::chrono::system_clock::now();
    int iResult{ 0 };
    char data[DEFAULT_BUFLEN]{};
    do
    {
        iResult = recv(clientSocket[socketID], data, DEFAULT_BUFLEN, 0);
        eventData = Json::Parse(data);
        if (eventData.GetType() != Json::Object)
            continue;
        if (!eventData.Contains("Status"))
            continue;

        if ((int)eventData["Status"] == event)
            return 1;
        
        const auto timeEnd = std::chrono::system_clock::now();
        const auto timeElapsed = timeEnd - timeStart;
        if (timeElapsed > std::chrono::milliseconds(waitMS))
            return -1;
    } while (true);
    return 0;
}

int Server::Listen()
{
    char buff[DEFAULT_BUFLEN]{};
    int iResult{};
    // Receive until the peer shuts down the connection
    do {

        for (size_t i = 0; i < numClients; i++)
        {
            iResult = recv(clientSocket[i], buff, DEFAULT_BUFLEN, 0);
            if (iResult > 0) {
                IncomingData(buff, i);
            }
            else if (iResult == 0)
                printf("Connection closing...\n");
            else {
                printf("recv failed with error: %d\n", WSAGetLastError());
                closesocket(clientSocket[i]);
                WSACleanup();
                return 1;
            }
        }
        ZeroMemory(buff, DEFAULT_BUFLEN);
    } while (iResult > 0);
}

int Server::ShutDown()
{
    for (size_t i = 0; i < numClients; i++)
    {
        int iResult = shutdown(clientSocket[i], SD_SEND);
        if (iResult == SOCKET_ERROR) {
            printf("shutdown failed with error: %d\n", WSAGetLastError());
            closesocket(clientSocket[i]);
            WSACleanup();
            return 1;
        }
    }
}

void Server::Clean()
{
    ShutDown();
    for (size_t i = 0; i < numClients; i++)
        closesocket(clientSocket[i]);
    WSACleanup();
}

Server::Server(const int numClientSockets, const std::string& serverIP)
    :numClients(numClientSockets), serverIP(serverIP)
{
    gs_app = this;
    clientSocket = new SOCKET[numClients];
    for (size_t i = 0; i < numClients; i++)
        clientSocket[i] = INVALID_SOCKET;
}

Server::~Server()
{
    delete listenTh;
    delete[] clientSocket;
}
