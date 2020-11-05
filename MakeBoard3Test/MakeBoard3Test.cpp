#include <iostream>
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
    int numShips{ 0 }, numShipsSunken{ 0 }, numCol{}, numRow{};
    std::vector<size_t> possibleHorizontalPositions, possibleVerticalPositions;
    std::map<size_t, std::vector<SHIPREF>> possiblePositions;

    std::map<size_t, SHIPREF> takenPositions;
    std::map<size_t, SHIPREF> hitPositions;
    std::vector<Ship*> allShipPtrs;
    void AddPosition(SHIPREF ship)
    {
        const Ship s = *(Ship*)ship;
        for (const auto pos : s.pos)
            takenPositions[pos] = ship;
    }
    void DeletePosition(size_t x)
    {
        for (const auto val : possiblePositions[x])
        {
            possibleHorizontalPositions.erase(std::remove(possibleHorizontalPositions.begin(), possibleHorizontalPositions.end(), val), possibleHorizontalPositions.end());
            possibleVerticalPositions.erase(std::remove(possibleVerticalPositions.begin(), possibleVerticalPositions.end(), val), possibleVerticalPositions.end());
        }
        possiblePositions.erase(x);
    }
    SHIPREF Intersect(size_t pos)
    {
        const auto it = takenPositions.find(pos);
        const auto end = takenPositions.end();
        if (it != end)
        {
            return it->second;
        }
        return 0;
    }
    Ship GetRandomHorizontalShip()
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
    Ship GetRandomVerticalShip()
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
    bool Shoot(size_t pos)
    {
        SHIPREF shipRef = Intersect(pos);
        if (shipRef)
        {
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
   
public:
    size_t GetPossiblePositionsSize()
    {
        return possiblePositions.size();
    }
    size_t GetPossibleHorizontalPositionsSize()
    {
        return possibleHorizontalPositions.size();
    }
    size_t GetPossibleVerticalPositionsSize()
    {
        return possibleVerticalPositions.size();
    }
    bool SetMove(size_t pos, char** board)
    {
        int x = pos % numCol;
        int y = pos / numCol;
        if (Shoot(pos))
        {         
            board[y][x] = 'H';
        }
        else
        {
            board[y][x] = 'M';
        }
        return true;
    }
    bool GetRandomShip(Ship& ship)
    {

        if (possibleVerticalPositions.empty() && possibleHorizontalPositions.empty())
            return false;
        if (possibleVerticalPositions.empty() && !possibleHorizontalPositions.empty())
        {
            ship = GetRandomHorizontalShip();
            return true;
        }
        else
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
    };
    size_t GetNumShips()
    {
        return numShips;
    }
    size_t GetNumSunkenShips()
    {
        return numShipsSunken;
    }
    Ships(const size_t numCol, const size_t numRow)
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
                possibleVerticalPositions.push_back((size_t)ship);

                possiblePositions[sX].push_back((size_t)ship);
                possiblePositions[sY].push_back((size_t)ship);
                possiblePositions[sZ].push_back((size_t)ship);
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
                possibleHorizontalPositions.push_back((size_t)ship);

                possiblePositions[sX].push_back((size_t)ship);
                possiblePositions[sY].push_back((size_t)ship);
                possiblePositions[sZ].push_back((size_t)ship);
            }
        }
    }
    ~Ships()
    {
        for (auto ref : allShipPtrs)
        {
            delete ref;
        }
        allShipPtrs.clear();
    }
};
constexpr size_t row = 10;
constexpr size_t col = 10;
Ships ships(col, row);
char** board;

void MakeBoard3(const int numShips)
{
    do
    {
        Ships::Ship ship;
        if (!ships.GetRandomShip(ship))
            break;

        for (size_t i = 0; i < ship.pos.size(); i++)
        {
            int x = ship.pos[i] % col;
            int y = ship.pos[i] / col;
            board[y][x] = 'S';
        }

    } while (ships.GetNumShips() < numShips);
}
void PrintBoard()
{
    for (size_t y = 0; y < row; y++)
    {
        for (size_t x = 0; x < col; x++)
        {
            std::cout << '|' << board[y][x];
        }
        std::cout << "|" << std::endl;
    }
    std::cout << "Number of ships: " << ships.GetNumShips() << std::endl;
    std::cout << "Number of Ships sunken " << ships.GetNumSunkenShips() << std::endl;
}

int main()
{
    std::srand(std::time(nullptr));
  
    board = new char* [row];
    for (size_t y = 0; y < row; y++)
    {
        board[y] = new char[col];

        for (size_t x = 0; x < col; x++)
        {
            board[y][x] = ' ';

        }
    }
 
    std::cout << "possible pos size: " << ships.GetPossiblePositionsSize() << std::endl;
    std::cout << "possible vert size: " << ships.GetPossibleVerticalPositionsSize() << std::endl;
    std::cout << "possible hor size: " << ships.GetPossibleHorizontalPositionsSize() << std::endl;

    MakeBoard3(40);
    PrintBoard();

    std::cout << "possible pos size: " << ships.GetPossiblePositionsSize() << std::endl;
    std::cout << "possible vert size: " << ships.GetPossibleVerticalPositionsSize() << std::endl;
    std::cout << "possible hor size: " << ships.GetPossibleHorizontalPositionsSize() << std::endl;
    
    
    int val{};
    do
    {      
        std::cin >> val;
        if (ships.SetMove((int)val, board))
        {
            system("cls");
            PrintBoard();
            std::cout << "hit a ship" << std::endl;
        }
    } while (val != 500);
   
}
