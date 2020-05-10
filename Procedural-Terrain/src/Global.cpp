#include "Global.h"

int Global::Seed = 0;
bool Global::ToggleFrustum = false;
bool Global::FrustumCamera = false;
float Global::FOV = 45.0f;
float Global::Width = 1280.0f;
float Global::Height = 720.0f;
float Global::farDistance = 2000.0f;
void Global::SetSeed()
{
	Seed = rand() % 3000;
}