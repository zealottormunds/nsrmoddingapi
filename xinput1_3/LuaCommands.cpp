#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>

#include "xinput1_3.h"
#include "LuaCommands.h"

using namespace std;
using namespace moddingApi;

char * __stdcall LuaCommands::ccAddMoney(int quantity)
{
	typedef char *(__stdcall * fc_ccAddMoney)(int quantity);
	fc_ccAddMoney cc_ccAddMoney;
	cc_ccAddMoney = (fc_ccAddMoney)(xinput1_3::moduleBase + 0x135AB0);
	return cc_ccAddMoney(quantity);
}

signed int __cdecl LuaCommands::changeMode(int a1)
{
	typedef int (__cdecl * fc_changeMode)(int a1);
	fc_changeMode cc_changeMode;
	cc_changeMode = (fc_changeMode)(xinput1_3::moduleBase + 0x427A60);
	return cc_changeMode(a1);
}

int __cdecl LuaCommands::ccCreate3dObj(char *a1, char* a2, int a3, int a4)
{
	typedef int(__cdecl * fc_create3dobj)(char *a1, char* a2, int a3, int a4);
	fc_create3dobj cc_create3dobj;
	cc_create3dobj = (fc_create3dobj)(xinput1_3::moduleBase + 0x420730);
	return cc_create3dobj(a1, a2, a3, a4);
}

int __cdecl LuaCommands::ccCreateFilter(char *a1, char *a2)
{
	typedef int(__cdecl * funct)(char *a1, char* a2);
	funct cc_funct;
	cc_funct = (funct)(xinput1_3::moduleBase + 0x4214B0);
	return cc_funct(a1, a2);
}

int __cdecl LuaCommands::ccStartScreenFilter(int a1)
{
	typedef int(__cdecl * funct)(int a1);
	funct cc_funct;
	cc_funct = (funct)(xinput1_3::moduleBase + 0x4203D0);
	return cc_funct(a1);
}

int __cdecl LuaCommands::ccStartString(int a1, char* a2)
{
	typedef int(__cdecl * funct)(int a1, char* a2);
	funct cc_funct;
	cc_funct = (funct)(xinput1_3::moduleBase + 0x423880);
	return cc_funct(a1, a2);
}

int __cdecl LuaCommands::ccCreateString(char* a1)
{
	typedef int(__cdecl * funct)(char* a1);
	funct cc_funct;
	cc_funct = (funct)(xinput1_3::moduleBase + 0x423D50);
	return cc_funct(a1);
}

int __cdecl LuaCommands::ccSetStringId(char* a1, char* a2)
{
	typedef int(__cdecl * funct)(char* a1, char* a2);
	funct cc_funct;
	cc_funct = (funct)(xinput1_3::moduleBase + 0x423DC0);
	return cc_funct(a1, a2);
}

int __cdecl LuaCommands::ccStringPosition(char* a1, float a2, float a3, float a4, float a5)
{
	typedef int(__cdecl * funct)(char* a1, float a2, float a3, float a4, float a5);
	funct cc_funct;
	cc_funct = (funct)(xinput1_3::moduleBase + 0x4238F0);
	return cc_funct(a1, a2, a3, a4, a5);
}

int __cdecl LuaCommands::ccStringScale(char* a1, float a2, float a3, float a4, float a5)
{
	typedef int(__cdecl * funct)(char* a1, float a2, float a3, float a4, float a5);
	funct cc_funct;
	cc_funct = (funct)(xinput1_3::moduleBase + 0x423990);
	return cc_funct(a1, a2, a3, a4, a5);
}

int LuaCommands::fc_isPtCutscene()
{
	typedef int(* funct)();
	funct cc_funct;
	cc_funct = (funct)(xinput1_3::moduleBase + 0x418EC0);
	return cc_funct();
}

void __cdecl LuaCommands::ccAdvChangeDemoSettingState(char* a1, char* a2, char* a3, float a4, float a5)
{
	typedef void(__cdecl * funct)(char* a1, char* a2, char* a3, float a4, float a5);
	funct cc_funct;
	cc_funct = (funct)(xinput1_3::moduleBase + 0xAE120);
	return cc_funct(a1, a2, a3, a4, a5);
}