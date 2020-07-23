#include <windows.h>
#include <stdio.h>

#include "xinput1_3.h"
#include "ccMain.h"

using namespace moddingApi;
using namespace std;

uintptr_t xinput1_3::moduleBase;
uintptr_t xinput1_3::st_hModule;
uintptr_t xinput1_3::baseAddressDetour = 0xAF1000;

HINSTANCE mHinst = 0, mHinstDLL = 0;
UINT_PTR mProcs[12] = {0};

LPCSTR mImportNames[] = {"DllMain", "XInputEnable", "XInputGetBatteryInformation", "XInputGetCapabilities", "XInputGetDSoundAudioDeviceGuids", "XInputGetKeystroke", "XInputGetState", "XInputSetState", (LPCSTR)100, (LPCSTR)101, (LPCSTR)102, (LPCSTR)103};
BOOL WINAPI DllMain( HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	mHinst = hinstDLL;
	if ( fdwReason == DLL_PROCESS_ATTACH )
	{
		mHinstDLL = LoadLibrary( "ori_xinput1_3.dll" );
		if ( !mHinstDLL ) return ( FALSE );
		for ( int i = 0; i < 12; i++ ) mProcs[ i ] = (UINT_PTR)GetProcAddress( mHinstDLL, mImportNames[ i ] );
		
		xinput1_3::st_hModule = (uintptr_t)hinstDLL;
		xinput1_3::moduleBase = (uintptr_t)GetModuleHandle(NULL) + 0xC00;
		//MessageBox(NULL, "NARUTO SHIPPUDEN: Ultimate Ninja Storm GEV", "", MB_OK);

		CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)ccMain::Main, hinstDLL, 0, nullptr));
	}
	else if ( fdwReason == DLL_PROCESS_DETACH )
	{
		FreeLibrary( mHinstDLL );
	}
	return ( TRUE );
}

extern "C" __declspec(naked) void __stdcall DllMain_wrapper(){__asm{jmp mProcs[0*4]}}
extern "C" __declspec(naked) void __stdcall XInputEnable_wrapper(){__asm{jmp mProcs[1*4]}}
extern "C" __declspec(naked) void __stdcall XInputGetBatteryInformation_wrapper(){__asm{jmp mProcs[2*4]}}
extern "C" __declspec(naked) void __stdcall XInputGetCapabilities_wrapper(){__asm{jmp mProcs[3*4]}}
extern "C" __declspec(naked) void __stdcall XInputGetDSoundAudioDeviceGuids_wrapper(){__asm{jmp mProcs[4*4]}}
extern "C" __declspec(naked) void __stdcall XInputGetKeystroke_wrapper(){__asm{jmp mProcs[5*4]}}
extern "C" __declspec(naked) void __stdcall XInputGetState_wrapper(){__asm{jmp mProcs[6*4]}}
extern "C" __declspec(naked) void __stdcall XInputSetState_wrapper(){__asm{jmp mProcs[7*4]}}
extern "C" __declspec(naked) void __stdcall ExportByOrdinal100(){__asm{jmp mProcs[8*4]}}
extern "C" __declspec(naked) void __stdcall ExportByOrdinal101(){__asm{jmp mProcs[9*4]}}
extern "C" __declspec(naked) void __stdcall ExportByOrdinal102(){__asm{jmp mProcs[10*4]}}
extern "C" __declspec(naked) void __stdcall ExportByOrdinal103(){__asm{jmp mProcs[11*4]}}
