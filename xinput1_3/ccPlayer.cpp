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

int action_ = -1;
int plinfo = -1;
int action = -1;
int playerNum = -1;
DWORD pl = -1;
//DWORD jmpBackAddy = xinput1_3::moduleBase + 0x4BFE90 + 5;
DWORD jmpBackAddy = xinput1_3::moduleBase + 0x4BFE99 + 6;
void __declspec(naked) ccPlayer::ActionFunct()
{
	__asm
	{
		mov eax, [esi + 828h]
		jmp[jmpBackAddy]
	}
}

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
vector<float> ccPlayer::DisableGuardInPlayer(6, 0);
int PlayerCount = 0;
time_t PlayerAllocationTime;
bool PlayerAllocated = false;

void InitializeCharacters(int charaid, int plNum);
void DeleteCharacters(int c, int plNum);

#define CHECK_BIT(var,pos) (((var)>>(pos)) & 1)
int prevFrame = 0;
int prevBattle = 0;
void DoCharacterLoop(int c, int plNum);

// For control disable after counter:
vector<float> DisableControlAfterCounter(2, 0);
vector<float> HealthWhenCounterFail(2, 0);

void ccPlayer::Loop()
{
	if (isOnBattle() != prevBattle)
	{
		prevBattle = isOnBattle();
		if (prevBattle == 0)
		{
			cout << "Quit battle" << endl;

			for (int x = 0; x < 2; x++)
			{
				DWORD s = GetPlayerStatus(x);
				if (GetPlayerHealth(s) <= 0) return; // If player is dead or there's no player, stop code
				DWORD p = GetPlayerInfo(x);
				if (p == 0) return;
				int charaid = GetCharaID(p);

				DeleteCharacters(charaid, x);
			}
		}
		else
		{
			cout << "Entered battle" << endl;

			for (int x = 0; x < 2; x++)
			{
				DWORD s = GetPlayerStatus(x);
				if (GetPlayerHealth(s) <= 0) return; // If player is dead or there's no player, stop code
				DWORD p = GetPlayerInfo(x);
				if (p == 0) return;
				int charaid = GetCharaID(p);
				InitializeCharacters(charaid, x);
			}

			//DisableAllEffects();
		}
	}

	if (isOnBattle() == 0) return;

	//cout << ccPlayer::GetTimer() << endl;
	//	cout << "action: " << hex << action_ << ", player: " << playerNum << ", player: " << hex << pl << endl;

	// Disable guard
	for (int x = 0; x < 2; x++)
	{
		//DWORD p = ccPlayer::PlayerList[x];
		DWORD s = GetPlayerStatus(x);
		//cout << "s: " << hex << s << endl;

		if (GetPlayerHealth(s) <= 0) return; // If player is dead or there's no player, stop code

		DWORD p = GetPlayerInfo(x);
		//cout << "p: " << hex << p << endl;

		if (p == 0) return;

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// TAUNT
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*if (GetTauntState(p) == 0) SetTauntState(p, 1);

		if (GetCurrentButtonStartSelect(p) == 0x20)
		{
			SetTauntState(p, 0);
			ExecutePlayerAct(p, 0x3);
			//SetTauntState(p, 1);
		}*/

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// GUARD STUFF
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		if (DisableGuardInPlayer[x] > 0 && isOnMenu() == false && prevFrame != GetCurrentFrame())
		{
			DisableGuardInPlayer[x] = DisableGuardInPlayer[x] - 1;

			if (DisableGuardInPlayer[x] <= 0)
			{
				//EnableGuard(p);
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
			//DisableGuard(p);
			DisableGuardInPlayer[x] = GetBlockDisableFrames(GetCharaID(p));
			cout << "Player " << x << " can't guard for " << DisableGuardInPlayer[x] << " frames " << endl;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// TEST STUFF
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//SetPlayerSize(p, GetPlayerHealth(s) / 100);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		// Counter stuff:
		if (GetPreviousState(p) == 0xC0 && GetCurrentState(p) != 0xC1 && DisableControlAfterCounter[x] == 0)
		{
			ccPlayer::SetControlEnabled(p, 0);
			ccPlayer::ExecutePlayerAct(p, 0x0);
			ccPlayer::ExecutePlayerAct(p, 0x0);
			DisableControlAfterCounter[x] = 240;
			HealthWhenCounterFail[x] = GetPlayerHealth(s);
			cout << "Disabled control (counter failed) for P" << x << endl;
		}
		if (DisableControlAfterCounter[x] > 0 && prevFrame != GetCurrentFrame())
		{
			DisableControlAfterCounter[x] = DisableControlAfterCounter[x] - 1;

			// If player goes in a cutscene, enable the controller again
			bool inCutscene = false;
			int pCurrState = ccPlayer::GetCurrentState(p);
			int enemyId = 0;
			if (x == 0) enemyId = 1;
			int eCurrState = ccPlayer::GetCurrentState(GetPlayerInfo(enemyId));

			if (pCurrState == 0x50 || pCurrState == 0x89 || eCurrState == 0x50 || pCurrState == 0x89) inCutscene = true;

			if (inCutscene)
			{
				DisableControlAfterCounter[x] = 0;
			}

			cout << "Disable: " << DisableControlAfterCounter[x] << endl;

			// When timer of controller disable ends...
			if (DisableControlAfterCounter[x] <= 0)
			{
				DisableControlAfterCounter[x] = 0;
				ccPlayer::SetControlEnabled(p, 1);
				ccPlayer::ExecutePlayerAct(p, 0x2);
				ccPlayer::ExecutePlayerAct(p, 0x2);
				HealthWhenCounterFail[x] = -1;
				cout << "Enabled control for P" << x << endl;
			}
		}

		// Character specific
		if (isOnMenu() == false && prevFrame != GetCurrentFrame()) DoCharacterLoop(GetCharaID(p), x);
	}
	prevFrame = GetCurrentFrame();
}

#include "ccPlayerMain_2ITC.h"
#include "ccPlayerMain_3DNZ.h"
#include "ccPlayerMain_3MNT.h"
#include "ccPlayerMain_2TOB.h"
DWORD plMain[2] = { 0, 0 };
void DoCharacterLoop(int c, int plNum)
{
	DWORD plm = plMain[plNum];
	switch (c)
	{
	case 0x19:
		//ccPlayerMain_2ITC plm = *(ccPlayerMain_2ITC*)plMain[plNum];
		(*(ccPlayerMain_2ITC*)(plm)).Loop(plNum);
		break;
	case 0x3A:
		(*(ccPlayerMain_3DNZ*)(plm)).Loop(plNum);
		break;
	case 0x7E:
		(*(ccPlayerMain_3MNT*)(plm)).Loop(plNum);
		break;
	case 0x25:
		(*(ccPlayerMain_2TOB*)(plm)).Loop(plNum);
		break;
	}
}

void InitializeCharacters(int c, int plNum)
{
	DWORD plm = 0;
	switch (c)
	{
	case 0x19:
	{
		ccPlayerMain_2ITC *c_2itc = (new ccPlayerMain_2ITC(plNum));
		plm = (DWORD)(c_2itc);
		plMain[plNum] = plm;
		break;
	}
	case 0x3A:
	{
		ccPlayerMain_3DNZ *c_3dnz = (new ccPlayerMain_3DNZ(plNum));
		plm = (DWORD)(c_3dnz);
		plMain[plNum] = plm;
		break;
	}
	case 0x7E:
	{
		ccPlayerMain_3MNT *c_3mnt = (new ccPlayerMain_3MNT(plNum));
		plm = (DWORD)(c_3mnt);
		plMain[plNum] = plm;
		break;
	}
	case 0x25:
	{
		ccPlayerMain_2TOB *c_2tob = (new ccPlayerMain_2TOB(plNum));
		plm = (DWORD)(c_2tob);
		plMain[plNum] = plm;
		break;
	}
	}
	cout << "Created pl at " << hex << plm << "\n";
}

void DeleteCharacters(int c, int plNum)
{
	DWORD plm = plMain[plNum];
	switch (c)
	{
	case 0x19:
		delete &(*(ccPlayerMain_2ITC*)(plm));
		break;
	case 0x3A:
		delete &(*(ccPlayerMain_3DNZ*)(plm));
		break;
	case 0x7E:
		delete &(*(ccPlayerMain_3MNT*)(plm));
		break;
	case 0x25:
		delete &(*(ccPlayerMain_2TOB*)(plm));
		break;
	}
	cout << "Deleted pl at " << hex << plm << "\n";
	plMain[plNum] = 0;
}

int ccPlayer::GetPlayerStatusNumber(DWORD s)
{
	int n = -1;
	for (int x = 0; x < 2; x++)
	{
		if (s == ccPlayer::GetPlayerStatus(x)) n = x;
	}
	return n;
}

int ccPlayer::GetPlayerInfoNumber(DWORD p)
{
	int n = -1;
	for (int x = 0; x < 2; x++)
	{
		if (p == ccPlayer::GetPlayerInfo(x)) n = x;
	}
	return n;
}

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

int ccPlayer::GetTimer()
{
	int p1 = 0;
	int p2 = 0;
	int p3 = 0;
	int p4 = 0;
	int p5 = 0;

	//cout << "step 1" << endl;
	memcpy(&p1, (void*)(xinput1_3::moduleBase - 0xC00 + 0x936A28), 4);
	if (p1 == 0) return 0;
	//cout << hex << p1 << endl;
	//cout << "step 2" << endl;
	memcpy(&p2, (void*)(p1 + 0x30), 4);
	if (p2 == 0) return 0;
	//cout << hex << p2 << endl;
	//cout << "step 3" << endl;
	memcpy(&p3, (void*)(p2 + 0x1B0), 4);
	if (p3 == 0) return 0;
	//cout << hex << p3 << endl;
	//cout << "step 4" << endl;
	memcpy(&p4, (void*)(p3 + 0x20), 4);
	if (p4 == 0) return 0;
	//p4 = p3 + 0x10;
	//memcpy(&p4, (void*)(p3 + 0x10), 4);
	p5 = p4 + 0x2C;
	int timer = -1;
	memcpy(&timer, (void*)p5, 4);
	//if (p4 == 0) return 0;
	//cout << hex << p4 << endl;
	return timer;
}

int ccPlayer::GetCurrentFrame()
{
	int f = 0;
	memcpy(&f, (void*)(xinput1_3::moduleBase - 0xC00 + 0xA1F42C), 4);
	return f;
}

void ccPlayer::g_DisableControlFix()
{
	DWORD dwOld = 0;
	VirtualProtect((void*)(xinput1_3::moduleBase + 0x4D4F7D), 6, PAGE_EXECUTE_READWRITE, &dwOld);
	memcpy((void*)(xinput1_3::moduleBase + 0x4D4F7D), new BYTE[6]{ 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 }, 6);
	VirtualProtect((void*)(xinput1_3::moduleBase + 0x4D4F7D), 6, dwOld, &dwOld);
	cout << "NOP 0x4D4F7D" << endl;
}

int ccPlayer::ExecutePlayerAct(DWORD p, int act)
{
	typedef int(__thiscall * execute_pl_act)(DWORD, int);
	execute_pl_act g_execute_pl_act;
	cout << "Executed action " << hex << act << " in player " << hex << p << endl;
	g_execute_pl_act = (execute_pl_act)(xinput1_3::moduleBase + 0x4BFE90);
	return g_execute_pl_act(p, act);
}

void ccPlayer::Start()
{
	ccPlayer::g_DisableControlFix();
}

int ccPlayer::GetCharaID(DWORD p)
{
	int a = 0;
	memcpy(&a, (void*)(p + 0x7F0), 4);
	//cout << "charid: " << hex << a << endl;
	return a;
}

BYTE ccPlayer::GetCurrentAxis(DWORD p)
{
	BYTE a = 0;
	memcpy(&a, (void*)(p + 0x4C6), 1);
	return a;
}

void ccPlayer::SetCurrentAxis(DWORD p, BYTE b)
{
	DWORD dwOld = 0;
	VirtualProtect((void*)(p + 0x4C6), 1, PAGE_EXECUTE_READWRITE, &dwOld);
	memcpy((void*)(p + 0x4C6), new BYTE[1]{ b }, 1);
	VirtualProtect((void*)(p + 0x4C6), 1, dwOld, &dwOld);
}

BYTE ccPlayer::GetCurrentButton(DWORD p)
{
	BYTE a = 0;
	memcpy(&a, (void*)(p + 0x4C5), 1);
	return a;
}

void ccPlayer::SetCurrentButton(DWORD p, BYTE b)
{
	DWORD dwOld = 0;
	VirtualProtect((void*)(p + 0x4C5), 1, PAGE_EXECUTE_READWRITE, &dwOld);
	memcpy((void*)(p + 0x4C5), new BYTE[1]{ b }, 1);
	VirtualProtect((void*)(p + 0x4C5), 1, dwOld, &dwOld);
}

BYTE ccPlayer::GetCurrentButtonAlt(DWORD p)
{
	BYTE a = 0;
	memcpy(&a, (void*)(p + 0x4C1), 1);
	return a;
}

void ccPlayer::SetCurrentButtonAlt(DWORD p, BYTE b)
{
	DWORD dwOld = 0;
	VirtualProtect((void*)(p + 0x4C5), 1, PAGE_EXECUTE_READWRITE, &dwOld);
	memcpy((void*)(p + 0x4C5), new BYTE[1]{ b }, 1);
	VirtualProtect((void*)(p + 0x4C5), 1, dwOld, &dwOld);
}

BYTE ccPlayer::GetCurrentButtonStartSelect(DWORD p)
{
	BYTE a = 0;
	memcpy(&a, (void*)(p + 0x4C4), 1);
	return a;
}

void ccPlayer::SetCurrentButtonStartSelect(DWORD p, BYTE b)
{
	DWORD dwOld = 0;
	VirtualProtect((void*)(p + 0x4C4), 1, PAGE_EXECUTE_READWRITE, &dwOld);
	memcpy((void*)(p + 0x4C4), new BYTE[1]{ b }, 1);
	VirtualProtect((void*)(p + 0x4C4), 1, dwOld, &dwOld);
}

void ccPlayer::EnableGuard(DWORD p)
{
	//BYTE* guard = (BYTE*)(p + 0x5DA);
	DWORD dwOld = 0;
	VirtualProtect((void*)(p + 0x5DA), 1, PAGE_EXECUTE_READWRITE, &dwOld);
	memcpy((void*)(p + 0x5DA), new BYTE[1]{ 0x02 }, 1);
	VirtualProtect((void*)(p + 0x5DA), 1, dwOld, &dwOld);
}

void ccPlayer::DisableGuard(DWORD p)
{
	//BYTE* guard = (BYTE*)(p + 0x5DA);
	DWORD dwOld = 0;
	VirtualProtect((void*)(p + 0x5DA), 1, PAGE_EXECUTE_READWRITE, &dwOld);
	memcpy((void*)(p + 0x5DA), new BYTE[1]{ 0x00 }, 1);
	VirtualProtect((void*)(p + 0x5DA), 1, dwOld, &dwOld);
}

int ccPlayer::blockFrameTable[0xFFFF];
int ccPlayer::GetBlockDisableFrames(int chara)
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

float ccPlayer::GetPlayerMoveSpeed(DWORD p)
{
	float s;
	memcpy(&s, (void*)(p + 0x72EC), 4);
	return s;
}

void ccPlayer::SetPlayerMoveSpeed(DWORD p, float s)
{
	DWORD dwOld = 0;
	VirtualProtect((void*)(p + 0x72EC), 4, PAGE_EXECUTE_READWRITE, &dwOld);
	memcpy((void*)(p + 0x72EC), &s, 4);
	VirtualProtect((void*)(p + 0x72EC), 4, dwOld, &dwOld);
}

float ccPlayer::GetPlayerAnmSpeed(DWORD p)
{
	float s;
	memcpy(&s, (void*)(p + 0x1AC), 4);
	return s;
}

void ccPlayer::SetPlayerAnmSpeed(DWORD p, float s)
{
	DWORD dwOld = 0;
	VirtualProtect((void*)(p + 0x1AC), 4, PAGE_EXECUTE_READWRITE, &dwOld);
	memcpy((void*)(p + 0x1AC), &s, 4);
	VirtualProtect((void*)(p + 0x1AC), 4, dwOld, &dwOld);
}

BYTE ccPlayer::GetCurrentState(DWORD p)
{
	BYTE state;
	memcpy(&state, (void*)(p + 0x828), 1);
	return state;
}

BYTE ccPlayer::GetPreviousState(DWORD p)
{
	BYTE state;
	memcpy(&state, (void*)(p + 0x82C), 1);
	return state;
}

Vector3 ccPlayer::GetPlayerPosition(DWORD p)
{
	float x, y, z;
	memcpy(&x, (void*)(p + 0x8), 4);
	memcpy(&y, (void*)(p + 0xC), 4);
	memcpy(&z, (void*)(p + 0x10), 4);
	return Vector3(x, y, z);
}

float ccPlayer::GetPlayerDistance(DWORD p1, DWORD p2)
{
	return Vector3::Distance(GetPlayerPosition(p1), GetPlayerPosition(p2));
}

float ccPlayer::GetPlayerHealth(DWORD s)
{
	float h = 0;
	if (s != 0) memcpy(&h, (void*)(s), 4);
	//cout << "h: " << h << endl;
	return h;
}

void ccPlayer::SetPlayerHealth(DWORD s, float h)
{
	DWORD dwOld = 0;
	VirtualProtect((void*)(s), 4, PAGE_EXECUTE_READWRITE, &dwOld);
	memcpy((void*)(s), &h, 4);
	VirtualProtect((void*)(s), 4, dwOld, &dwOld);
}

float ccPlayer::GetPlayerMaxHealth(DWORD s)
{
	float h = 0;
	if (s != 0) memcpy(&h, (void*)(s + 0x4), 4);
	return h;
}

void ccPlayer::SetPlayerMaxHealth(DWORD s, float h)
{
	DWORD dwOld = 0;
	VirtualProtect((void*)(s + 0x4), 4, PAGE_EXECUTE_READWRITE, &dwOld);
	memcpy((void*)(s + 0x4), &h, 4);
	VirtualProtect((void*)(s + 0x4), 4, dwOld, &dwOld);
}

bool ccPlayer::GetAwakenedState(DWORD p)
{
	int a = 0;
	if (p != 0) memcpy(&a, (void*)(p + 0x824), 4);
	//cout << "h: " << h << endl;

	if (a == 1) return true;
	else return false;
}

float ccPlayer::GetPlayerSize(DWORD p)
{
	float s;
	memcpy(&s, (void*)(p + 0x1A0), 4);
	return s;
}

void ccPlayer::SetPlayerSize(DWORD p, float s)
{
	DWORD dwOld = 0;
	VirtualProtect((void*)(p + 0x1A0), 4, PAGE_EXECUTE_READWRITE, &dwOld);
	memcpy((void*)(p + 0x1A0), &s, 4);
	VirtualProtect((void*)(p + 0x1A0), 4, dwOld, &dwOld);
}

int ccPlayer::GetTauntState(DWORD p)
{
	int t;
	memcpy(&t, (void*)(p + 0x7898), 4);
	return t;
}

void ccPlayer::SetTauntState(DWORD p, int state)
{
	DWORD dwOld = 0;
	VirtualProtect((void*)(p + 0x7898), 4, PAGE_EXECUTE_READWRITE, &dwOld);
	memcpy((void*)(p + 0x7898), &state, 4);
	VirtualProtect((void*)(p + 0x7898), 4, dwOld, &dwOld);
}

float ccPlayer::GetMaxSubState(DWORD s)
{
	float t;
	memcpy(&t, (void*)(s + 0x1C), 4);
	return t;
}

void ccPlayer::SetMaxSubState(DWORD s, float state)
{
	DWORD dwOld = 0;
	VirtualProtect((void*)(s + 0x1C), 4, PAGE_EXECUTE_READWRITE, &dwOld);
	memcpy((void*)(s + 0x1C), &state, 4);
	VirtualProtect((void*)(s + 0x1C), 4, dwOld, &dwOld);
}

float ccPlayer::GetSubState(DWORD s)
{
	float t;
	memcpy(&t, (void*)(s + 0x18), 4);
	return t;
}

void ccPlayer::SetSubState(DWORD s, float state)
{
	DWORD dwOld = 0;
	VirtualProtect((void*)(s + 0x18), 4, PAGE_EXECUTE_READWRITE, &dwOld);
	memcpy((void*)(s + 0x18), &state, 4);
	VirtualProtect((void*)(s + 0x18), 4, dwOld, &dwOld);
}

BYTE ccPlayer::GetControlEnabled(DWORD p)
{
	BYTE t;
	memcpy(&t, (void*)(p + 0x814), 1);
	return t;
}

void ccPlayer::SetControlEnabled(DWORD p, BYTE state)
{
	DWORD dwOld = 0;
	VirtualProtect((void*)(p + 0x814), 1, PAGE_EXECUTE_READWRITE, &dwOld);
	memcpy((void*)(p + 0x814), &state, 1);
	VirtualProtect((void*)(p + 0x814), 1, dwOld, &dwOld);
}

int ccPlayer::GetCanDoJutsuChakra(DWORD p)
{
	BYTE t;
	memcpy(&t, (void*)(p + 0x5FC0), 4);
	return t;
}

void ccPlayer::SetCanDoJutsuChakra(DWORD p, int state)
{
	DWORD dwOld = 0;
	VirtualProtect((void*)(p + 0x5FC0), 4, PAGE_EXECUTE_READWRITE, &dwOld);
	memcpy((void*)(p + 0x5FC0), &state, 4);
	VirtualProtect((void*)(p + 0x5FC0), 4, dwOld, &dwOld);
}

void __declspec(naked) ccPlayer::ExecuteAction_Action()
{
	/*push ebx
	mov ebx, [esp+8]*/
	__asm
	{
		push ebx
		mov ebx, [esp+8]
		mov action_, ebx
		mov pl, ecx

		pushfd
		pushad
	}

	//cout << "action: " << hex << action_ << endl;
	playerNum = GetPlayerInfoNumber((DWORD)pl);
	if (action_ == 0x4A /*&& DisableGuardInPlayer[playerNum] > 0*/)
	{
		__asm
		{
			mov [esp+8], 1
			mov ebx, 1
		}
	}

	__asm
	{
		popad
		popfd
		jmp[HookFunctions::jmpback_ExecuteAction_Action]
	}
}

