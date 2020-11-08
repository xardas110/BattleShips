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
#include "AiClient.h"

int main()
{
	
	std::shared_ptr<Battleships> battleShips = std::make_shared<Battleships>(Battleships());
	PlayerClient pC;
	AiClient aC;
	std::thread th;
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
				aC = AiClient();
				battleShips->InitMP(2);
				th = std::thread(Battleships::Init);
				aC.Connect("127.0.0.1");
				th.join();
				battleShips->RunMP();
				aC.Run();
				battleShips->JoinThreads();
				system("pause");
				break;
			case '3':	
				pC = PlayerClient();
				battleShips->InitMP(2);
				th = std::thread(Battleships::Init);
				pC.Connect("127.0.0.1");
				th.join();
				battleShips->RunMP();
				pC.Run();
				battleShips->JoinThreads();
				system("pause");
				break;
			default:
				break;
		}
	} while (input != '4');

	
}
