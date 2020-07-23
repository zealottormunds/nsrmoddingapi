#pragma once

#include <vector>
using namespace std;

namespace moddingApi
{
	class ccMain
	{
	public:
		static int GAMEVERSION;

		static DWORD WINAPI Main();
		static DWORD WINAPI Loop();
		static void ReadApiFiles();

		// Mod list variables:
		static int ModCount;
		static vector<std::string> ModList;
		static vector<std::string> ModDesc;
		static vector<std::string> ModAuth;
		static vector<BYTE> ModState;

		static int LoadCpk(int a, size_t s);
		static void LoadCpks();
	};
}