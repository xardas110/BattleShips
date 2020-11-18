#include <iostream>
#include "SHIPS.h"
constexpr size_t row = 20;
constexpr size_t col = 20;
Ships ships(col, row);
char** board;

void MakeBoard3(const int numShips)
{
    int shipss{ 0 };
    while (shipss < numShips)
    {
        Ships::Ship ship;
        if (!ships.GetRandomShip(ship))
            break;
        shipss++;
        for (size_t i = 0; i < ship.pos.size(); i++)
        {
            int x = ship.pos[i] % col;
            int y = ship.pos[i] / col;
            board[y][x] = 'S';
        }
    } ;
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
    std::cout << "Number of hits: " << ships.GetNumHits() << std::endl;
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

    MakeBoard3(200);
    PrintBoard();

    std::cout << "possible pos size: " << ships.GetPossiblePositionsSize() << std::endl;
    std::cout << "possible vert size: " << ships.GetPossibleVerticalPositionsSize() << std::endl;
    std::cout << "possible hor size: " << ships.GetPossibleHorizontalPositionsSize() << std::endl;
    
    
    int val{};
    do
    {      
        std::cin >> val;
        if (val > 0)
        {
            MakeBoard3(val);
            system("cls");
            PrintBoard();
        }
        else
        {
            ships.SetMove((int)val, board);
            system("cls");
            PrintBoard();
            std::cout << "hit a ship" << std::endl;
        }
       
    } while (val != 500);
   
    for (size_t i = 0; i < 100; i++)
    {
        std::cout << std::rand() % 2 << std::endl;
    }
}
