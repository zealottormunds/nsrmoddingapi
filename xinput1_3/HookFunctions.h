#pragma once

#include <vector>
using namespace std;

namespace moddingApi
{
	class HookFunctions
	{
	public:
		static void InitializeHooks();

		static void DoMessageInfoHook();
		static void UndoMessageInfoHook();
		static void DoMessageInfoHook2();
		static void UndoMessageInfoHook2();

		static void MakeJMP(BYTE*, DWORD, DWORD);
		static void MakeCALL(BYTE*, DWORD, DWORD);
		static bool Hook(void*, void*, int, bool);

		static void UndoCpkHook();

		static int jmpback_ExecuteAction_Action;

		//static void DoHook(uintptr_t address, void* originalDest, int len, void* funct);
		//static void UndoHook(uintptr_t address, void* originalDest, int len);
	};
}