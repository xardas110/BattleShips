#pragma once
#include "Server.h"
#include <vector>
#include "Player.h"
#include <memory>
#include <assert.h>
#include <thread>

#define BS_NUM_COLS 6
#define BS_NUM_ROWS 6

class Battleships : public std::enable_shared_from_this<Battleships>
{
	Server* server = nullptr;
	std::thread* serverTH = nullptr;
	Player* players;
	int numPlayers{ 0 };
	struct Ship
	{
		std::vector<int> pos;
	};
	std::vector<Ship> Ships;
	enum GameMode
	{
		Invalid, SingleCell, TrippleCell
	} gameMode = GameMode::Invalid;
	enum class HitStatus
	{
		Invalid, Miss, Hit
	};
	const char empty{ ' ' };
	const char ship{ 'S' };
	const char miss{ 'M' };
	const char hit{ 'H' };
private:	
	HitStatus IsHit(iVec2D& pos, const size_t playerID);
	bool IsCellEmpty(const iVec2D& pos, const size_t playerID);
	void PrintPlayerBoards();	
	const char ReadTile(const int pos, const size_t playerID);
	const bool SetTile(const int pos, const char ch, const size_t playerID);
	const void SetMove(const int pos, const size_t playerID, const size_t enemyID);
	void MakeEmptyBoard();
	void MakeBoard(const int numShips);

	void RespondShootAccepted(const size_t playerID, const int pos, const char ch);
	void RespondPlayerShipHit(const size_t playerID, const int pos, const char ch);
	void RespondGameUpdate(const size_t playerID);
	void RespondGameBegin(const size_t playerID);
	void RespondGameFinished(const size_t playerID);
protected:	
	int GetRandomRow();
	int GetRandomColumn();
	int Convert2Dto1D(const iVec2D& val);
	iVec2D Convert1Dto2D(const int n);
	const int GetRandomOpenTile(const size_t playerID);
	void WriteLetters();
	virtual void OnUpdateRenderMP(const size_t ID);

public:
	~Battleships();
	Battleships();
	static Battleships* Get();
	static void OnListen(const size_t playerID);
	static void Init();
	virtual int InitMP(const int numPlayers = 2);
	virtual void RunMP();
	void JoinThreads();
	
};

