#pragma once
#include "ClientTCP.h"
#include "Json.h"
#include "TerminalHelpers.h"
#include "Vec.h"
#include <thread>

class PlayerClient
{
protected:
	std::thread			listenTH;							//Thread for listening to responses from server
	HANDLE				hWaitUpdate, hWaitBegin;			//Syncronizing core to core events
	char**				playerBoardReplicate{ nullptr };	//Player board replicate
	char**				enemyboardReplicate{ nullptr };		//Enemy board replicate
	Client				client;								//TCP stream wrapper for sockets
	size_t				numShots{}, numHits{};
	size_t				numRow{}, numCol{}, numShipsSunken{};
	bool				IsValidInput(const std::string& str, iVec2D& pos);//Checking if input is entered correctly(i.e a1, 1a etc.)
	enum ShootStatus
	{
		OutOfAmmo,
		OK,
		FailedToConvertUpper,
		InvalidInput
	};

	void GameBegin(const Json& data);			//Handling server response for game begin event
	void GameUpdate(const Json& data);			//Handling server response for game update event
	void GameFinished(const Json& data);		//Handling server response for game finished event
	void GamePlayerShipHit(const Json& data);	//Handling server response for play ship hit event

	int Convert2Dto1D(const iVec2D pos);
	iVec2D Convert1Dto2D(const int pos);
	bool SetTile(const int pos, char ch);
	void SetMove(const int pos, char ch);
	ShootStatus Shoot(iVec2D& pos);
	void PrintBoard();
	void PrintEndScreen(const int enemyShipsSunken, const std::string& statusText);
	void WriteLetters();
	static void ThreadWrapper();
	virtual void OnListen();
	const size_t GetNumShots() const;
	void SetNumShots(const size_t val);
public:
	PlayerClient();
	static PlayerClient* Get();
	void RequestGameBegin();
	virtual void RequestShoot();
	void RequestGameUpdate();
	void Connect(const std::string IP);
	void Run();
};

