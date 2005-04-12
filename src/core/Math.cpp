#include "Math.h"

float sinLut[SINCOS_TABLESIZE];
float cosLut[SINCOS_TABLESIZE];
float SINCOS_LOOKUP = SINCOS_TABLESIZE/(float)TWICE_PI;

void initMath()
{
	for (int n=0; n<SINCOS_TABLESIZE; n++)
	{
		float a=n*(TWICE_PI/(float)SINCOS_TABLESIZE);
		sinLut[n]=sin(a);
		cosLut[n]=cos(a);
	}
}

float uniform_bounded(float a,float b)
{
  return ((float)((rand() % 32000) / 32000.0f) * (b-a)) + a;
}
