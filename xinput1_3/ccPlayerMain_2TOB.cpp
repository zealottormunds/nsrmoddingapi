#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>

#include "xinput1_3.h"
#include "Vector3.h"
#include "ccPlayer.h"
#include "ccPlayerMain_2TOB.h"
#include "LuaCommands.h"

using namespace moddingApi;
using namespace std;

ccPlayerMain_2TOB::ccPlayerMain_2TOB(int c)
{
	maxSub = 125;
	totalSub = 125;
	init = false;
}

void ccPlayerMain_2TOB::Loop(int pln)
{
	//cout << "started loop 1" << endl;
	ccPlayer c;

	int pl = pln;
	int en = 1;
	if (pl == 1) en = 0;

	DWORD p = c.GetPlayerInfo(pl);
	DWORD s = c.GetPlayerStatus(pl);
	DWORD ep = c.GetPlayerInfo(en);
	DWORD es = c.GetPlayerStatus(en);
	//cout << "started loop 2" << endl;

	if (init == false)
	{
		c.SetMaxSubState(s, maxSub);
		c.SetSubState(s, totalSub);
		//c.SetPlayerMaxHealth(s, 30);
		c.SetPlayerHealth(s, 30);
		init = true;
		//cout << "Init 2TOB " << pln << endl;
	}
	else
	{
		/*if (c.GetSubState(s) < c.GetMaxSubState(s))
		{
			c.SetMaxSubState(s, c.GetSubState(s));
		}*/

		if ((c.GetMaxSubState(s)) > maxSub)
		{
			c.SetMaxSubState(s, maxSub);
		}

		if ((c.GetSubState(s)) > totalSub)
		{
			c.SetSubState(s, totalSub);
		}
		else if (c.GetSubState(s) < totalSub)
		{
			//c.SetMaxSubState(s, c.GetSubState(s));
			totalSub = c.GetSubState(s);
		}	
	}
}