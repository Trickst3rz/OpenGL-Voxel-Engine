#include "Global.h"

int Global::Seed = 0;
bool Global::ToggleFrustum = false;
bool Global::FrustumCamera = false;

void Global::SetSeed()
{
	Seed = rand() % 3000;
}