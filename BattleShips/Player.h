#pragma once
#include "TerminalHelpers.h"
#include "Vec.h"
#include <vector>
class Player
{
	friend class Battleships;
	struct Ship
	{
		std::vector<int> pos;
	};
	std::vector<Ship> Ships;
	enum GameMode
	{
		SingleCell, TrippleCell, Invalid
	} gameMode = GameMode::Invalid;

	int numShots{};
	int numHits{};
	int numCol{};
	int numRow{};
	int numShipsSunken{};
	char** board;
	const char empty{ ' ' };
	const char ship{ 'S' };
	const char miss{ 'M' };
	const char hit{ 'H' };
	bool IsValidInput(const std::string &str);
	bool IsValidInput(const std::string& str, char& ch, int& val);
	bool IsValidInput(const std::string& str, iVec2D &pos);
	std::vector<int> availableTiles;
	std::vector<int> takenTiles;
public:
	
	Player(const int numCol, const int numRow, const int numShots = 1);
	Player(const Player& other);
	Player& operator=(const Player& other);
	Player();
	void PrintBoard();
	const int Shoot();
	const int Shoot(iVec2D& pos);
	void SetTile(const iVec2D& pos, const char& hitCh, const bool bHit);
	~Player();
};
