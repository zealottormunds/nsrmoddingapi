#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>

#include "ccMain.h"
#include "xinput1_3.h"
#include "ccMessageInfo.h"
#include "API_Console.h"
#include "ccPlayer.h"

using namespace moddingApi;
using namespace std;

DWORD WINAPI FixedLoop();

bool Enable = false;
int ccMain::GAMEVERSION = 1;
bool EnableConsole = true;

// Main function of the API
DWORD WINAPI ccMain::Main()
{
	// Console allocation
	if (EnableConsole)
	{
		AllocConsole();
		freopen("CONIN$", "r", stdin);
		freopen("CONOUT$", "w", stdout);
	}
	cout << "====== Welcome to STORM REV MODDING CONSOLE ======" << endl;

	// Enable dll
	Enable = true;
	cout << "Dll enabled." << endl;

	// Initialize stuff
	ReadApiFiles();
	API_Console::InitializeConsole();
	ccMessageInfo::Initialize();
	ccPlayer::Start();

	CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)FixedLoop, (HMODULE)xinput1_3::st_hModule, 0, nullptr);
	ccMain::Loop();

	return 0;
}

DWORD WINAPI ccMain::Loop()
{
	while (!Enable)
	{
		Sleep(100);
	}

	while (Enable)
	{
		std::string cmd;
		cout << "> ";
		cin >> cmd;

		API_Console::DoConsoleCommand(cmd);

		Sleep(100);
	}

	return 0;
}

DWORD WINAPI FixedLoop()
{
	while (true)
	{
		ccPlayer::Loop();
		Sleep(10);
	}
	return 0;
}

int ccMain::ModCount = 0;
vector<std::string> ccMain::ModList;
vector<std::string> ccMain::ModDesc;
vector<std::string> ccMain::ModAuth;
vector<BYTE> ccMain::ModState;

vector<BYTE> ReadAllBytes(string _file);
void ReadGuardFrameParam(string _file);

void ccMain::ReadApiFiles()
{
	char ApiPath[_MAX_PATH];
	GetCurrentDirectory(_MAX_PATH, ApiPath);
	int ActualLength = strlen(ApiPath);

	strcat(ApiPath, "\\gev_files\\");

	char ConfigPath[_MAX_PATH];
	strcpy(ConfigPath, ApiPath);
	ActualLength = strlen(ConfigPath);
	strcat(ConfigPath, "config.ini");

	cout << "Config finished..." << endl;

	// Start reading mods
	strcat(ApiPath, "data\\");

	filesystem::create_directories(ApiPath);

	for (const auto & entry : filesystem::directory_iterator(ApiPath))
	{
		std::string s = entry.path().string();

		char ModName[100];
		int StartIndex = strlen(ApiPath);
		int Count = s.length() - StartIndex;

		strncpy(ModName, &s[StartIndex], 100);

		std::string ModPath = (std::string)(ApiPath) + (std::string)(ModName);
		std::string InfoPath = ModPath + "\\info.txt";

		//cout << InfoPath << endl;

		if (filesystem::exists(InfoPath) == true)
		{
			BYTE actual = 0;
			string modName;
			string modDesc;
			string modAuth;

			ifstream file;
			file.open(InfoPath);

			while (file.eof() == false && actual < 3)
			{
				char a;
				switch (actual)
				{
				case 0:
					a = file.get();
					if (a == '|') actual = 1;
					else modName = modName + a;
					break;
				case 1:
					a = file.get();
					if (a == '|') actual = 2;
					else modDesc = modDesc + a;
					break;
				case 2:
					a = file.get();
					if (a == '|') actual = 3;
					else modAuth = modAuth + a;
					break;
				}
			}
			file.close();

			if (modName != "")
			{
				// Start reading mod files
				vector<string> files;
				for (const auto & f : filesystem::directory_iterator(ModPath))
				{
					string _file = f.path().string();
					string _ext = _file.substr(_file.length() - 4, 4);

					if (_ext == "ns4s")
					{
						vector<string> msg = ccMessageInfo::ReadMessageFile(_file);

						for (int x = 0; x + 1 < msg.size(); x = x + 2)
						{
							ccMessageInfo::MessageID.push_back(msg[x]);
							ccMessageInfo::MessageStr.push_back(msg[x + 1]);
						}
					}
					else if (_ext == "ns4t")
					{
						vector<string> msg = ccMessageInfo::ReadMessageFile(_file);

						for (int x = 0; x + 1 < msg.size(); x = x + 2)
						{
							ccMessageInfo::MessageID_ALT.push_back(msg[x]);
							ccMessageInfo::MessageStr_ALT.push_back(msg[x + 1]);
						}
					}
					else if (_file.length() > 0x15 && _file.substr(_file.length() - 0x15, 0x15) == "guardFrameParam.xfbin")
					{
						ReadGuardFrameParam(_file);
					}
				}
				ccMain::ModCount++;
				ccMain::ModList.push_back(modName);
				ccMain::ModDesc.push_back(modDesc);
				ccMain::ModAuth.push_back(modAuth.substr(0, modAuth.length() - 1));
				ccMain::ModState.push_back(1);

				cout << "Enabling mod \"" << ModName << "\"" << endl;
			}
			else
			{
				cout << "Disabling mod \"" << ModName << "\" - Invalid info file." << endl;
			}
		}
		else
		{
			cout << "Disabling mod \"" << ModName << "\" - No info.txt file found." << endl;
		}
	}

	cout << "Finished reading mods" << endl;
}

