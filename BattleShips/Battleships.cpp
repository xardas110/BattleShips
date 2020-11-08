#include "Battleships.h"
#include <memory>
#include <ctime>
#include <iostream>
#include "Json.h"
#include "NetworkEvents.h"

Battleships *gs_BS = nullptr;

using UINT = unsigned int;
Battleships::~Battleships()
{
    delete server;
    delete[] serverTH;
}
Battleships::Battleships()
{
  
}
Battleships* Battleships::Get()
{
    return gs_BS;
}
void Battleships::OnListen(const size_t playerID)
{
    Battleships::Get()->OnUpdateRenderMP(playerID);
}

void Battleships::Init()
{
    Battleships::Get()->server->Init();
}

Battleships::HitStatus Battleships::IsHit(iVec2D& pos, const size_t playerID)
{
    if (pos.x < 0 || pos.x >= BS_NUM_COLS)
        return HitStatus::Invalid;
    if (pos.y < 0 || pos.y >= BS_NUM_ROWS)
        return  HitStatus::Invalid;
    if (players[playerID].board[pos.y][pos.x] == ship)
        return HitStatus::Hit;
    return HitStatus::Miss;
}
bool Battleships::IsCellEmpty(const iVec2D& pos, const size_t playerID)
{
    if (pos.x < 0 || pos.x >= BS_NUM_COLS)
        return false;
    if (pos.y < 0 || pos.y >= BS_NUM_ROWS)
        return false;
    if (players[playerID].board[pos.y][pos.x] != empty)
        return false;
    return true;
}
void Battleships::PrintPlayerBoards()
{
    system("cls");
    for (size_t i = 0; i < numPlayers; i++)
    {
        players[i].PrintBoard();
    }
}
const char Battleships::ReadTile(const int pos, const size_t playerID)
{
    const auto pos2D = Convert1Dto2D(pos);
    if (pos2D.x >= BS_NUM_COLS)
        return NULL;
    if (pos2D.y >= BS_NUM_ROWS)
        return NULL;
    return players[playerID].board[pos2D.y][pos2D.x];
}
const bool Battleships::SetTile(const int pos, const char ch, const size_t playerID)
{
    const auto pos2D = Convert1Dto2D(pos);
    if (pos2D.x >= BS_NUM_COLS)
        return false;
    if (pos2D.y >= BS_NUM_ROWS)
        return false;
    players[playerID].board[pos2D.y][pos2D.x] = ch;
    return true;
}
const void Battleships::SetMove(const int pos, const size_t playerID, const size_t enemyID)
{
    if (players[playerID].numShots > 0)
    { 
        const auto tile = ReadTile(pos, enemyID);
        if (tile == ship)
        { 
            SetTile(pos, hit, enemyID);
            players[playerID].numHits++;
            players[playerID].numShipsSunken++;
        }
        if (tile == empty)
            SetTile(pos, miss, enemyID);
        players[playerID].numShots--;
    }
}
void Battleships::MakeEmptyBoard()
{
    for (size_t i = 0; i < numPlayers; i++)
    {
        for (int y = 0; y < BS_NUM_ROWS; y++)
        {
            for (int x = 0; x < BS_NUM_COLS; x++)
            {        
                players[i].board[y][x] = ' ';
                players[i].availableTiles.push_back((y * BS_NUM_COLS) + x);
            }
        }
    }
}

int Battleships::GetRandomRow()
{
    return rand() % BS_NUM_ROWS;
}

int Battleships::GetRandomColumn()
{
    return rand() % BS_NUM_COLS;
}

int Battleships::Convert2Dto1D(const iVec2D& val)
{
    return val.y * BS_NUM_COLS + val.x;
}

iVec2D Battleships::Convert1Dto2D(const int n)
{
    iVec2D pos;
    pos.x = n % BS_NUM_COLS;
    pos.y = n / BS_NUM_COLS;
    return pos;
}

const int Battleships::GetRandomOpenTile(const size_t playerID)
{
    const int randNum = rand() % players[playerID].availableTiles.size();
    const int result = players[playerID].availableTiles[randNum];
    players[playerID].availableTiles.erase(players[playerID].availableTiles.begin() + randNum);
    players[playerID].takenTiles.push_back(result);
    return result;
}

void Battleships::WriteLetters()
{
    int currVal{ 65 };
    for (int x = 0; x < BS_NUM_COLS; x++)
        std::cout << '|' <<  (char)currVal++;
    std::cout << '|' << std::endl;
}

