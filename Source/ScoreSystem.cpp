//------------------------------------------------------------------------------
//
// File Name:	ScoreSystem.c
// Author(s):	Julian Artha Djaya (julian.arthadjaya)
// Project:		Project 0
// Course:		CS230S24
//
// Copyright © 2024 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include <Windows.h>

#include "BaseSystem.h"
#include "ScoreSystem.h"

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

typedef struct ScoreSystem
{
	// WARNING: The base class must always be included first.
	BaseSystem	base;

	// Add any system-specific variables second.
	unsigned score;
	unsigned highScore;
	unsigned waveCount;

} ScoreSystem;

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//  System-specific State functions:
static bool ScoreSystemInit(void);
static void ScoreSystemUpdate(float dt);
static void ScoreSystemRender(void);
static void ScoreSystemExit(void);

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

static ScoreSystem instance =
{
	{ "ScoreSystem", ScoreSystemInit, ScoreSystemUpdate, ScoreSystemRender, ScoreSystemExit},
};

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Get the instance of the Score System.
BaseSystem* ScoreSystemGetInstance()
{
	// Initialize any system-specific variables here:
	instance.score = 0;
	instance.highScore = 0;
	instance.waveCount = 0;

	// Return a reference to the instance of this system.
	return (BaseSystem*)&instance;
}

// Clear the Score System
// (Set the score, highScore, and waveCount variables to 0.)
void ScoreSystemClear(void)
{
	instance.score = 0;
	instance.highScore = 0;
	instance.waveCount = 0;
}

// Reset the Score System.
// (Step 1: The highScore variable should be updated if score is higher.)
// (Step 2: The score and waveCount variables should be set to zero.)
void ScoreSystemReset(void)
{
	if (instance.score > instance.highScore)
		instance.highScore = instance.score;

	instance.score = 0;
	instance.waveCount = 0;
}

// Get the specified value from the Score System.
// (HINT: There many different ways to handle this.  Two options include:
//   - Store the Score System variables in an array and use this index
//     to access into the array.
//   - Use a switch-statement to access the correct variable.)
// returns:
//   The value stored in the specified variable.
unsigned ScoreSystemGetValue(ScoreSystemId valueId)
{
	switch (valueId)
	{
	case SsiScore:
		return instance.score;
		break;

	case SsiHighScore:
		return instance.highScore;
		break;

	case SsiWaveCount:
		return instance.waveCount;
		break;

	default:
		break;
	}

	return 0;
}

// Increase the current score by a specified amount.
// Params:
//   amount = The amount to be added to the score.
void ScoreSystemIncreaseScore(unsigned amount)
{
	instance.score += amount;
}

// Increase the current wave count by one.
void ScoreSystemIncreaseWave(void)
{
	++instance.waveCount;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

// Initialize the System.
static bool ScoreSystemInit(void)
{
	return true;
}

// Update the System.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
static void ScoreSystemUpdate(float dt)
{
	// Tell the compiler that the 'dt' variable is unused.
	UNREFERENCED_PARAMETER(dt);
}

// Render the System.
static void ScoreSystemRender(void)
{
}

// Shutdown the System.
//   (HINT: Free any allocated resources and/or opened files).
static void ScoreSystemExit(void)
{
}

