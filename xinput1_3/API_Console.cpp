#include <windows.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>

#include "API_Console.h"

#include "ccMain.h"
#include "xinput1_3.h"
#include "HookFunctions.h"
#include "ccMessageInfo.h"
#include "LuaCommands.h"

using namespace std;
using namespace moddingApi;

// Console Functions
vector<string> consoleCommands;
vector<uintptr_t> consoleFunctions;
vector<int> consoleParams;
void AddCommand(string, uintptr_t, int);
int GetCommandIndex(string);

// Command Functions
void c_GetGlobalPointer();
void c_ConvertMessage();
void c_ChangeMode();
void c_IsMenu();
void c_IsBattle();

void c_ViewMessageConversions();
void c_Help();
void c_GetPlayerStatus();
void c_GetPlayerInfo();

void c_ccStartString();
void c_ccCreateString();
void c_ccSetStringId();
void c_ccStringPosition();
void c_ccStringScale();

void c_IsPtCutscene();
void c_ccAdvChangeDemoSettingState();

void API_Console::InitializeConsole()
{
	AddCommand("GetGlobalPointer", (uintptr_t)c_GetGlobalPointer, 0);
	AddCommand("ConvertMessage", (uintptr_t)c_ConvertMessage, 1);
	AddCommand("ChangeMode", (uintptr_t)c_ChangeMode, 1);
	AddCommand("IsMenu", (uintptr_t)c_IsMenu, 0);
	AddCommand("IsBattle", (uintptr_t)c_IsBattle, 0);
	//AddCommand("EndLoad", (uintptr_t)c_EndLoad, 0);
	AddCommand("ViewMessageConversions", (uintptr_t)c_ViewMessageConversions, 0);
	AddCommand("Help", (uintptr_t)c_Help, 0);
	AddCommand("GetPlayerStatus", (uintptr_t)c_GetPlayerStatus, 1);
	AddCommand("GetPlayerInfo", (uintptr_t)c_GetPlayerInfo, 1);

	AddCommand("ccStartString", (uintptr_t)c_ccStartString, 2);
	AddCommand("ccCreateString", (uintptr_t)c_ccCreateString, 2);
	AddCommand("ccSetStringId", (uintptr_t)c_ccSetStringId, 2);

	AddCommand("IsPtCutscene", (uintptr_t)c_IsPtCutscene, 0);
	AddCommand("ccAdvChangeDemoSettingState", (uintptr_t)c_ccAdvChangeDemoSettingState, 5);
}

typedef void(__stdcall * f)();
f Function;

void API_Console::DoConsoleCommand(string Input)
{
	string Command = Input;

	int ind = GetCommandIndex(Command);
	if (ind != -1)
	{
		Function = (f)consoleFunctions[ind];
		Function();

		cout << endl;
	}
	else
	{
		cout << "Command not found" << endl;
	}
}

DWORD GetGlobalPointer()
{
	__asm
	{
		mov eax, DWORD PTR ds : 0xcf69fc
	}
}

void c_GetGlobalPointer()
{
	cout << hex << GetGlobalPointer() << endl;
}

void c_ConvertMessage()
{
	std::string param1;

	cout << "MSG >> ";
	cin >> param1;
	char * param1_c = strcpy(new char[param1.length() + 1], param1.c_str());

	cout << ccMessageInfo::MessageToString(param1_c);
}

void c_ChangeMode()
{
	std::string param1;

	cout << "MODE >> ";
	cin >> param1;
	char * param1_c = strcpy(new char[param1.length() + 1], param1.c_str());

	LuaCommands::changeMode(stoi(param1_c));

	cout << "Changed mode to " << param1_c << endl;
}

#include "ccPlayer.h"
void c_IsMenu()
{
	bool i = ccPlayer::isOnMenu();
	if (i) cout << "is menu" << endl;
	else cout << "is not menu" << endl;
}

void c_IsBattle()
{
	bool i = ccPlayer::isOnBattle();
	if (i) cout << "is battle" << endl;
	else cout << "is not battle" << endl;
}

/*void c_EndLoad()
{
	ccGeneralGameFunctions::EndLoad();
}*/

