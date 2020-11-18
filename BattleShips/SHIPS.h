#pragma once
#include <set>
#include <vector>
#include <map>
#include <ctime>
#include <algorithm>
#include <conio.h>


using SHIPREF = size_t;

class Ships
{
public:
    struct Ship
    {
        std::vector<size_t> pos;
    };
private:
    int numShips{ 0 }, numShipsSunken{ 0 }, numCol{0}, numRow{0}, numHits{0};
    std::vector<SHIPREF> possibleHorizontalPositions, possibleVerticalPositions;
    std::map<size_t, std::vector<SHIPREF>> possiblePositions;

    std::map<size_t, SHIPREF> takenPositions;
    std::map<size_t, SHIPREF> hitPositions;

    std::vector<Ship*> allShipPtrs;
    void AddPosition(SHIPREF ship);
    void DeletePosition(size_t x);
    SHIPREF Intersect(size_t pos);
    Ship GetRandomHorizontalShip();
    Ship GetRandomVerticalShip();
    bool Shoot(size_t pos);

public:
    size_t GetPossiblePositionsSize();
    size_t GetPossibleHorizontalPositionsSize();
    size_t GetPossibleVerticalPositionsSize();
    bool SetMove(size_t pos, char** board);
    bool GetRandomShip(Ship& ship);
    const std::map<size_t, SHIPREF>& GetTakenPositions() const;
    size_t GetNumShips();
    size_t GetNumSunkenShips();
    size_t GetNumHits();
    Ships(const size_t numCol, const size_t numRow);
   
    ~Ships();
};

