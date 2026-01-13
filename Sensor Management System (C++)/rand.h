#include <stdlib.h>

float getRandomNumber(float lowLimit, float highLimit)
{
	return ((float)random()/RAND_MAX * (highLimit-lowLimit)) + lowLimit;
}
