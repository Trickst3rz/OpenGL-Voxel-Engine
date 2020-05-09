#pragma once

#include <stdlib.h>

class Global 
{
public:

	static void SetSeed();
	static int GetSeed() { return Seed; }

	static bool ToggleFrustum;
	static bool FrustumCamera;
private:
	static float FOV;
	static int Seed;
};