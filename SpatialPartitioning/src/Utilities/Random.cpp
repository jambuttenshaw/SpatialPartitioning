#include "Random.h"


std::mt19937* Random::s_RandomNumberGenerator = nullptr;
std::uniform_real_distribution<float> Random::s_NormamlizedFloatDistribution(0, 1);

void Random::Init()
{
	std::random_device rd();
	s_RandomNumberGenerator = new std::mt19937(rd);
}

void Random::Shutdown()
{
	delete s_RandomNumberGenerator;
}

float Random::RandomFloat(float max)
{
	return RandomFloat(0, max);
}
float Random::RandomFloat(float min, float max)
{
	std::uniform_real_distribution<float> dist(min, max);
	return dist(*s_RandomNumberGenerator);
}
float Random::RandomNormalizedFloat()
{
	return s_NormamlizedFloatDistribution(*s_RandomNumberGenerator);
}

Vector2f Random::RandomVector(Vector2f min, Vector2f max)
{
	return Vector2f{ RandomFloat(min.x, max.x), RandomFloat(min.y, max.y) };
}

Vector2f Random::RandomUnitVector()
{
	float angle = RandomFloat(2 * PI);
	return Vector2f{ cosf(angle), sinf(angle) };
}