void c_ViewMessageConversions()
{
	if (ccMessageInfo::MessageConversions == 0)
	{
		cout << "Enabling message conversions..." << endl;
		ccMessageInfo::EnableMessageConversion();
	}
	else
	{
		cout << "Disabling message conversions..." << endl;
		ccMessageInfo::DisableMessageConversion();
	}
}

void c_Help()
{
	cout << endl;
	cout << "Available commands:" << endl;
	for (int x = 0; x < consoleCommands.size(); x++)
	{
		cout << consoleCommands[x] << endl;
	}
}

void c_GetPlayerStatus()
{
	std::string param1;

	cout << "MODE >> ";
	cin >> param1;
	char * param1_c = strcpy(new char[param1.length() + 1], param1.c_str());

	cout << "PlayerStatus: " << hex << ccPlayer::GetPlayerStatus(stoi(param1_c)) << endl;
}

void c_GetPlayerInfo()
{
	std::string param1;

	cout << "MODE >> ";
	cin >> param1;
	char * param1_c = strcpy(new char[param1.length() + 1], param1.c_str());

	cout << "PlayerInfo: " << hex << ccPlayer::GetPlayerInfo(stoi(param1_c)) << endl;
}

void c_ccStartString()
{
	std::string param1;
	std::string param2;

	cout << "PARAM >> ";
	cin >> param1;
	char * param1_c = strcpy(new char[param1.length() + 1], param1.c_str());

	cout << "PARAM >> ";
	cin >> param2;
	char * param2_c = strcpy(new char[param2.length() + 1], param2.c_str());

	LuaCommands::ccStartString(stoi(param1_c), param2_c);
	cout << "Started string " << param1_c << " > " << param2_c << endl;
}

void c_ccCreateString()
{
	std::string param1;

	cout << "PARAM >> ";
	cin >> param1;
	char * param1_c = strcpy(new char[param1.length() + 1], param1.c_str());

	LuaCommands::ccCreateString(param1_c);
	cout << "Created string " << param1_c << endl;
}

void c_ccSetStringId()
{
	std::string param1;
	std::string param2;

	cout << "PARAM >> ";
	cin >> param1;
	char * param1_c = strcpy(new char[param1.length() + 1], param1.c_str());

	cout << "PARAM >> ";
	cin >> param2;
	char * param2_c = strcpy(new char[param2.length() + 1], param2.c_str());

	LuaCommands::ccSetStringId(param1_c, param2_c);
	cout << "Set string id " << param1_c << " > " << param2_c << endl;
}

void c_ccStringPosition();
void c_ccStringScale();

void c_IsPtCutscene()
{
	cout << "PT: " << LuaCommands::fc_isPtCutscene() << endl;
}

void c_ccAdvChangeDemoSettingState()
{
	std::string param1;

	cout << "P >> ";
	cin >> param1;
	char * param1_c = strcpy(new char[param1.length() + 1], param1.c_str());

	cout << "P >> ";
	cin >> param1;
	char * param2_c = strcpy(new char[param1.length() + 1], param1.c_str());

	cout << "P >> ";
	cin >> param1;
	char * param3_c = strcpy(new char[param1.length() + 1], param1.c_str());

	cout << "P >> ";
	cin >> param1;
	char * param4_c = strcpy(new char[param1.length() + 1], param1.c_str());

	cout << "P >> ";
	cin >> param1;
	char * param5_c = strcpy(new char[param1.length() + 1], param1.c_str());

	LuaCommands::ccAdvChangeDemoSettingState(param1_c, param2_c, param3_c, stof(param4_c), stof(param5_c));
}

void AddCommand(string command, uintptr_t function, int paramCount)
{
	consoleCommands.push_back(command);
	consoleFunctions.push_back(function);
	consoleParams.push_back(paramCount);
}

int GetCommandIndex(string command)
{
	int cmd = -1;

	for (int x = 0; x < consoleCommands.size(); x++)
	{
		//cout << command << " " << consoleCommands[x] << endl;
		if (command == consoleCommands[x])
		{
			cmd = x;
			x = consoleCommands.size();
		}
	}

	return cmd;
}