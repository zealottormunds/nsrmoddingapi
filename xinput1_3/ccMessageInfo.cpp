#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>

#include "ccMessageInfo.h"
#include "xinput1_3.h"
#include "HookFunctions.h"
#include "ccUpdater.h"

using namespace moddingApi;
using namespace std;

// Initialize
void ccMessageInfo::Initialize()
{
	HookFunctions::InitializeHooks();
}

// GAME MESSAGE TO STRING
typedef char *(__cdecl * message_to_string)(char *);
message_to_string g_MessageToString;
char * ccMessageInfo::MessageToString(char * msg)
{
	g_MessageToString = (message_to_string)(xinput1_3::moduleBase + 0x58E950);
	return g_MessageToString(msg);
}

// GAME MESSAGE TO STRING ALT
typedef char *(__cdecl * message_to_string2)(char *);
message_to_string2 g_MessageToString2;
char * ccMessageInfo::MessageToString2(char * msg)
{
	//g_MessageToString2 = (message_to_string2)(xinput1_3::moduleBase + 0x58E990);
	g_MessageToString2 = (message_to_string2)(xinput1_3::moduleBase + 0x58E9C0);
	return g_MessageToString2(msg);
}

// Message conversions (ids to the equivalent of messageInfo)
bool ccMessageInfo::MessageConversions = true;

void ccMessageInfo::DisableMessageConversion()
{
	ccMessageInfo::MessageConversions = false;
}

void ccMessageInfo::EnableMessageConversion()
{
	ccMessageInfo::MessageConversions = true;
}

// For custom messages
vector<string> ccMessageInfo::ReadMessageFile(string _file)
{
	ifstream f;
	f.open(_file);

	vector<string> Messages;
	string actual = "";
	while (!f.eof())
	{
		char a = f.get();

		if (a != 0x00)
		{
			actual = actual + a;
		}
		else
		{
			//cout << "Push back " << actual << endl;
			Messages.push_back(actual);
			actual = "";
		}
	}

	f.close();

	return Messages;
}

vector<string> ccMessageInfo::MessageID;
vector<string> ccMessageInfo::MessageStr;
vector<string> ccMessageInfo::MessageID_ALT;
vector<string> ccMessageInfo::MessageStr_ALT;

uintptr_t ccMessageInfo::GetCustomMessage1(uintptr_t MessageToDecode)
{
	return ccMessageInfo::GetCustomMessage(MessageToDecode, false);
}

uintptr_t ccMessageInfo::GetCustomMessage2(uintptr_t MessageToDecode)
{
	return ccMessageInfo::GetCustomMessage(MessageToDecode, true);
}


// Custom message conversions
uintptr_t ccMessageInfo::GetCustomMessage(uintptr_t MessageToDecode, bool alt = false)
{
	if (ccMessageInfo::MessageConversions == 1 && strlen((char*)MessageToDecode) > 0 && *(char*)MessageToDecode != '+')
	{
		HookFunctions::UndoMessageInfoHook();
		char* result = ccMessageInfo::MessageToString((char*)MessageToDecode);
		HookFunctions::DoMessageInfoHook();

		if (alt)
		{
			HookFunctions::UndoMessageInfoHook2();
			result = ccMessageInfo::MessageToString2((char*)MessageToDecode);
			HookFunctions::DoMessageInfoHook2();
		}

		if (MessageToDecode != 0)
		{
			bool finished = false;

			string message((char*)MessageToDecode);

			if (message == "network_agreement_EU_s-A" || message == "network_agreement_s-A")
			{
				result = "lmao"; //(char*)GetModMessage().c_str();
				finished = true;
			}
			else if (message == "network_agreementp2_EU_s-A" || message == "network_agreementp2_s-A")
			{
				result = "";
				finished = true;
			}

			if (finished == false)
			{
				std::string msg = (std::string)(char*)MessageToDecode;

				if (msg == "network_agreement_ng") result = "<icon btn_2 />";
				else
				{
					//if (msg == "battleresult_002") result = "1P Health: %%%, 2P Health: %%%";
					if (msg == "MSG_AutoSaveWarn") result = (char*)(ccUpdater::CheckUpdates().c_str());
					else
					{
						if (alt == false)
						{
							for (int x = 0; x < MessageID.size(); x++)
							{
								if (msg == MessageID[x])
								{
									result = (char*)MessageStr[x].c_str();
								}
							}
						}
						else
						{
							for (int x = 0; x < MessageID_ALT.size(); x++)
							{
								if (msg == MessageID_ALT[x])
								{
									result = (char*)MessageStr_ALT[x].c_str();
								}
							}
						}
					}
				}
				if (msg != (std::string)(char*)MessageToDecode) result = (char*)msg.c_str();
			}
		}
		//cout << (char*)result << endl;

		return (uintptr_t)result;
	}
	else
	{
		if (*(char*)MessageToDecode == '+')
		{
			return (MessageToDecode + 1);
		}

		string m((char*)MessageToDecode);
		m = m + " [ALT]";

		if (alt) return (uintptr_t)(m.c_str());
		return MessageToDecode;
	}
}