void Battleships::MakeBoard(const int numShips)
{
    for (size_t j = 0; j < numPlayers; j++)
    {
        for (int i = 0; i < numShips; i++)
        {   
            const int n = GetRandomOpenTile(j);
            const int x = n % BS_NUM_COLS;
            const int y = n / BS_NUM_COLS;
            players[j].board[y][x] = ship;      
        }
    }
}
void Battleships::MakeBoard3(const int numShips)
{
    for (size_t j = 0; j < numPlayers; j++)
    {
        int nShips{ 0 };
        while (nShips < numShips)
        {
            Ships::Ship ship;
            if (!players[j].ships->GetRandomShip(ship))
                break;
            nShips++;
            for (size_t i = 0; i < ship.pos.size(); i++)
            {
                iVec2D pos = Convert1Dto2D(ship.pos[i]);
                players[j].board[pos.y][pos.x] = this->ship;
            }

        };
    }
}
void Battleships::RespondShootAccepted(const size_t ID, const int pos, const char ch)
{
    Json response = Json(Json::Object);
    response.Set("Events", Json::Array);
    response.Set("Status", S_OK);
    response.Set("Event_Turn_Accepted", Json::Object);
    response["Events"].Add(EVENT_TURN_ACCEPTED);
    response["Event_Turn_Accepted"].Set("Pos", pos);
    response["Event_Turn_Accepted"].Set("Ch", (int)ch);
    server->Send(response.Stringify().c_str(), ID);
}
void Battleships::RespondPlayerShipHit(const size_t ID, const int pos, const char ch)
{
    Json response = Json(Json::Object);
    response.Set("Events", Json::Array);
    response.Set("Status", S_OK);
    response.Set("Event_Player_Ship_Hit", Json::Object);
    response["Events"].Add(EVENT_PLAYER_SHIP_HIT);
    response["Event_Player_Ship_Hit"].Set("Pos", pos);
    response["Event_Player_Ship_Hit"].Set("Ch", (int)ch);
    server->Send(response.Stringify().c_str(), ID);
}
void Battleships::RespondGameUpdate(const size_t ID)
{
    const size_t enemyID = ID == 0 ? 1 : 0;
    Json response = Json(Json::Object);
    response.Set("Events", Json::Array);
    response.Set("Status", S_OK);
    response["Events"].Add(EVENT_UPDATE);
    response.Set("Event_Update", Json::Object);
    response["Event_Update"].Set("NumShots", players[ID].numShots);
    response["Event_Update"].Set("NumHits", (int)players[enemyID].ships->GetNumHits());
    response["Event_Update"].Set("NumShipsSunken", (int)players[enemyID].ships->GetNumSunkenShips());
    response["Event_Update"].Set("Board", Json::Array);
    for (size_t y = 0; y < BS_NUM_ROWS; y++)
    {
        response["Event_Update"]["Board"].Add(Json::Array);
        for (size_t x = 0; x < BS_NUM_COLS; x++)
        {
            const int val = players[ID].board[y][x];
            response["Event_Update"]["Board"][y].Add(val);
        }
    }
    server->Send(response.Stringify().c_str(), ID);
}
void Battleships::RespondGameBegin(const size_t ID)
{
    Json response = Json(Json::Object);
    response.Set("Events", Json::Array);
    response.Set("Status", S_OK);
    response["Events"].Add(EVENT_BEGIN);
    response.Set("Event_Begin", Json::Object);
    response["Event_Begin"].Set("NumCol", BS_NUM_COLS);
    response["Event_Begin"].Set("NumRow", BS_NUM_ROWS);
    response["Event_Begin"].Set("NumShots", players[ID].numShots);
    response["Event_Begin"].Set("Board", Json::Array);
    for (size_t y = 0; y < BS_NUM_ROWS; y++)
    {
        response["Event_Begin"]["Board"].Add(Json::Array);
        for (size_t x = 0; x < BS_NUM_COLS; x++)
        {
            const int val = players[ID].board[y][x];
            response["Event_Begin"]["Board"][y].Add(val);
        }
    }
    server->Send(response.Stringify().c_str(), ID);
}
void Battleships::RespondGameFinished(const size_t playerID)
{
    const size_t enemyID = playerID == 0 ? 1 : 0;
    Json response = Json(Json::Object);
    response.Set("Events", Json::Array);
    response["Events"].Add(EVENT_FINISHED);
    response.Set("Event_Finished", Json::Object);
    response["Event_Finished"].Set("EnemyBoard", Json::Array);
    for (size_t y = 0; y < BS_NUM_ROWS; y++)
    {
        response["Event_Finished"]["EnemyBoard"].Add(Json::Array);
        for (size_t x = 0; x < BS_NUM_COLS; x++)
        {
            const int val = players[enemyID].board[y][x];
            response["Event_Finished"]["EnemyBoard"][y].Add(val);
        }
    }
    response["Event_Finished"].Set("YourBoard", Json::Array);
    for (size_t y = 0; y < BS_NUM_ROWS; y++)
    {
        response["Event_Finished"]["YourBoard"].Add(Json::Array);
        for (size_t x = 0; x < BS_NUM_COLS; x++)
        {
            const int val = players[playerID].board[y][x];
            response["Event_Finished"]["YourBoard"][y].Add(val);
        }
    }
    response["Event_Finished"].Set("EnemyHits", (int)players[playerID].ships->GetNumHits());
    response["Event_Finished"].Set("YourHits", (int)players[enemyID].ships->GetNumHits());
    response["Event_Finished"].Set("EnemyShipsSunken", (int)players[playerID].ships->GetNumSunkenShips());
    response["Event_Finished"].Set("YourShipsSunken", (int)players[enemyID].ships->GetNumSunkenShips());
    if (players[enemyID].ships->GetNumShips() > players[playerID].ships->GetNumShips())
        response["Event_Finished"].Set("Status", "Enemy Wins");
    else if (players[enemyID].ships->GetNumShips() == players[playerID].ships->GetNumShips())
        response["Event_Finished"].Set("Status", "Draw");
    else
        response["Event_Finished"].Set("Status", "You Win");
    server->Send(response.Stringify().c_str(), playerID);
}
void Battleships::RespondGamePing(const size_t playerID)
{
    Json response(Json::Object);
    response.Set("Events", Json::Array);
    response["Events"].Add(EVENT_PING);
    server->Send(response.Stringify().c_str(), playerID);
}
/*
void Battleships::MakeBoard3(const int numShips)
{
    if (gameMode == Invalid)
        gameMode = TrippleCell;
    assert(GameMode::TrippleCell == gameMode);
    iVec2D pos;
    pos.x = GetRandomColumn();
    pos.y = GetRandomRow();
    int placedShips{ 0 };
    while (placedShips < numShips)
    {
    }

}
*/

