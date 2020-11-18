#include "PlayerServer.h"
#include <iostream>
bool Player::IsValidInput(const std::string& str)
{
    std::string sStr{""};
    int val{ 0 };
    for (size_t i = 0; i < str.length(); i++)
    {

        if (thelp::IsDigit(str[i]))
        {
            int tempVal{ 0 };
            if (thelp::ConvertChToInt(str[i], tempVal))
            { 
                val *= 10;
                val += tempVal;
                continue;
            }
            else
            {
                std::cerr << "Digit conversion failed!" << std::endl;
                thelp::This_Thread_Sleep(1000);
                return false;
            }
        }
        if (thelp::IsAlphabetic(str[i]))
        {
            sStr += str[i];
            continue;
        }
        std::cerr << "Contains unrecognized characters!" << std::endl;
        thelp::This_Thread_Sleep(1000);
        return false;
    }

    if (val < 0 || val > numRow)
    { 
        std::cerr << "value contains too many rows: " << val << std::endl;
        thelp::This_Thread_Sleep(1000);
        return false;
    }

    if (sStr.length() > 1 or sStr.length() == 0)
    { 
        std::cerr << "Too many alphabetic characters" << std::endl;
        thelp::This_Thread_Sleep(1000);
        return false;
    }

    if (sStr[0] < 65 or sStr[0] > 64 + numCol)
    { 
        std::cerr << "Alphabetic character out of bounds" << std::endl;
        thelp::This_Thread_Sleep(1000);
        return false;
    }

    return true;
}
bool Player::IsValidInput(const std::string& str, char& ch, int& value)
{
    std::string sStr{ "" };
    int val{ 0 };
    for (size_t i = 0; i < str.length(); i++)
    {

        if (thelp::IsDigit(str[i]))
        {
            int tempVal{ 0 };
            if (thelp::ConvertChToInt(str[i], tempVal))
            {
                val *= 10;
                val += tempVal;
                continue;
            }
            else
            {
                std::cerr << "Digit conversion failed!" << std::endl;
                thelp::This_Thread_Sleep(1000);
                return false;
            }
        }
        if (thelp::IsAlphabetic(str[i]))
        {
            sStr += str[i];
            continue;
        }
        std::cerr << "Contains unrecognized characters!" << std::endl;
        thelp::This_Thread_Sleep(1000);
        return false;
    }

    if (val < 0 || val > numRow)
    {
        std::cerr << "value contains too many rows: " << val << std::endl;
        thelp::This_Thread_Sleep(1000);
        return false;
    }

    if (sStr.length() > 1 or sStr.length() == 0)
    {
        std::cerr << "Too many alphabetic characters" << std::endl;
        thelp::This_Thread_Sleep(1000);
        return false;
    }

    if (sStr[0] < 65 or sStr[0] > 64 + numCol)
    {
        std::cerr << "Alphabetic character out of bounds" << std::endl;
        thelp::This_Thread_Sleep(1000);
        return false;
    }

    ch = sStr[0];
    value = val;
    return true;
}
bool Player::IsValidInput(const std::string& str, iVec2D& pos)
{
    std::string sStr{ "" };
    int val{ 0 };
    for (size_t i = 0; i < str.length(); i++)
    {

        if (thelp::IsDigit(str[i]))
        {
            int tempVal{ 0 };
            if (thelp::ConvertChToInt(str[i], tempVal))
            {
                val *= 10;
                val += tempVal;
                continue;
            }
            else
            {
                std::cerr << "Digit conversion failed!" << std::endl;
                thelp::This_Thread_Sleep(1000);
                return false;
            }
        }
        if (thelp::IsAlphabetic(str[i]))
        {
            sStr += str[i];
            continue;
        }
        std::cerr << "Contains unrecognized characters!" << std::endl;
        thelp::This_Thread_Sleep(1000);
        return false;
    }

    if (val < 0 || val > numRow)
    {
        std::cerr << "value contains too many rows: " << val << std::endl;
        thelp::This_Thread_Sleep(1000);
        return false;
    }

    if (sStr.length() > 1 or sStr.length() == 0)
    {
        std::cerr << "Too many alphabetic characters" << std::endl;
        thelp::This_Thread_Sleep(1000);
        return false;
    }

    if (sStr[0] < 65 or sStr[0] > 64 + numCol)
    {
        std::cerr << "Alphabetic character out of bounds" << std::endl;
        thelp::This_Thread_Sleep(1000);
        return false;
    }

    pos.x = (int)sStr[0]-65;
    pos.y = numRow - val;
    return true;
}
Player::Player(const int numCol, const int numRow, const int numShots)
    :numCol(numCol), numRow(numRow), numShots(numShots), numHits{ 0 }
{
    ships = new Ships(numCol, numRow);
    board = new char*[numRow];
    for (size_t y = 0; y < numRow; y++)
    {
        board[y] = new char[numCol];
        for (size_t x = 0; x < numCol; x++)
            board[y][x] = ' ';      
    }
    
}
Player::Player(const Player& other)
    :numCol(other.numCol), numRow(other.numRow),numShots(other.numShots), numHits(other.numHits)
{
    ships = new Ships(other.numCol, other.numRow);
    board = std::move(other.board);
}

