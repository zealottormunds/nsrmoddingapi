#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include <math.h>

#include "Vector3.h"

using namespace std;
using namespace moddingApi;

float Vector3::Distance(Vector3 a, Vector3 b)
{
	float d = sqrtf(pow((b.x - a.x), 2) + pow((b.y - a.y), 2) + pow((b.z - a.z), 2));
	return d;
}