#pragma once

#define AUTH_NUM_COLS 4
#define AUTH_NUM_ROWS 4

class Authenticate
{
	char data[100];
	char grid[AUTH_NUM_COLS][AUTH_NUM_ROWS];
public:
	/*Filelist of passwords*/
	Authenticate(const char* path);
	bool Login(const char* password);
	bool ChangePassword();
	bool Run();
private:
	void Print();
};

