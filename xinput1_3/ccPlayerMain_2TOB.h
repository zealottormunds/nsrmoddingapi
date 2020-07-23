#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>

//using namespace moddingApi;
//using namespace std;

namespace moddingApi
{
	class ccPlayerMain_2TOB
	{
	private:
		float maxSub = 100;
		float totalSub = 100;
		bool init = false;
	public:
		ccPlayerMain_2TOB(int c);
		void Loop(int pln);
	};
}