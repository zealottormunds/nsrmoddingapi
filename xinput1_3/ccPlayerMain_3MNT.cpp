#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>

#include "xinput1_3.h"
#include "Vector3.h"
#include "ccPlayer.h"
#include "ccPlayerMain_3MNT.h"
#include "LuaCommands.h"

using namespace moddingApi;
using namespace std;

ccPlayerMain_3MNT::ccPlayerMain_3MNT(int c)
{
	maxSub = 25;
	totalSub = 25;
	init = false;
}

void ccPlayerMain_3MNT::Loop(int pln)
{
	ccPlayer c;

	int pl = pln;
	int en = 1;
	if (pl == 1) en = 0;

	DWORD p = c.GetPlayerInfo(pl);
	DWORD s = c.GetPlayerStatus(pl);
	DWORD ep = c.GetPlayerInfo(en);
	DWORD es = c.GetPlayerStatus(en);

	if (init == false)
	{
		c.SetMaxSubState(s, maxSub);
		c.SetSubState(s, totalSub);
		init = true;
		cout << "Init 3MNT " << pln << endl;
	}
	else
	{
		if (c.GetSubState(s) < c.GetMaxSubState(s))
		{
			c.SetMaxSubState(s, c.GetSubState(s));
		}

		/*if ((c.GetMaxSubState(s)) > maxSub)
		{
			c.SetMaxSubState(s, maxSub);
		}

		if ((c.GetSubState(s)) > totalSub)
		{
			c.SetSubState(s, totalSub);
		}
		else if (c.GetSubState(s) < totalSub)
		{
			totalSub = c.GetSubState(s);
		}*/
	}
}