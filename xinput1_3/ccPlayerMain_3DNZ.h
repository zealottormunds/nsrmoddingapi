#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>

//using namespace moddingApi;
//using namespace std;

namespace moddingApi
{
	class ccPlayerMain_3DNZ
	{
	private:
		float totalSub = 25;
		float maxSub = 250;
		int subsLeft = 10;
		bool init = false;
		int prevState = -1;
		float prevSub = 0;
	public:
		ccPlayerMain_3DNZ(int c);
		void Loop(int pln);
	};
}