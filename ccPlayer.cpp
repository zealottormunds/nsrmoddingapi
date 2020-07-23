#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>

#include "ccPlayer.h"
#include "HookFunctions.h"
#include "ccMain.h"
#include "xinput1_3.h"
#include "ccMessageInfo.h"
#include "Vector3.h"

using namespace moddingApi;
using namespace std;

void ClearPlayerList();
void AddPlayer(int p);
void HookPlayerCreate();

DWORD jmpBackAddy;
int actualPl = 0;
void __declspec(naked) player_1cmn_create()
{
	__asm
	{
		mov[esi + 8360h], ebx
		mov eax, esi
		pop esi
		pop ebx

		pushfd
		pushad
	}

	__asm
	{
		mov actualPl, eax
	}

	//AddPlayer(actualPl);

	__asm
	{
		popad
		popfd
		retn
	}
}

void ccPlayer::HookPlayerCreate()
{
	int len = 5;
	DWORD hookAddress = (xinput1_3::moduleBase + 0x4DAB7B);
	jmpBackAddy = hookAddress + len;
	HookFunctions::Hook((void*)(xinput1_3::moduleBase + 0x4DAB7B), player_1cmn_create, 5, false);
}

bool ccPlayer::isOnMenu()
{
	BYTE *b = (BYTE*)(xinput1_3::moduleBase - 0xC00 + 0x935240);
	return *b;
}

bool ccPlayer::isOnBattle()
{
	BYTE *b = (BYTE*)(xinput1_3::moduleBase - 0xC00 + 0x9354A0);
	return *b;
}

#include <time.h>
vector<int> ccPlayer::PlayerList(6, 0);
vector<bool> IsAcding(6, false);
vector<float> DisableGuardInPlayer(6, 0);
int PlayerCount = 0;
time_t PlayerAllocationTime;
bool PlayerAllocated = false;

DWORD ccPlayer::GetPlayerStatus(int n)
{
	int p1 = 0;
	int p2 = 0;
	int p3 = 0;
	int p4 = 0;

	int p11 = 0;
	int p12 = 0;
	int p13 = 0;
	int p14 = 0;

	switch (n)
	{
		default:
			//cout << "step 1" << endl;
			memcpy(&p1, (void*)(xinput1_3::moduleBase - 0xC00 + 0x9369F8), 4);
			if (p1 == 0) return 0;
			//cout << hex << p1 << endl;
			//cout << "step 2" << endl;
			memcpy(&p2, (void*)(p1 + 0x18), 4);
			if (p2 == 0) return 0;
			//cout << hex << p2 << endl;
			//cout << "step 3" << endl;
			memcpy(&p3, (void*)(p2 + 0x00), 4);
			if (p3 == 0) return 0;
			//cout << hex << p3 << endl;
			//cout << "step 4" << endl;
			p4 = p3 + 0x10;
			//memcpy(&p4, (void*)(p3 + 0x10), 4);
			if (p4 == 0) return 0;
			//cout << hex << p4 << endl;
			return p4;
		case 1:
			memcpy(&p11, (void*)(xinput1_3::moduleBase - 0xC00 + 0x9369F8), 4);
			if (p11 == 0) return 0;
			memcpy(&p12, (void*)(p11 + 0x18), 4);
			if (p12 == 0) return 0;
			memcpy(&p13, (void*)(p12 + 0x60), 4);
			if (p13 == 0) return 0;
			//memcpy(&p14, (void*)(p13 + 0x10), 4);
			p14 = p13 + 0x10;
			if (p14 == 0) return 0;
			return p14;
	}
}

DWORD ccPlayer::GetPlayerInfo(int n)
{
	//cout << "Getting player info of " << n << endl;
	DWORD a = ccPlayer::GetPlayerStatus(n);
	//cout << "a is " << hex << a << endl;
	DWORD b = (DWORD)a - 4;
	//cout << "b is " << hex << b << endl;
	DWORD c = 0;
	memcpy(&c, (void*)b, 4);
	//cout << "info: " << hex << c << endl;
	return c;
}

void ClearPlayerList()
{
	ccPlayer::PlayerList.clear();
	IsAcding.clear();
	DisableGuardInPlayer.clear();
	PlayerCount = 0;
	cout << "Cleared player list" << endl;
}

int GetCurrentFrame()
{
	int f = 0;
	memcpy(&f, (void*)(xinput1_3::moduleBase - 0xC00 + 0xA1F42C), 4);
	return f;
}

void AddPlayer(int p)
{
	time_t timeInSec_;
	time(&timeInSec_);

	if (PlayerAllocated && (long long)timeInSec_ - PlayerAllocationTime > 5)
	{
		ClearPlayerList();
	}

	cout << "Added player at " << hex << p << endl;
	PlayerAllocated = true;
	time(&PlayerAllocationTime);
	ccPlayer::PlayerList.push_back(p);
	IsAcding.push_back(false);
	DisableGuardInPlayer.push_back(0);
	PlayerCount++;
}

