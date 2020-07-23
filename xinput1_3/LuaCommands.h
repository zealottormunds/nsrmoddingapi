#pragma once

#include <vector>
using namespace std;

namespace moddingApi
{
	class LuaCommands
	{
	public:
		static char *__stdcall ccAddMoney(int quantity);
		static signed int __cdecl changeMode(int a1);
		static int __cdecl ccCreate3dObj(char *a1, char* a2, int a3, int a4);
		static int __cdecl ccCreateFilter(char *a1, char *a2);
		static int __cdecl ccStartScreenFilter(int a1);

		static int __cdecl ccStartString(int a1, char* a2); // 423880
		static int __cdecl ccCreateString(char* a1); // 423D50
		static int __cdecl ccSetStringId(char* a1, char* a2); // 423DC0
		static int __cdecl ccStringPosition(char* a1, float a2, float a3, float a4, float a5); // 4238F0
		static int __cdecl ccStringScale(char* a1, float a2, float a3, float a4, float a5); // 423990

		static int fc_isPtCutscene();
		static void __cdecl ccAdvChangeDemoSettingState(char* a1, char* a2, char* a3, float a4, float a5);
	};
}