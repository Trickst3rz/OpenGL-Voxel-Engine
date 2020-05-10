#pragma once

#include <stdlib.h>

class Global 
{
public:

	static void SetSeed();
	static int GetSeed() { return Seed; }

	static bool ToggleFrustum;
	static bool FrustumCamera;
	static float FOV;
	static float Width;
	static float Height;
	static float farDistance;
private:
	static int Seed;
};