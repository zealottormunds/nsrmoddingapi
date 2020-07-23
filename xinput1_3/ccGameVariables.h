#pragma once

#include <vector>
using namespace std;

namespace moddingApi
{
	class ccMessageInfo
	{
	public:
		static bool IsInMenu();
		static void SetAnmSpeed(float);
		static void SetLightIntensity(float);
		static void SetBloomIntensity(float);
		static void SetOutlineWidth(float);
		static void SetRedTint(float);
		static void SetGreenTint(float);
		static void SetBlueTint(float);
		static void SetResolutionX(float);
		static void SetResolutionY(float);
		static void SetEnableShadows(bool);
	};
}