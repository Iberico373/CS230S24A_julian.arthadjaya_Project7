//------------------------------------------------------------------------------
//
// File Name:	Random.h
// Author(s):	Doug Schilling (dschilling)
// Project:		Project 5
// Course:		CS230S24
//
// Copyright © 2024 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Initialize the pseudorandom number generator (PRNG).
// You may use the example code from
//	 https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/rand.
// However, if you do so, then you must cite this source within the .c file.
void RandomInit();

// Generate a random number in the interval [rangeMin, rangeMax] (inclusive).
// You may use the example code from
//	 https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/rand.
// However, if you do so, then you must cite this source within the .c file.
// Params:
//	 rangeMin = The lowest possible number generated.
//	 rangeMax = The highest possible number generated.
// Returns:
//   A random number in the interval [rangeMin, rangeMax].
int RandomRange(int rangeMin, int rangeMax);

// Generate a random floating-point number in the interval [rangeMin, rangeMax] (inclusive).
// Params:
//	 rangeMin = The lowest possible number generated.
//	 rangeMax = The highest possible number generated.
float RandomRangeFloat(float rangeMin, float rangeMax);

//------------------------------------------------------------------------------