#include <windows.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <filesystem>
#include <fstream>

#include "xinput1_3.h"
#include "ccMain.h"
#include "ccUpdater.h"

#pragma comment(lib, "urlmon.lib")

using namespace moddingApi;
using namespace std;

string getVersion(int v);

string ccUpdater::CheckUpdates()
{
	// Read local version
	char versionFilePath[_MAX_PATH];
	GetCurrentDirectory(_MAX_PATH, versionFilePath);
	strcat(versionFilePath, "\\data_win32\\version.txt");
	cout << versionFilePath << endl;
	if (filesystem::exists(versionFilePath))
	{
		ifstream localversion;
		localversion.open(versionFilePath);
		BYTE actual = 0;
		string versionString1 = "";
		int strlen = 0;
		while(localversion.eof() == false)
		{
			strlen++;
			char a = localversion.get();
			versionString1 = versionString1 + a;
		}

		localversion.close();

		try
		{
			ccMain::GAMEVERSION = stoi(versionString1);
		}
		catch (exception e)
		{
			return "Thanks for playing <color 0x9a25b8>UNS: Gev</color>. The file \"version.txt\" is corrupted. Please download the latest Gev build.";
		}
	}
	else
	{
		return "Thanks for playing <color 0x9a25b8>UNS: Gev</color>. The file \"version.txt\" could not be found. Please download the latest Gev build.";
	}

	// Download version file from the server
	string dwnld_URL = "https://pastebin.com/raw/rtU681Sw";
	char ApiPath[_MAX_PATH];
	GetCurrentDirectory(_MAX_PATH, ApiPath);
	int ActualLength = strlen(ApiPath);
	strcat(ApiPath, "\\gev_files\\");

	filesystem::create_directories(ApiPath);

	char ConfigPath[_MAX_PATH];
	strcpy(ConfigPath, ApiPath);
	strcat(ConfigPath, "version_server.txt");

	if (remove(ConfigPath) == 0)
	{
		cout << "version_server.txt removed correctly." << endl;
	}
	else
	{
		cout << "Couldn't remove version_server.txt" << endl;
	}

	URLDownloadToFile(NULL, dwnld_URL.c_str(), ConfigPath, 0, NULL);
	string versionString = "";
	string result = "";
	int ver = -1;

	if (filesystem::exists(ConfigPath))
	{
		ifstream file;
		file.open(ConfigPath);
		BYTE actual = 0;
		versionString = "";
		int strlen = 0;

		for(int x = 0; x < 3; x++)
		{
			strlen++;
			char a;
			a = file.get();
			versionString = versionString + a;
		}

		file.close();

		ver = stoi(versionString);
	}
	
	if (ver == -1)
	{
		result = "Couldn't retrieve data from the server.";
		return result;
	}

	cout << "Server: " << ver << endl;
	cout << "Game: " << ccMain::GAMEVERSION << endl;

	string versionLocal = "";
	if(ccMain::GAMEVERSION < 10)

	if (ver == ccMain::GAMEVERSION)
	{
		result = "<color 0x009cffff>Thank You</color> for <color 0x009cffff>Installing & Playing</color>\n                <color 0x8708c3ff>UNS: Gev!</color>\n\n           <color 0xc853ee>Current Build: v" + getVersion(ver) + "</color>\n             <color 0x009cffff>Your Build: v" + getVersion(ccMain::GAMEVERSION) + "</color>";
		//result = "<color 0x47c2ff>Thank You</color> for <color 0x47c2ff>Installing & Playing</color>\n";
		//result = result + "<color 0xda47ff>UNS: Gev</color> <color 0x47ff47>Version " + to_string(((float)ver)/10) + "</color>";
		//result = "Thanks for playing <color 0x9a25b8>UNS: Gev</color>. Your game is up to date with the current build.";
	}
	else
	{
		result = "<color 0x009cffff>Thank You</color> for <color 0x009cffff>Installing & Playing</color>\n                <color 0x8708c3ff>UNS: Gev!</color>\n\n";
		result = result + "      Your game is outdated.\n Please update to the current build.";
	}

	return result;
}

string getVersion(int v)
{
	string ver = to_string((float)v / 10);
	string ver2 = "";

	for (int x = 0; x < 3; x++)
	{
		ver2 = ver2 + ver[x];
	}

	return ver2;
}