void g_DisableControlFix()
{
	DWORD dwOld = 0;
	VirtualProtect((void*)(xinput1_3::moduleBase + 0x4D4F7D), 6, PAGE_EXECUTE_READWRITE, &dwOld);
	memcpy((void*)(xinput1_3::moduleBase + 0x4D4F7D), new BYTE[6]{ 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 }, 6);
	VirtualProtect((void*)(xinput1_3::moduleBase + 0x4D4F7D), 6, dwOld, &dwOld);
	cout << "NOP 0x4D4F7D" << endl;
}

// EXECUTE PLAYER ACTION
typedef int (__thiscall * execute_pl_act)(DWORD, int);
execute_pl_act g_execute_pl_act;
int ExecutePlayerAct(DWORD p, int act)
{
	cout << "Executed action " << hex << act << " in player " << hex << p << endl;
	g_execute_pl_act = (execute_pl_act)(xinput1_3::moduleBase + 0x4BFE90);
	return g_execute_pl_act(p, act);
}

void ccPlayer::Start()
{
	g_DisableControlFix();
}

float GetPlayerHealth(DWORD s)
{
	float h = 0;
	if(s != 0) memcpy(&h, (void*)(s), 4);
	//cout << "h: " << h << endl;
	return h;
}

int GetCharaID(DWORD p)
{
	int a = 0;
	memcpy(&a, (void*)(p + 0x7F0), 4);
	//cout << "charid: " << hex << a << endl;
	return a;
}

BYTE GetCurrentAxis(DWORD p)
{
	BYTE a = 0;
	memcpy(&a, (void*)(p + 0x4C6), 1);
	return a;
}

void SetCurrentAxis(DWORD p, BYTE b)
{
	DWORD dwOld = 0;
	VirtualProtect((void*)(p + 0x4C6), 1, PAGE_EXECUTE_READWRITE, &dwOld);
	memcpy((void*)(p + 0x4C6), new BYTE[1]{ b }, 1);
	VirtualProtect((void*)(p + 0x4C6), 1, dwOld, &dwOld);
}

BYTE GetCurrentButton(DWORD p)
{
	BYTE a = 0;
	memcpy(&a, (void*)(p + 0x4C5), 1);
	return a;
}

void SetCurrentButton(DWORD p, BYTE b)
{
	DWORD dwOld = 0;
	VirtualProtect((void*)(p + 0x4C1), 1, PAGE_EXECUTE_READWRITE, &dwOld);
	memcpy((void*)(p + 0x4C1), new BYTE[1]{ b }, 1);
	VirtualProtect((void*)(p + 0x4C1), 1, dwOld, &dwOld);
}

BYTE GetCurrentButtonAlt(DWORD p)
{
	BYTE a = 0;
	memcpy(&a, (void*)(p + 0x4C1), 1);
	return a;
}

void SetCurrentButtonAlt(DWORD p, BYTE b)
{
	DWORD dwOld = 0;
	VirtualProtect((void*)(p + 0x4C5), 1, PAGE_EXECUTE_READWRITE, &dwOld);
	memcpy((void*)(p + 0x4C5), new BYTE[1]{ b }, 1);
	VirtualProtect((void*)(p + 0x4C5), 1, dwOld, &dwOld);
}

void EnableGuard(DWORD p)
{
	//BYTE* guard = (BYTE*)(p + 0x5DA);
	DWORD dwOld = 0;
	VirtualProtect((void*)(p + 0x5DA), 1, PAGE_EXECUTE_READWRITE, &dwOld);
	memcpy((void*)(p + 0x5DA), new BYTE[1]{ 0x02 }, 1);
	VirtualProtect((void*)(p + 0x5DA), 1, dwOld, &dwOld);
}

void DisableGuard(DWORD p)
{
	//BYTE* guard = (BYTE*)(p + 0x5DA);
	DWORD dwOld = 0;
	VirtualProtect((void*)(p + 0x5DA), 1, PAGE_EXECUTE_READWRITE, &dwOld);
	memcpy((void*)(p + 0x5DA), new BYTE[1]{0x00}, 1);
	VirtualProtect((void*)(p + 0x5DA), 1, dwOld, &dwOld);
}

int ccPlayer::blockFrameTable[0xFFFF];
int GetBlockDisableFrames(int chara)
{
	int frames = ccPlayer::blockFrameTable[chara];
	return frames;
	/*switch (chara)
	{
	
	default:
		return 7;
	case 0x31: // 1roc
		return 30;
	case 0x6A: // 4ssi
		return 4;
	case 0x7E: // 3mnt
		return 4;
	}*/
}

float GetPlayerMoveSpeed(DWORD p)
{
	float s;
	memcpy(&s, (void*)(p + 0x72EC), 4);
	return s;
}

