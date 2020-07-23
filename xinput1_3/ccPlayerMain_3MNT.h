#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>

//using namespace moddingApi;
//using namespace std;

namespace moddingApi
{
	class ccPlayerMain_3MNT
	{
	private:
		float maxSub = 25;
		float totalSub = 25;
		bool init = false;
	public:
		ccPlayerMain_3MNT(int c);
		void Loop(int pln);
	};
}