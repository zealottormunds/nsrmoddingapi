#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>

#include "HookFunctions.h"

#include "ccMain.h"
#include "xinput1_3.h"
#include "ccMessageInfo.h"
#include "ccPlayer.h"

using namespace moddingApi;
using namespace std;

// For messageInfo hook
BYTE originalMsgInfo[6];
BYTE originalMsgInfo2[6];

void BattleSelectParamHookAwakening();
void ExecuteActionHook();

// WRITE ALL THE FUNCTIONS YOU WANT TO HOOK IN HERE
void HookFunctions::InitializeHooks()
{
	memcpy(originalMsgInfo, (void*)(xinput1_3::moduleBase + 0x58E950), 6);
	HookFunctions::DoMessageInfoHook();
	//memcpy(originalMsgInfo2, (void*)(xinput1_3::moduleBase + 0x58E990), 6);
	memcpy(originalMsgInfo2, (void*)(xinput1_3::moduleBase + 0x58E9C0), 6);
	HookFunctions::DoMessageInfoHook2();
	ccPlayer::HookPlayerCreate();

	// For awakening select in charsel
	BattleSelectParamHookAwakening();
	//ExecuteActionHook();
}

void HookFunctions::DoMessageInfoHook()
{
	HookFunctions::Hook((void*)(xinput1_3::moduleBase + 0x58E950), (void*)ccMessageInfo::GetCustomMessage1, 6, false);
	//cout << "game: " << hex << xinput1_3::moduleBase + 0x58E950 << endl;
	//cout << "func: " << hex << ccMessageInfo::GetCustomMessage << endl;
}

void HookFunctions::UndoMessageInfoHook()
{
	DWORD dwOld = 0;
	VirtualProtect((void*)(xinput1_3::moduleBase + 0x58E950), 6, PAGE_EXECUTE_READWRITE, &dwOld);
	memcpy((void*)(xinput1_3::moduleBase + 0x58E950), originalMsgInfo, 6);
	VirtualProtect((void*)(xinput1_3::moduleBase + 0x58E950), 6, dwOld, &dwOld);
}

void HookFunctions::DoMessageInfoHook2()
{
	//HookFunctions::Hook((void*)(xinput1_3::moduleBase + 0x58E990), (void*)ccMessageInfo::GetCustomMessage2, 6, false);
	HookFunctions::Hook((void*)(xinput1_3::moduleBase + 0x58E9C0), (void*)ccMessageInfo::GetCustomMessage2, 6, false);
}

void HookFunctions::UndoMessageInfoHook2()
{
	/*DWORD dwOld = 0;
	VirtualProtect((void*)(xinput1_3::moduleBase + 0x58E990), 6, PAGE_EXECUTE_READWRITE, &dwOld);
	memcpy((void*)(xinput1_3::moduleBase + 0x58E990), originalMsgInfo2, 6);
	VirtualProtect((void*)(xinput1_3::moduleBase + 0x58E990), 6, dwOld, &dwOld);*/

	DWORD dwOld = 0;
	VirtualProtect((void*)(xinput1_3::moduleBase + 0x58E9C0), 6, PAGE_EXECUTE_READWRITE, &dwOld);
	memcpy((void*)(xinput1_3::moduleBase + 0x58E9C0), originalMsgInfo2, 6);
	VirtualProtect((void*)(xinput1_3::moduleBase + 0x58E9c0), 6, dwOld, &dwOld);
}

// Function to hook
bool HookFunctions::Hook(void * toHook, void * ourFunct, int len, bool call = false)
{
	if(call == false) MakeJMP((BYTE*)toHook, (DWORD)ourFunct, len);
	else MakeCALL((BYTE*)toHook, (DWORD)ourFunct, len);

	return true;
}

