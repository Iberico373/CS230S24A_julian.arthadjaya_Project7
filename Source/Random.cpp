//------------------------------------------------------------------------------
//
// File Name:	Random.c
// Author(s):	Julian Artha Djaya (julian.arthadjaya)
// Project:		Project 0
// Course:		CS230S24
//
// Copyright © 2024 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Random.h"
#include "time.h"

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Initialize the pseudorandom number generator (PRNG).
// You may use the example code from
//	 https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/rand.
// However, if you do so, then you must cite this source within the .c file.
void RandomInit()
{
	// Code used from  
	// https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/rand
	srand((unsigned int)(time(NULL)));
}

// Generate a random number in the interval [rangeMin, rangeMax] (inclusive).
// You may use the example code from
//	 https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/rand.
// However, if you do so, then you must cite this source within the .c file.
// Params:
//	 rangeMin = The lowest possible number generated.
//	 rangeMax = The highest possible number generated.
// Returns:
//   A random number in the interval [rangeMin, rangeMax].
int RandomRange(int rangeMin, int rangeMax)
{
	// Code used from
	// https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/rand
	int result = (int)(((float)rand() / (RAND_MAX + 1)) * (rangeMax - rangeMin + 1) + rangeMin);
	return result;
}

// Generate a random floating-point number in the interval [rangeMin, rangeMax] (inclusive).
// Params:
//	 rangeMin = The lowest possible number generated.
//	 rangeMax = The highest possible number generated.
float RandomRangeFloat(float rangeMin, float rangeMax)
{
	// Code used from
	// https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/rand
	float result = ((float)rand() / (RAND_MAX + 1)) * (rangeMax - rangeMin + 1) + rangeMin;
	return result;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

