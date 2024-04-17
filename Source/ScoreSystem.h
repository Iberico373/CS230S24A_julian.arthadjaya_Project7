//------------------------------------------------------------------------------
//
// File Name:	ScoreSystem.h
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

typedef struct BaseSystem BaseSystem;

//------------------------------------------------------------------------------
// Public Constants:
//------------------------------------------------------------------------------

typedef enum
{
	SsiInvalid = -1,
	SsiScore = 0,
	SsiHighScore,
	SsiWaveCount,
	SsiMax,
} ScoreSystemId;

//------------------------------------------------------------------------------
// Public Typedefs:
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

// Get the instance of the Score System.
BaseSystem* ScoreSystemGetInstance(void);

// Clear the Score System
// (Set the score, highScore, and waveCount variables to 0.)
void ScoreSystemClear(void);

// Reset the Score System.
// (Step 1: The highScore variable should be updated if score is higher.)
// (Step 2: The score and waveCount variables should be set to zero.)
void ScoreSystemReset(void);

// Get the specified value from the Score System.
// (HINT: There many different ways to handle this.  Two options include:
//   - Store the Score System variables in an array and use this index
//     to access into the array.
//   - Use a switch-statement to access the correct variable.)
// returns:
//   The value stored in the specified variable.
unsigned ScoreSystemGetValue(ScoreSystemId valueId);

// Increase the current score by a specified amount.
// Params:
//   amount = The amount to be added to the score.
void ScoreSystemIncreaseScore(unsigned amount);

// Increase the current wave count by one.
void ScoreSystemIncreaseWave(void);

/*----------------------------------------------------------------------------*/