void HookFunctions::MakeJMP(BYTE *pAddress, DWORD dwJumpTo, DWORD dwLen)
{

	DWORD dwOldProtect, dwBkup, dwRelAddr;

	// give the paged memory read/write permissions

	VirtualProtect(pAddress, dwLen, PAGE_EXECUTE_READWRITE, &dwOldProtect);

	// calculate the distance between our address and our target location
	// and subtract the 5bytes, which is the size of the jmp
	// (0xE9 0xAA 0xBB 0xCC 0xDD) = 5 bytes

	dwRelAddr = (DWORD)(dwJumpTo - (DWORD)pAddress) - 5;

	// overwrite the byte at pAddress with the jmp opcode (0xE9)

	*pAddress = 0xE9;

	// overwrite the next 4 bytes (which is the size of a DWORD)
	// with the dwRelAddr

	*((DWORD *)(pAddress + 0x1)) = dwRelAddr;

	// overwrite the remaining bytes with the NOP opcode (0x90)
	// NOP opcode = No OPeration

	for (DWORD x = 0x5; x < dwLen; x++) *(pAddress + x) = 0x90;

	// restore the paged memory permissions saved in dwOldProtect

	VirtualProtect(pAddress, dwLen, dwOldProtect, &dwBkup);

	return;

}

void HookFunctions::MakeCALL(BYTE *pAddress, DWORD dwJumpTo, DWORD dwLen)
{
	cout << "Make call" << endl;
	DWORD dwOldProtect, dwBkup, dwRelAddr;

	// give the paged memory read/write permissions

	VirtualProtect(pAddress, dwLen, PAGE_EXECUTE_READWRITE, &dwOldProtect);

	// calculate the distance between our address and our target location
	// and subtract the 5bytes, which is the size of the jmp
	// (0xE9 0xAA 0xBB 0xCC 0xDD) = 5 bytes

	dwRelAddr = (DWORD)(dwJumpTo - (DWORD)pAddress) - 5;

	// overwrite the byte at pAddress with the jmp opcode (0xE9)

	*pAddress = 0xE8;

	// overwrite the next 4 bytes (which is the size of a DWORD)
	// with the dwRelAddr

	*((DWORD *)(pAddress + 0x1)) = dwRelAddr;

	// overwrite the remaining bytes with the NOP opcode (0x90)
	// NOP opcode = No OPeration

	for (DWORD x = 0x5; x < dwLen; x++) *(pAddress + x) = 0x90;

	// restore the paged memory permissions saved in dwOldProtect

	VirtualProtect(pAddress, dwLen, dwOldProtect, &dwBkup);

	return;

}

void HookFunctions::UndoCpkHook()
{
	DWORD dwOld = 0;
	VirtualProtect((void*)(xinput1_3::moduleBase + 0x657130), 6, PAGE_EXECUTE_READWRITE, &dwOld);
	memcpy((void*)(xinput1_3::moduleBase + 0x657130), new BYTE[6]{ 0x55, 0x8B, 0xEC, 0x8B, 0x55, 0x08 }, 6);
	VirtualProtect((void*)(xinput1_3::moduleBase + 0x657130), 6, dwOld, &dwOld);
}

// FORCE AWAKENING IN CHARSEL
int jmpback_BattleSelectParamHookAwakening_Action;
void __declspec(naked) BattleSelectParamHookAwakening_Action()
{
	//		mov ecx, 1
	__asm
	{
		mov [eax+38h], 1
		mov ecx, [eax+38h]
		push ecx
		mov ecx, ebx
		jmp[jmpback_BattleSelectParamHookAwakening_Action]
	}
}

void BattleSelectParamHookAwakening()
{
	jmpback_BattleSelectParamHookAwakening_Action = xinput1_3::moduleBase + 0x34AF2D + 6;
	HookFunctions::Hook((void*)(xinput1_3::moduleBase + 0x34AF2D), (void*)BattleSelectParamHookAwakening_Action, 6, false);
}

// HOOK EXECUTE ACTION
int HookFunctions::jmpback_ExecuteAction_Action;
void ExecuteActionHook()
{
	HookFunctions::jmpback_ExecuteAction_Action = xinput1_3::moduleBase + 0x4BFE90 + 5;
	HookFunctions::Hook((void*)(xinput1_3::moduleBase + 0x4BFE90), (void*)ccPlayer::ExecuteAction_Action, 5, false);
}