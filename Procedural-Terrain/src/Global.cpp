#include "Global.h"

void Global::SetSeed()
{
	Seed = rand() % RAND_MAX;
}

Global::~Global()
{

}