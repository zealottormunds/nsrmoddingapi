#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>

#include "xinput1_3.h"
#include "Vector3.h"
#include "ccPlayer.h"
#include "ccPlayerMain_2ITC.h"
#include "LuaCommands.h"

using namespace moddingApi;
using namespace std;

ccPlayerMain_2ITC::ccPlayerMain_2ITC(int c)
{
	chakraCharge = 0;

	// IA and CU1
	effectEnabled = false;
	originalMoveSpeed = -1;
	originalAnmSpeed = -1;
	prevEnemyHealth = -1;
	doneAwk = false;
	doneUj = false;
	frameCounterForUj = 0;
	removedLifeEffect = 0;
	framesToDoCU1 = 0;

	// Amaterasu
	framesToDoCD1 = 0;
	cd1_enableEffect = 0;
}

void Amaterasu(int pln, DWORD p, DWORD s, DWORD ep, DWORD es);

void ccPlayerMain_2ITC::Loop(int pln)
{
	ccPlayer c;

	int pl = pln;
	int en = 1;
	if (pl == 1) en = 0;

	DWORD p = c.GetPlayerInfo(pl);
	DWORD s = c.GetPlayerStatus(pl);
	DWORD ep = c.GetPlayerInfo(en);
	DWORD es = c.GetPlayerStatus(en);

	BYTE actualButton = c.GetCurrentButton(p);
 	BYTE actualAxis = c.GetCurrentAxis(p);

	chakraCharge = c.GetCanDoJutsuChakra(p);

	if (framesToDoCU1 > 0)
	{
		cout << "CU1: " << framesToDoCU1 << endl;
		framesToDoCU1--;
	}
	if (framesToDoCD1 > 0)
	{
		cout << "CD1: " << framesToDoCD1 << endl;
		framesToDoCD1--;
	}

	if (chakraCharge && actualButton == 0x20)
	{
		if (actualAxis == 0x10) framesToDoCU1 = 30;
		if (actualAxis == 0x20) framesToDoCD1 = 30;
	}

	/*if (actualButton == 0xA0 && actualAxis == 0x10)
	{
		cout << "cu1 10 frames" << endl;
		framesToDoCU1 = 30;
	}
	else if(framesToDoCU1 > 0) framesToDoCU1--;

	if (actualButton == 0xA0 && actualAxis == 0x20)
	{
		cout << "cd1 10 frames" << endl;
		framesToDoCD1 = 30;
	}
	else if (framesToDoCD1 > 0) framesToDoCD1--;*/

	////////////////////////////////////////////////////////////////////
	// Awakened bubble for tsukuyomi:
	////////////////////////////////////////////////////////////////////
	if (c.GetAwakenedState(p) == 1)
	{
		doneAwk = true;
		effectEnabled = true;
	}
	else
	{
		if (doneAwk == true) doneAwk = false;
		if(doneUj == false) effectEnabled = false;
	}

	if (cd1_enableEffect > 0) cd1_enableEffect - 1;

	if ((c.GetCurrentState(p) == 0x89 && c.GetPreviousState(p) == 0x88) || (c.GetCurrentState(p) == 0x40 && c.GetPreviousState(p) == 0x4E))
	{
		if (framesToDoCU1 > 0)
		{
			doneUj = true; // check for UJ and then count 30 * 9 frames = 270 (that's what the uj lasts)
		}
		if (framesToDoCD1 > 0 && cd1_enableEffect == 0)
		{
			cd1_enableEffect = 15;
			c.SetPlayerHealth(s, c.GetPlayerHealth(s) - 30);
		}
	}

	if (doneUj)
	{
		if (frameCounterForUj < 270)
		{
			frameCounterForUj = frameCounterForUj + 1;
		}
		else
		{
			effectEnabled = true;
			//cout << "Effect of tsukuyomi is enabled" << endl;
		}
	}

	if (effectEnabled)
	{
		playerSpeed = 1;
		float actualHealth = c.GetPlayerHealth(s);
		if (actualHealth <= 5) playerSpeed = playerSpeed - 0.7;
		else if (actualHealth <= 10) playerSpeed = playerSpeed - 0.60;
		else if (actualHealth <= 25) playerSpeed = playerSpeed - 0.45;
		else if (actualHealth <= 50) playerSpeed = playerSpeed - 0.25;
		c.SetPlayerMoveSpeed(p, playerSpeed);

		if (originalMoveSpeed == -1) originalMoveSpeed = c.GetPlayerMoveSpeed(ep);
		if (originalAnmSpeed == -1) originalAnmSpeed = c.GetPlayerAnmSpeed(ep);
		//effectEnabled = true;

		float distance = c.GetPlayerDistance(p, ep);
		float maxDist = 500;
		float minDist = 0;

		//if (distance <= maxDist && prevEnemyHealth != -1 && prevEnemyHealth == c.GetPlayerHealth(es) && c.GetCurrentState(ep) != 0x40 && c.GetCurrentState(ep) != 0x89)
		if (distance <= maxDist && prevEnemyHealth != -1 && prevEnemyHealth == c.GetPlayerHealth(es))
		{
			c.SetPlayerMoveSpeed(ep, 0.125);
			c.SetPlayerAnmSpeed(ep, 0.125);
		}
		else
		{
			c.SetPlayerMoveSpeed(ep, originalMoveSpeed);
			c.SetPlayerAnmSpeed(ep, originalAnmSpeed);
			originalMoveSpeed = -1;
			originalAnmSpeed = -1;
		}

		prevEnemyHealth = c.GetPlayerHealth(es);

		float plHealth = c.GetPlayerHealth(s);
		if (plHealth > 1)
		{
			if(doneAwk == false) c.SetPlayerHealth(s, plHealth - 0.05);
			else c.SetPlayerHealth(s, plHealth - 0.1);
		}
	}
	else
	{
		if (c.GetPlayerAnmSpeed(ep) != 1 || c.GetPlayerMoveSpeed(ep) != 1)
		{
			playerSpeed = 1;
			c.SetPlayerMoveSpeed(p, 1);

			c.SetPlayerMoveSpeed(ep, 1);
			c.SetPlayerAnmSpeed(ep, 1);
			originalMoveSpeed = -1;
			originalAnmSpeed = -1;
		}
	}

	if(cd1_enableEffect) Amaterasu(pln, p, s, ep, es);
}

void Amaterasu(int pln, DWORD p, DWORD s, DWORD ep, DWORD es)
{
	//cout << "amaterasu effect enabled" << endl;
}