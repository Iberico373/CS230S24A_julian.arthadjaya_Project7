//------------------------------------------------------------------------------
//
// File Name:	BehaviorHudText.c
// Author(s):	Julian Artha Djaya (julian.arthadjaya)
// Project:		Project 0
// Course:		CS230S24
//
// Copyright © 2024 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "BehaviorHudText.h"
#include "Behavior.h"
#include "ScoreSystem.h"
#include "Stream.h"
#include "Sprite.h"
#include "Entity.h"

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

typedef enum HudTextStates
{
	cHudTextInvalid = -1,	// HUD Text has not yet been initialized.
	cHudTextIdle,			// HUD Text will normally remain static.

} HudTextStates;

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

typedef struct BehaviorHudText
{
	// Inherit the base behavior structure.
	Behavior	base;

	// Add HUD Text-specific behavior variables.

	// The index used to access values from the Score System.
	ScoreSystemId scoreSystemId;

	// The format string to be used with sprintf_s() when updating the HUD Text object.
	// (For example: "Score: %d")
	// (NOTE: This buffer has an arbitrary length of 32 chars, which is sufficient for this project.
	//	Exercise caution when using buffers of a fixed length (e.g. use sprintf_s).
	char formatString[32];

	// The buffer to be used with sprintf_s() when updating the HUD Text object.
	// (For example: "Score: 9001")
	// (NOTE: This buffer has an arbitrary length of 32 chars, which is sufficient for this project.
	//	Exercise caution when using buffers of a fixed length (e.g. use sprintf_s).
	char displayString[32];

	// The value currently being displayed by the HUD Text object.
	// (NOTE: This value can be compared with *watchValue to determine when the text must be updated.)
	// (NOTE: Make sure to update this value each time the text is updated.)
	unsigned displayValue;

} BehaviorHudText;

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

static void BehaviorHudTextInit(Behavior* behavior);
static void BehaviorHudTextUpdate(Behavior* behavior, float dt);
static void BehaviorHudTextExit(Behavior* behavior);
static void BehaviorHudTextUpdateText(BehaviorHudText* behavior);

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Dynamically allocate a new (HUD Text) behavior component.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
Behavior* BehaviorHudTextCreate(void)
{
	BehaviorHudText* behavior = (BehaviorHudText*)calloc(1, sizeof(BehaviorHudText));

	if (behavior)
	{
		behavior->base.memorySize = sizeof(BehaviorHudText);
		behavior->base.stateCurr = cHudTextInvalid;
		behavior->base.stateNext = cHudTextInvalid;
		behavior->base.onInit = &BehaviorHudTextInit;
		behavior->base.onUpdate = &BehaviorHudTextUpdate;
		behavior->base.onExit = &BehaviorHudTextExit;
		behavior->scoreSystemId = SsiInvalid;

		return (Behavior*)behavior;
	}

	return NULL;
}

// Read the properties of a Behavior component from a file.
// (NOTE: Read the base Behavior values using BehaviorRead.)
// (NOTE: Read the formatString using StreamReadToken.)
// (NOTE: Read the scoreSystemId using StreamReadInt.)
// Params:
//	 behavior = Pointer to the Behavior component.
//	 stream = Pointer to the data stream used for reading.
void BehaviorHudTextRead(Behavior* behavior, Stream stream)
{
	BehaviorHudText* hudText = (BehaviorHudText*)behavior;

	BehaviorRead(&hudText->base, stream);
	strcpy_s(hudText->formatString, _countof(hudText->formatString), StreamReadToken(stream));
	hudText->scoreSystemId = (ScoreSystemId)StreamReadInt(stream);
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

static void BehaviorHudTextInit(Behavior* behavior)
{
	BehaviorHudText* hudText = (BehaviorHudText*)behavior;

	BehaviorHudTextUpdateText(hudText);
	SpriteSetText(EntityGetSprite(behavior->parent), hudText->displayString);
}

static void BehaviorHudTextUpdate(Behavior* behavior, float dt)
{
	UNREFERENCED_PARAMETER(dt);

	BehaviorHudText* hudText = (BehaviorHudText*)behavior;
	if (hudText->displayValue != ScoreSystemGetValue(hudText->scoreSystemId))
		BehaviorHudTextUpdateText(hudText);
}

static void BehaviorHudTextExit(Behavior* behavior)
{
	UNREFERENCED_PARAMETER(behavior);
}

static void BehaviorHudTextUpdateText(BehaviorHudText* behavior)
{
	if (behavior->scoreSystemId == SsiInvalid)
		return;

	behavior->displayValue = ScoreSystemGetValue(behavior->scoreSystemId);
	sprintf_s(behavior->displayString, _countof(behavior->displayString), behavior->formatString, behavior->displayValue);
}