void ReadGuardFrameParam(string _file)
{
	vector<BYTE> fileBytes = ReadAllBytes(_file);

	int slotCount = fileBytes.size() / 0x8;

	for (int x = 0; x < slotCount; x++)
	{
		int index = x * 0x8;
		int charaID = (fileBytes[index + 0] * 0x1) + (fileBytes[index + 1] * 0x100) + (fileBytes[index + 2] * 0x10000) + (fileBytes[index + 3] * 0x1000000);
		int timerNum = (fileBytes[index + 4] * 0x1) + (fileBytes[index + 5] * 0x100) + (fileBytes[index + 6] * 0x10000) + (fileBytes[index + 7] * 0x1000000);

		if (charaID == 0)
		{
			cout << "Set default blocking frames to " << timerNum << endl;
			for (int y = 0; y < 0xFFFF; y++)
			{
				ccPlayer::blockFrameTable[y] = timerNum;
			}
		}
		else
		{
			ccPlayer::blockFrameTable[charaID] = timerNum;
			cout << "Set " << timerNum << " frames to ID " << hex << charaID << endl;
		}
	}
}

#include "HookFunctions.h"
// LOAD CUSTOM CPK
static int CPKLOADER = -1;
//0xc0afa4
typedef int(__thiscall * fc_loadcpk)(int, size_t);
int ccMain::LoadCpk(int a, size_t s)
{
	if (CPKLOADER == -1)
	{
		cout << "Set CPKLOADER to " << std::hex << (uintptr_t)a << endl;
		CPKLOADER = a;
		//HookFunctions::UndoCpkHook();
	}

	cout << "holi" << endl;
	cout << hex << (void*)(xinput1_3::moduleBase + 0xE1F448) << endl;
	cout << hex << *(int*)(void*)(xinput1_3::moduleBase + 0xE1F448) << endl;
	cout << "a: " << hex << &a << endl;

	/*fc_loadcpk g_loadcpk = (fc_loadcpk)(xinput1_3::moduleBase + 0x657130);
	cout << "Loading: " << a << " > " << s << endl;*/
	return 0;
	//return g_loadcpk(a, s);
}

vector<BYTE> ReadAllBytes(string _file)
{
	ifstream f;
	f.open(_file);

	int FileSize = 0;
	while (!f.eof())
	{
		f.get();
		FileSize++;
	}

	f.close();
	f.open(_file, ios::binary);

	//cout << _file.c_str() << endl;
	std::vector<BYTE> result(FileSize);

	f.seekg(0, ios::beg);

	for (int x = 0; x < FileSize; x++)
	{
		BYTE a = f.get();
		memcpy((void*)&result[0 + x], &a, 1);
	}

	//f.read(&result[0], FileSize);

	f.close();

	return result;
}