int Battleships::InitMP(const int numPlayers)
{
    gs_BS = shared_from_this().get();
    this->numPlayers = numPlayers;
    std::cout << "waiting conn..." << std::endl;
    server = new Server(numPlayers);
    players = new Player[numPlayers];
    for (size_t i = 0; i < numPlayers; i++)
        players[i] = Player(BS_NUM_COLS, BS_NUM_ROWS);
    
    srand(UINT(std::time(nullptr)));
    MakeEmptyBoard();
    MakeBoard3(9);
    return true;
}

void Battleships::RunMP()
{
    serverTH = new std::thread[numPlayers];
    for (size_t i = 0; i < numPlayers; i++)
        serverTH[i] = std::thread(Battleships::OnListen, i);
    
}

void Battleships::JoinThreads()
{
    for (size_t i = 0; i < numPlayers; i++)
    {
        serverTH[i].join();
    }
}

void Battleships::OnUpdateRenderMP(const size_t ID)
{
    const size_t enemyID = ID == 0 ? 1 : 0;  
    while (players[ID].numShots > 0 || players[enemyID].numShots > 0)
    { 
        char data[DEFAULT_BUFLEN]{};
        Json request;
        Json response;
        int iResult = recv(Server::Get()->clientSocket[ID], data, DEFAULT_BUFLEN, 0);
        if (iResult == -1)return;  if (iResult == 0)continue;
                              
        request = Json::Parse(data);
        int pos{ 0 }; HitStatus hitResult = HitStatus::Invalid; iVec2D pos2D; char hitCH{};
        for (const auto event : request["Events"])
        {
            switch ((int)event.Value())
            {
            case EVENT_BEGIN:
                RespondGameBegin(ID);
                break;
            case EVENT_SHOOT:
                pos = request["Event_Shoot"]["Pos"];
                pos2D = Convert1Dto2D(pos);
                hitResult = IsHit(pos2D, enemyID);
                if (hitResult == HitStatus::Invalid)
                { 
                    response.Set("Status", S_FAILED);
                    server->Send(response.Stringify().c_str(), ID);
                    continue;
                }
                //So the move is valid, set move and send updated info to the player
                if (players[ID].numShots > 0)
                { 
                    players[enemyID].ships->SetMove(pos, players[enemyID].board);
                    players[ID].numShots--;
                    hitCH = hitResult == HitStatus::Hit ? (int)hit : (int)miss;   
                    //Respond to player that shot succeded
                    RespondShootAccepted(ID, pos, hitCH);
                    //Now lets send updated info to enemy  
                    RespondPlayerShipHit(enemyID, pos, (int)players[enemyID].board[pos2D.y][pos2D.x]);
                }
                break;
            case EVENT_UPDATE:
                RespondGameUpdate(ID);
                break;
            case EVENT_PING:
                RespondGamePing(ID);
                break;
            default:
                break;
            }
        } 
        Sleep(10);//trying 100 ticks
    }
    Sleep(1000);
    RespondGameFinished(ID);
}
