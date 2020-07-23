#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>

#include "xinput1_3.h"
#include "Vector3.h"
#include "ccPlayer.h"
#include "ccPlayerMain_3DNZ.h"
#include "LuaCommands.h"

using namespace moddingApi;
using namespace std;

ccPlayerMain_3DNZ::ccPlayerMain_3DNZ(int c)
{
	totalSub = 25;
	maxSub = 100;
	subsLeft = 10;
	init = false;
	prevState = -1;
	prevSub = ccPlayer::GetMaxSubState(ccPlayer::GetPlayerStatus(c));
}

void ccPlayerMain_3DNZ::Loop(int pln)
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
		cout << "init 3dnz " << pln << endl;
	}
	else
	{
		if (c.GetMaxSubState(s) > maxSub)
		{
			c.SetMaxSubState(s, maxSub);
		}

		if (c.GetSubState(s) > totalSub)
		{
			c.SetSubState(s, totalSub);
		}

		//cout << "subs left: " << subsLeft << endl;

		if (c.GetSubState(s) < totalSub)
		{
			subsLeft = subsLeft - 1;
			if (subsLeft <= 0)
			{
				subsLeft = 0;
				totalSub = 0;
			}
			c.SetSubState(s, totalSub);
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

		/*if ((c.GetSubState(s)) < prevSub)
		{
			totalSub = totalSub - 25;
			if (totalSub < 0) totalSub = 0;

			c.SetSubState(s, totalSub);
		}

		prevSub = c.GetSubState(s);*/
	}
}