#pragma once
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <thread>
#include <memory>

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"
#include "Json.h"
#include <mutex>

class Server
{
private:
	friend class Battleships;
	const int numClients;
	const std::string serverIP;	
	std::thread* listenTh = nullptr;
	SOCKET listenSocket = INVALID_SOCKET;
	SOCKET* clientSocket = nullptr;
	int Listen();
	
public:
	int Init();
	void IncomingData(char* data, const int socketIndex);
	static Server* Get();
	static void WINAPI OnListen();
	void Run();
	int SendAll(char* data);
	int Send(const char* data, const int socketIndex);
	int Recieve(char* data, const int socketIndex);
	int WaitEvent(int event, const size_t waitMS, Json& eventData, const size_t socketID);
	int ShutDown();
	void Clean();
	Server(const int numClientSockets = 1, const std::string& serverIP = "127.0.0.1");
	~Server();
};

