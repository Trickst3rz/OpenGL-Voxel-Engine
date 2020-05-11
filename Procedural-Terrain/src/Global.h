#pragma once

#include <stdlib.h>

class Global 
{
public:

	Global(const Global&) = delete;

	static Global& GetInstance()
	{
		static Global instance;
		return instance;
	}

	~Global();

	void SetSeed();

	int GetAmountOfChunks() { return AmountOfChunksX2; }

	void SetAmountOfChunks(int newSize) { AmountOfChunksX2 = newSize; }

	bool ToggleFrustum = false;
	bool FrustumCamera = false;
	bool OcclusionCulling = true;

	float FOV = 45.0f;
	float Width = 1280.0f;
	float Height = 720;
	float farDistance = 2000.0f;

	int Seed = 0;
	double Frequency = 1.0;
	int OctaveCount = 6;

private:
	Global() {}

	int AmountOfChunksX2 = 9;
};