Player& Player::operator=(const Player& other)
{
    numCol = other.numCol;
    numRow = other.numRow;
    numShots = other.numShots;
    numHits = other.numHits;
    ships =  new Ships(numCol, numRow);//Should make a copy of ships class here, but Im just to lazy
    board = new char* [numRow];
    for (size_t i = 0; i < numRow; i++)
    {
        board[i] = new char[numCol];
    }

    for (size_t y = 0; y < numRow; y++)
    {
        for (size_t x = 0; x < numCol; x++)
        {
            board[y][x] = other.board[y][x];
        }
    }


    return *this;
}

Player::Player()
    :numCol(0), numRow(0), numShots(0), numHits{ 0 }
{
    ships = new Ships(0, 0);
}
void Player::PrintBoard()
{
    for (size_t y = 0, ym = numRow; y < numRow; y++, ym--)
    {
        std::cout << ym << " ";
        for (size_t x = 0; x < numCol; x++)
        {
            std::cout << '|' << board[y][x];
        }
        std::cout << '|' << std::endl;
    }
    std::cout << "  ";
    for (int i = 0; i < numCol * 2 + 1; i++)
        std::cout << '-';
    std::cout << std::endl;
    std::cout << "  ";
    int currVal{ 65 };
    for (int x = 0; x < numCol; x++)
        std::cout << '|' << (char)currVal++;
    std::cout << '|' << std::endl;
    std::cout << "Num Shots: " << numShots;
    std::cout << "Num Hits: " << numHits;
}

const int Player::Shoot()
{
    std::string str{""};
    thelp::AskInputS("Select a tile to shoot at:(i.e A1/a1 or 1A/1a) ", str);

    if (!thelp::ToUpper(str))
        return 0;

    char ch{};
    int val{};
    if (!IsValidInput(str, ch, val))
        return false;

    const int x = (int)ch - 65;
    const int y = numCol -  val;
    std::cout << "x: " << x << " y: " << y << std::endl;
    thelp::This_Thread_Sleep(1000);
    return y*numCol + x;
}

const int Player::Shoot(iVec2D& pos)
{
    if (numShots <= 0)
    { 
        std::cerr << "Player no ammo" << std::endl;
        thelp::This_Thread_Sleep(2000);
        return false;
    }
    std::string str{ "" };
    thelp::AskInputS("Select a tile to shoot at:(i.e A1/a1 or 1A/1a) ", str);

    if (!thelp::ToUpper(str))
        return 0;

    if (!IsValidInput(str, pos))
        return false;

    return true;
}

void Player::SetTile(const iVec2D& pos, const char& hitCh, const bool bHit)
{   
    numShots--;
    if (bHit && board[pos.y][pos.x]!= hitCh)
    { 
        numHits++;
        board[pos.y][pos.x] = hitCh;
    }
}

Player::~Player()
{
    delete ships;
}
