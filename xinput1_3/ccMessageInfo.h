#pragma once

#include <vector>
using namespace std;

namespace moddingApi
{
	class ccMessageInfo
	{
	public:
		static void Initialize();

		static char* MessageToString(char *);
		static char* MessageToString2(char *);

		static bool MessageConversions;
		static void EnableMessageConversion();
		static void DisableMessageConversion();

		static vector<string> ReadMessageFile(string);
		static vector<string> MessageID;
		static vector<string> MessageStr;
		static vector<string> MessageID_ALT;
		static vector<string> MessageStr_ALT;

		static uintptr_t GetCustomMessage1(uintptr_t);
		static uintptr_t GetCustomMessage2(uintptr_t);
		static uintptr_t GetCustomMessage(uintptr_t, bool);
	};
}