void SetPlayerMoveSpeed(DWORD p, float s)
{
	DWORD dwOld = 0;
	VirtualProtect((void*)(p + 0x72EC), 4, PAGE_EXECUTE_READWRITE, &dwOld);
	memcpy((void*)(p + 0x72EC), &s, 4);
	VirtualProtect((void*)(p + 0x72EC), 4, dwOld, &dwOld);
}

float GetPlayerAnmSpeed(DWORD p)
{
	float s;
	memcpy(&s, (void*)(p + 0x1AC), 4);
	return s;
}

void SetPlayerAnmSpeed(DWORD p, float s)
{
	DWORD dwOld = 0;
	VirtualProtect((void*)(p + 0x1AC), 4, PAGE_EXECUTE_READWRITE, &dwOld);
	memcpy((void*)(p + 0x1AC), &s, 4);
	VirtualProtect((void*)(p + 0x1AC), 4, dwOld, &dwOld);
}

BYTE GetCurrentState(DWORD p)
{
	BYTE state;
	memcpy(&state, (void*)(p + 0x828), 1);
	return state;
}

BYTE GetPreviousState(DWORD p)
{
	BYTE state;
	memcpy(&state, (void*)(p + 0x82C), 1);
	return state;
}

Vector3 GetPlayerPosition(DWORD p)
{
	float x, y, z;
	memcpy(&x, (void*)(p + 0x8), 4);
	memcpy(&y, (void*)(p + 0xC), 4);
	memcpy(&z, (void*)(p + 0x10), 4);
	return Vector3(x, y, z);
}

float GetPlayerDistance(DWORD p1, DWORD p2)
{
	return Vector3::Distance(GetPlayerPosition(p1), GetPlayerPosition(p2));
}

#define CHECK_BIT(var,pos) (((var)>>(pos)) & 1)
int prevFrame = 0;
int prevBattle = 0;
void DoCharacterLoop(int c);
void ccPlayer::Loop()
{
	if (isOnBattle() != prevBattle)
	{
		prevBattle = isOnBattle();
		if (prevBattle == 0)
		{
			cout << "Quit battle" << endl;
			//ClearPlayerList();
		}
		else
		{
			cout << "Entered battle" << endl;
		}
	}

	if (isOnBattle() == 0) return;

	// Disable guard
	for (int x = 0; x < 2; x++)
	{
		//DWORD p = ccPlayer::PlayerList[x];
		DWORD s = GetPlayerStatus(x);
		//cout << "s: " << hex << s << endl;

		if (GetPlayerHealth(s) <= 0) return; // If player is dead or there's no player, stop code
		
		DWORD p = GetPlayerInfo(x);
		//cout << "p: " << hex << p << endl;

		if(p == 0) return;

		if (DisableGuardInPlayer[x] > 0 && isOnMenu() == false && prevFrame != GetCurrentFrame())
		{
			DisableGuardInPlayer[x] = DisableGuardInPlayer[x] - 1;

			BYTE axis = GetCurrentAxis(p);
			BYTE button = GetCurrentButton(p);
			bool isBlocking = (bool)CHECK_BIT(axis, 1);
			bool isPunching = false;

			if (button % 10 == 2) isPunching = true;

			//if (isBlocking) cout << "blocking" << endl;
			//if (isPunching) cout << "punching" << endl;

			if (isBlocking && isPunching)
			{
				int currButton = GetCurrentButton(p);

				ExecutePlayerAct(p, 0x01);
				EnableGuard(p);
				//ExecutePlayerAct(p, 0x49);

				SetCurrentAxis(p, 0x2);
				SetCurrentButton(p, 0x20);
				SetCurrentButtonAlt(p, 0x20);
			}
			else
			{
				DisableGuard(p);
			}

			if (DisableGuardInPlayer[x] <= 0)
			{
				EnableGuard(p);
				cout << "Player " << x << " can guard again" << endl;
			}
		}
		BYTE prev = GetPreviousState(p);
		if (GetCurrentState(p) == 0x11 && (prev == 0x5 || prev == 0x6 || prev == 0x7 || prev == 0x8 || prev == 0x9))
		{
			IsAcding[x] = true;
			cout << "Player " << x << " is ACDing" << endl;
		}
		if (IsAcding[x] == true && GetCurrentState(p) != 0x11)
		{
			IsAcding[x] = false;
			DisableGuard(p);
			//DisableGuardInPlayer[x] = 7;
			DisableGuardInPlayer[x] = GetBlockDisableFrames(GetCharaID(p));
			cout << "Player " << x << " can't guard for " << DisableGuardInPlayer[x] << " frames " << endl;
		}

		// Character specific
		DoCharacterLoop(GetCharaID(p));
	}
	prevFrame = GetCurrentFrame();
}

#include "ccPlayerMain_2ITC.cpp"
void DoCharacterLoop(int c)
{
	switch (c)
	{
		case 0x19:
			ccPlayerMain_2ITC::Loop();
		break;
	}
}