// trigo.cpp - IMPORTANT : everything is in DEGREES !

#include "include/core/maths/trigo.h"
#include <iostream>
#define SIN_ARRAY_SIZE 72000

using namespace std;

static bool trigo_init = false;
static float sin_array[SIN_ARRAY_SIZE];
static float cos_array[SIN_ARRAY_SIZE];
static float asin_array[SIN_ARRAY_SIZE];
static float acos_array[SIN_ARRAY_SIZE];

void initTrigo()
{
	if(!trigo_init)
	{
		// Precalculus
		for(int i=0 ; i<SIN_ARRAY_SIZE ; i++)
			sin_array[i] = sin((2*M_PI) * float(i) / float(SIN_ARRAY_SIZE - 1.0)); // sin(0..2PI)

		for(int i=0 ; i<SIN_ARRAY_SIZE ; i++)
			cos_array[i] = cos((2*M_PI) * float(i) / float(SIN_ARRAY_SIZE - 1.0));	 // cos(0..2PI)

		for(int i=0 ; i<SIN_ARRAY_SIZE ; i++)
			asin_array[i] = 360.0 * asin(float(i)/float(SIN_ARRAY_SIZE-1)) / (2.0*M_PI);	// (360/2PI) * asin(0..1)

		for(int i=0 ; i<SIN_ARRAY_SIZE ; i++)
			acos_array[i] = 360.0 * acos(float(i)/float(SIN_ARRAY_SIZE-1)) / (2.0*M_PI);	// (360/2PI) * acos(0..1)

		//cout << "trigoInit : trigo functions baking takes " << 4*sizeof(sin_array) << " bytes of memory" << endl;

	}
	trigo_init = true;
}

float fastSin(float angle)
{
	int index = (int( float(SIN_ARRAY_SIZE - 1.0)*angle / 360.0) ) % SIN_ARRAY_SIZE;

	if(index >= 0)
		return sin_array[index];
	else
		return -sin_array[-index];
}

float fastCos(float angle)
{
	int index = (int( float(SIN_ARRAY_SIZE- 1.0)*angle / 360.0) ) % SIN_ARRAY_SIZE;

	if(index >= 0)
		return cos_array[index];
	else
		return cos_array[-index];
}

float fastAsin(float x)
{
	if(x >= -1.0 && x < 0.0)
		return -asin_array[ int(-x * float(SIN_ARRAY_SIZE-1)) ];
	else if(x >= 0.0 && x <= 1.0)
		return asin_array[ int(x*float(SIN_ARRAY_SIZE-1)) ];
	else
		return 0.0;
}

float fastAcos(float x)
{
	if(x >= -1.0 && x < 0.0)
		return 180.0 - acos_array[ int(-x*float(SIN_ARRAY_SIZE-1)) ];
	if(x >= 0.0 && x <= 1.0)
		return acos_array[ int(x*float(SIN_ARRAY_SIZE-1)) ];
	else
		return 0.0;
}
