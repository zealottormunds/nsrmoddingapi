#pragma once

#include <vector>
#include "Vector3.h"

using namespace std;

namespace moddingApi
{
	class ccPlayer
	{
	public:
		static int blockFrameTable[];

		static vector<int> PlayerList;
		static vector<float> DisableGuardInPlayer;

		static int GetPlayerStatusNumber(DWORD p);
		static int GetPlayerInfoNumber(DWORD p);

		static DWORD GetPlayerStatus(int n);
		static DWORD GetPlayerInfo(int n);

		static int GetTimer();

		///////////////////////////////////////////////////////////////////
		// PLAYER FUNCTIONS
		///////////////////////////////////////////////////////////////////
		static int GetCurrentFrame();
		static void g_DisableControlFix();
		static int ExecutePlayerAct(DWORD p, int act);

		static int GetCharaID(DWORD p);

		static BYTE GetCurrentAxis(DWORD p);
		static void SetCurrentAxis(DWORD p, BYTE b);
		static BYTE GetCurrentButton(DWORD p);
		static void SetCurrentButton(DWORD p, BYTE b);
		static BYTE GetCurrentButtonAlt(DWORD p);
		static void SetCurrentButtonAlt(DWORD p, BYTE b);
		static BYTE ccPlayer::GetCurrentButtonStartSelect(DWORD p);
		static void ccPlayer::SetCurrentButtonStartSelect(DWORD p, BYTE b);

		static void EnableGuard(DWORD p);
		static void DisableGuard(DWORD p);
		static int GetBlockDisableFrames(int chara);

		static float GetPlayerMoveSpeed(DWORD p);
		static void SetPlayerMoveSpeed(DWORD p, float s);
		static float GetPlayerAnmSpeed(DWORD p);
		static void SetPlayerAnmSpeed(DWORD p, float s);

		static BYTE GetCurrentState(DWORD p);
		static BYTE GetPreviousState(DWORD p);

		static Vector3 GetPlayerPosition(DWORD p);
		static float GetPlayerDistance(DWORD p1, DWORD p2);

		static float GetPlayerHealth(DWORD s);
		static void SetPlayerHealth(DWORD s, float h);
		static float GetPlayerMaxHealth(DWORD s);
		static void SetPlayerMaxHealth(DWORD s, float h);

		static bool GetAwakenedState(DWORD p);

		static float GetPlayerSize(DWORD p);
		static void SetPlayerSize(DWORD p, float s);

		static int GetTauntState(DWORD p);
		static void SetTauntState(DWORD p, int state);

		static float GetMaxSubState(DWORD s);
		static void SetMaxSubState(DWORD s, float state);
		static float GetSubState(DWORD s);
		static void SetSubState(DWORD s, float state);

		static BYTE GetControlEnabled(DWORD p);
		static void SetControlEnabled(DWORD p, BYTE state);

		static int GetCanDoJutsuChakra(DWORD p);
		static void SetCanDoJutsuChakra(DWORD p, int state);

		static void ExecuteAction_Action();

		///////////////////////////////////////////////////////////////////

		static void HookPlayerCreate();
		static bool isOnMenu();
		static bool isOnBattle();
		static void Start();
		static void Loop();

		static void ActionFunct();
	};
}

typedef struct playerInfo
{
	
};