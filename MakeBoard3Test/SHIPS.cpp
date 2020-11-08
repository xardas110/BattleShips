#include "SHIPS.h"

void Ships::AddPosition(SHIPREF ship)
{
    const Ship s = *(Ship*)ship;
    for (const auto pos : s.pos)
        takenPositions[pos] = ship;
}

void Ships::DeletePosition(size_t x)
{
    for (const auto val : possiblePositions[x])
    {
        possibleHorizontalPositions.erase(std::remove(possibleHorizontalPositions.begin(), possibleHorizontalPositions.end(), val), possibleHorizontalPositions.end());
        possibleVerticalPositions.erase(std::remove(possibleVerticalPositions.begin(), possibleVerticalPositions.end(), val), possibleVerticalPositions.end());
    }
    possiblePositions.erase(x);
}

SHIPREF Ships::Intersect(size_t pos)
{
    const auto it = takenPositions.find(pos);
    const auto end = takenPositions.end();
    if (it != end)
    {
        return it->second;
    }
    return 0;
}

Ships::Ship Ships::GetRandomHorizontalShip()
{
    const int newRand = std::rand() % possibleHorizontalPositions.size();
    Ship ship = *(Ship*)possibleHorizontalPositions[newRand];
    AddPosition(possibleHorizontalPositions[newRand]);
    for (const auto val : ship.pos)
    {
        DeletePosition(val);
    }
    numShips++;
    return ship;
}

Ships::Ship Ships::GetRandomVerticalShip()
{
    const int newRand = std::rand() % possibleVerticalPositions.size();
    Ship ship = *(Ship*)possibleVerticalPositions[newRand];
    AddPosition(possibleVerticalPositions[newRand]);
    for (const auto val : ship.pos)
    {
        DeletePosition(val);
    }
    numShips++;
    return ship;
}

bool Ships::Shoot(size_t pos)
{
    SHIPREF shipRef = Intersect(pos);
    if (shipRef)
    {
        numHits++;
        Ship* shipPtr = (Ship*)shipRef;
        takenPositions.erase(pos);
        hitPositions[pos] = shipRef;
        const auto beg = shipPtr->pos.begin();
        const auto end = shipPtr->pos.end();
        shipPtr->pos.erase(std::remove(beg, end, pos), shipPtr->pos.end());
        if (shipPtr->pos.empty())
        {
            numShips--;
            numShipsSunken++;
        }
        return true;
    }
    return false;
}

size_t Ships::GetPossiblePositionsSize()
{
    return possiblePositions.size();
}

size_t Ships::GetPossibleHorizontalPositionsSize()
{
    return possibleHorizontalPositions.size();
}

size_t Ships::GetPossibleVerticalPositionsSize()
{
    return possibleVerticalPositions.size();
}

bool Ships::SetMove(size_t pos, char** board)
{
    int x = pos % numCol;
    int y = pos / numCol;
    if (Shoot(pos))
    {
        board[y][x] = 'H';
    }
    else
    {
        if (board[y][x] == ' ')
            board[y][x] = 'M';
    }
    return true;
}

bool Ships::GetRandomShip(Ship& ship)
{
    if (possibleVerticalPositions.empty() && possibleHorizontalPositions.empty())
        return false;
    if (possibleVerticalPositions.empty())
    {
        ship = GetRandomHorizontalShip();
        return true;
    }

    if (possibleHorizontalPositions.empty())
    {
        ship = GetRandomVerticalShip();
        return true;
    }

    const int rand = std::rand() % 2;
    if (rand)
        ship = GetRandomHorizontalShip();
    else
        ship = GetRandomVerticalShip();
    return true;
}

size_t Ships::GetNumShips()
{
    return numShips;
}

size_t Ships::GetNumSunkenShips()
{
    return numShipsSunken;
}

size_t Ships::GetNumHits()
{
    return numHits;
}

Ships::Ships(const size_t numCol, const size_t numRow)
    :numRow(numRow), numCol(numCol)
{
    const size_t halfSize = 1;
    //vertical ship insert
    for (size_t y = halfSize; y < numRow - halfSize; y++)
    {
        for (size_t x = 0; x < numCol; x++)
        {
            Ships::Ship* ship = new Ships::Ship();
            allShipPtrs.push_back(ship);
            const size_t sX = (y - halfSize) * numCol + x;
            const size_t sY = y * numCol + x;
            const size_t sZ = (y + halfSize) * numCol + x;
            ship->pos.push_back(sX);
            ship->pos.push_back(sY);
            ship->pos.push_back(sZ);
            possibleVerticalPositions.push_back((SHIPREF)ship);

            possiblePositions[sX].push_back((SHIPREF)ship);
            possiblePositions[sY].push_back((SHIPREF)ship);
            possiblePositions[sZ].push_back((SHIPREF)ship);
        }
    }
    //horizontal ship
    for (size_t y = 0; y < numRow; y++)
    {
        for (size_t x = halfSize; x < numCol - halfSize; x++)
        {
            Ship* ship = new Ship();
            allShipPtrs.push_back(ship);
            const size_t sX = y * numCol + (x - halfSize);
            const size_t sY = y * numCol + x;
            const size_t sZ = y * numCol + (x + halfSize);

            ship->pos.push_back(sX);
            ship->pos.push_back(sY);
            ship->pos.push_back(sZ);
            possibleHorizontalPositions.push_back((SHIPREF)ship);

            possiblePositions[sX].push_back((SHIPREF)ship);
            possiblePositions[sY].push_back((SHIPREF)ship);
            possiblePositions[sZ].push_back((SHIPREF)ship);
        }
    }
}

Ships::~Ships()
{
    for (auto ref : allShipPtrs)
    {
        delete ref;
    }
    allShipPtrs.clear();
}
