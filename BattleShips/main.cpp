#define SINGLEPLAYER 1
#include "Battleships.h"
#include "Auth.h"
#include "TerminalHelpers.h"

#include <iostream>
#include <chrono>
#include <conio.h>
#include <string>
#include <memory>
#include <thread>
#include "ClientTCP.h"
#include "Json.h"
#include "PlayerClient.h"

int main()
{
	/*
	std::shared_ptr<Battleships> battleShips = std::make_shared<Battleships>(Battleships());
	int numPlay{ 2 };
	Authenticate auth("Password.txt");
	while (!auth.Run())
	{
		std::cout << "pass mismatch" << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
	std::cout << "pass matching" << std::endl;

	char input{};
	do
	{	
		system("cls");
		std::cout << "1. Change the Password" << std::endl;
		std::cout << "2. Play Battleship" << std::endl;
		std::cout << "3. Play Battleship multiplayer" << std::endl;
		std::cout << "4. Quit" << std::endl;
		input = _getch();
		switch (input)
		{
			case '1':
				system("cls");		
				if (!auth.ChangePassword())
				{ 
					std::cout << "Failed to change password";
					thelp::This_Thread_Sleep(2000);
					continue;
				}
				break;
			case'2':
				if (!battleShips->InitSP()) return -1;
				do
				{
					battleShips->OnUpdateRender();
					battleShips->OnClear();
				} while (true);
				break;
			case '3':			
				thelp::AskInput("Choose number of players: ", numPlay);
				if (!battleShips->InitMP(numPlay)) return -1;
				do
				{
					battleShips->OnUpdateRenderMP();
				} while (true);
				break;
			default:
				break;
		}
	} while (input != '4');
	*/
	std::shared_ptr<Battleships> battleShips = std::make_shared<Battleships>(Battleships());
	PlayerClient pC;
	battleShips->InitMP(2);
	std::thread th(Battleships::Init);	
	pC.Connect("192.168.2.248");
	th.join();
	battleShips->RunMP();
	pC.Run();

	battleShips->JoinThreads();
}
