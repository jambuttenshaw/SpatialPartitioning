#pragma once

#include <random>

#include "Utilities/Constants.h"
#include "Utilities/Vector2.h"


class Random
{
public:
	static void Init();
	static void Shutdown();

	// generate a random float between 0 and max
	static float RandomFloat(float max);
	// generate a random float between min and max
	static float RandomFloat(float min, float max);
	// generate a random float between 0 and 1
	static float RandomNormalizedFloat();

	static Vector2f RandomVector(Vector2f min, Vector2f max);
	static Vector2f RandomUnitVector();

private:
	// the pseudo random number generator used
	static std::mt19937* s_RandomNumberGenerator;
	// normalized floats are always in the same range so we can pre-define a uniform distribution of them
	static std::uniform_real_distribution<float> s_NormamlizedFloatDistribution;
};
