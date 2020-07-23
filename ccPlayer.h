#pragma once

#include <vector>
using namespace std;

namespace moddingApi
{
	class ccPlayer
	{
	public:
		static int blockFrameTable[];

		static vector<int> PlayerList;

		static DWORD GetPlayerStatus(int n);
		static DWORD GetPlayerInfo(int n);

		static void HookPlayerCreate();
		static bool isOnMenu();
		static bool isOnBattle();
		static void Start();
		static void Loop();
	};
}

typedef struct playerInfo
{
	
};