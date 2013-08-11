#include <cstdlib>

float RandWithStep(float start, float end, float step)
{
	return ((rand() % int((end - start) * (1 / step))) * step) + start;
}