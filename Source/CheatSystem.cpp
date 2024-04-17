//------------------------------------------------------------------------------
//
// File Name:	CheatSystem.c
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
#include "CheatSystem.h"
#include "SceneSystem.h"
#include "Level1Scene.h"
#include "Level2Scene.h"
#include "AsteroidsScene.h"
#include "OmegaScene.h"
#include "DemoScene.h"
#include "SandboxScene.h"
#include "DGL.h"

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

typedef struct CheatSystem
{
	// WARNING: The base class must always be included first.
	BaseSystem	base;

	// Add any system-specific variables second.

} CheatSystem;

typedef struct CheatSystemKeyBinding
{
	char key;
	const Scene* (*getInstance)(void);

} CheatSystemKeyBinding;

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//  System-specific State functions:
static bool CheatSystemInit(void);
static void CheatSystemUpdate(float dt);
static void CheatSystemRender(void);
static void CheatSystemExit(void);

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

static CheatSystem instance =
{
	{ "CheatSystem", CheatSystemInit, CheatSystemUpdate, CheatSystemRender, CheatSystemExit},
};

static const CheatSystemKeyBinding keyBindings[] = { {'1', Level1SceneGetInstance}, {'2', Level2SceneGetInstance}, {'3', AsteroidsSceneGetInstance}, {'4', OmegaSceneGetInstance}, {'9', SandboxSceneGetInstance}, {'0', DemoSceneGetInstance}};

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Get the instance of the Stub System.
BaseSystem* CheatSystemGetInstance()
{
	// Initialize any system-specific variables here:

	// Return a reference to the instance of this system.
	return (BaseSystem*)&instance;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

// Initialize the System.
static bool CheatSystemInit(void)
{
	return true;
}

// Update the System.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
static void CheatSystemUpdate(float dt)
{
	// Tell the compiler that the 'dt' variable is unused.
	UNREFERENCED_PARAMETER(dt);

	for (int i = 0; i < 6; ++i)
	{
		if (DGL_Input_KeyTriggered(keyBindings[i].key))
		{
			SceneSystemSetNext(keyBindings[i].getInstance());
		}
	}
}

// Render the System.
static void CheatSystemRender(void)
{
}

// Shutdown the System.
//   (HINT: Free any allocated resources and/or opened files).
static void CheatSystemExit(void)
{
}

