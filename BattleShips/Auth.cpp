#include "Auth.h"
#include <fstream>
#include <iostream>
#include "Vec.h"
#include <conio.h>
#include "TerminalHelpers.h"

Authenticate::Authenticate(const char* path)
{
    std::ifstream is;
    is.open("Password.txt", std::ios::in);
    is >> data;
    is.close();

    int startVal = 65;
    for (int x = 0; x < AUTH_NUM_COLS; x++)
        for (int y = 0; y < AUTH_NUM_ROWS; y++)
            grid[x][y] = (char)startVal++;

}

bool Authenticate::Login(const char* password)
{
    if (memcmp(password, data, std::strlen(data)) == 0)
        return true;
    
    return false;
}

bool Authenticate::ChangePassword()
{
    iVec2D pos(0, 0);
    char pass[100]{};
    char occupied{ '*' };
    pass[0] = grid[pos.y][pos.x];
    grid[pos.y][pos.x] = occupied;

    Print();
    thelp::Print(pass);
    char input{};
    do
    {
        const auto currLen = strlen(pass);
        grid[pos.y][pos.x] = pass[currLen - 1];
        input = _getch();
        switch (input)
        {
        case 'w':
            if (pos.y - 1 >= 0) pos.y--; else continue;
            break;
        case 's':
            if (pos.y + 1 < AUTH_NUM_ROWS) pos.y++; else continue;
            break;
        case 'd':
            if (pos.x + 1 < AUTH_NUM_COLS) pos.x++; else continue;
            break;
        case 'a':
            if (pos.x - 1 >= 0) pos.x--; else continue;
            break;
        default:
            continue;
        }
        pass[currLen] = grid[pos.y][pos.x];
        grid[pos.y][pos.x] = occupied;
        Print();
        thelp::Print(pass);
    } while (input != 13);

    std::ofstream os;
    os.open("Password.txt", std::ios::out);

    if (os.fail() or !os.is_open())
        return false;

    os.write(pass, strlen(pass));
    os.close();
    return true;
}

bool Authenticate::Run()
{
    iVec2D pos(0, 0);
    int currLen{ 1 };
    const int passLength = strlen(data);
    char* pass = new char[passLength];
    char occupied{ '*' };
    pass[0] = grid[pos.y][pos.x];
    grid[pos.y][pos.x] = occupied;
    
    Print();
    thelp::Print(pass, currLen);
    do
    {
        grid[pos.y][pos.x] = pass[currLen - 1];
        switch (_getch())
        {           
            case 'w':
                if (pos.y - 1 > 0) pos.y--; else continue;
                break;
            case 's':
                if (pos.y + 1 < AUTH_NUM_ROWS) pos.y++; else continue;
                break;
            case 'd':
                if (pos.x + 1 < AUTH_NUM_COLS) pos.x++; else continue;
                break;
            case 'a':
                if (pos.x - 1 > 0) pos.x--; else continue;
                break;
            default:
                continue;
        }
        pass[currLen] = grid[pos.y][pos.x];
        grid[pos.y][pos.x] = occupied;
        currLen++;
        Print();
        thelp::Print(pass, currLen);
    } while (currLen != passLength);
    grid[pos.y][pos.x] = pass[currLen - 1];
    if (Login(pass))
    { 
        delete[] pass;
        return true;
    }
    delete[] pass;
    return false;
}

void Authenticate::Print()
{
    system("cls");
    int startVal = 65;
    for (int y = 0; y < AUTH_NUM_COLS; y++)
    {      
        for (int x = 0; x < AUTH_NUM_ROWS; x++)
            std::cout << grid[y][x] << " ";
        std::cout << std::endl;
    }
}
