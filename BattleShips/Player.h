#pragma once
#include "TerminalHelpers.h"
#include "Vec.h"
#include <vector>
#include <set>
#include "SHIPS.h"
class Player
{
	friend class Battleships;
	Ships* ships;
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
	
	Player(const int numCol, const int numRow, const int numShots = 20);
	Player(const Player& other);
	Player& operator=(const Player& other);
	Player();
	void PrintBoard();
	const int Shoot();
	const int Shoot(iVec2D& pos);
	void SetTile(const iVec2D& pos, const char& hitCh, const bool bHit);
	~Player();
};

