#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>

//using namespace moddingApi;
//using namespace std;

namespace moddingApi
{
	class ccPlayerMain_2ITC
	{
	private:
		int chakraCharge = false;

		// UJ
		bool effectEnabled = false;
		float originalMoveSpeed = -1;
		float originalAnmSpeed = -1;
		float prevEnemyHealth = -1;
		bool doneAwk = false;
		bool doneUj = false;
		int frameCounterForUj = 0;
		float removedLifeEffect = 0;
		float playerSpeed = 1;
		int framesToDoCU1 = 0;

		// Amaterasu
		int framesToDoCD1 = 0;
		int cd1_enableEffect = 0;
	public:
		ccPlayerMain_2ITC(int c);
		void Loop(int pln);
	};
}