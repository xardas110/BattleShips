#pragma once
#include "Server.h"
#include <vector>
#include "PlayerServer.h"
#include <memory>
#include <assert.h>
#include <thread>

#define BS_NUM_COLS 6
#define BS_NUM_ROWS 6

class Battleships : public std::enable_shared_from_this<Battleships>
{
	enum class HitStatus
	{
		Invalid, Miss, Hit
	};
	std::thread*	serverTH = nullptr;
	Server*			server = nullptr;	
	Player*			players;
	int				numPlayers{ 0 };

	const char		empty{ ' ' };
	const char		ship{ 'S' };
	const char		miss{ 'M' };
	const char		hit{ 'H' };
	
	HitStatus		IsHit(iVec2D pos, const size_t playerID);
	bool			IsCellEmpty(const iVec2D& pos, const size_t playerID);
	void			PrintPlayerBoards();	
	const char		ReadTile(const int pos, const size_t playerID);
	const bool		SetTile(const int pos, const char ch, const size_t playerID);
	const void		SetMove(const int pos, const size_t playerID, const size_t enemyID);
	void			MakeEmptyBoard();
	void			MakeBoard(const int numShips);
	void			MakeBoard3(const int numShips);

	//Server response to client-------------------------------------------------------------------//
	void _stdcall	RespondShootAccepted(const size_t playerID, const int pos, const char ch) const;
	void _stdcall	RespondPlayerShipHit(const size_t playerID, const int pos, const char ch) const;
	void _stdcall	RespondGameUpdate(const size_t playerID) const;
	void _stdcall	RespondGameBegin(const size_t playerID) const;
	void _stdcall	RespondGameFinished(const size_t playerID) const;
	void _stdcall	RespondGamePing(const size_t playerID) const;
	void _stdcall	RespondShootEvent(const size_t pos, const size_t playerID, const size_t enemyID);
protected:	
	const int		GetRandomRow() const;
	const int		GetRandomColumn() const;
	const int		Convert2Dto1D(const iVec2D& val) const;
	const iVec2D	Convert1Dto2D(const int n) const;
	const int		GetRandomOpenTile(const size_t playerID) const;
	void			WriteLetters() const;
	void			OnUpdateRenderMP(const size_t ID); // Game loop
	const bool		SetMove(const size_t pos, Player& player);
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

