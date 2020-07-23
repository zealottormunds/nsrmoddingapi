#pragma once

#include <vector>
using namespace std;

namespace moddingApi
{
	class Vector3
	{
	public:
		float x;
		float y;
		float z;
		
		Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z)
		{

		}

		static float Distance(Vector3 a, Vector3 b);
	};
}