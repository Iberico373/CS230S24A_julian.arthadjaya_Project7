//------------------------------------------------------------------------------
//
// File Name:	StubSystem.c
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
#include "StubSystem.h"

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

typedef struct StubSystem
{
	// WARNING: The base class must always be included first.
	BaseSystem	base;

	// Add any system-specific variables second.

} StubSystem;

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//  System-specific State functions:
static bool StubSystemInit(void);
static void StubSystemUpdate(float dt);
static void StubSystemRender(void);
static void StubSystemExit(void);

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

static StubSystem instance =
{
	{ "StubSystem", StubSystemInit, StubSystemUpdate, StubSystemRender, StubSystemExit},
};

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Get the instance of the Stub System.
BaseSystem* StubSystemGetInstance()
{
	// Initialize any system-specific variables here:

	// Return a reference to the instance of this system.
	return (BaseSystem*)&instance;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

// Initialize the System.
static bool StubSystemInit(void)
{
	return true;
}

// Update the System.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
static void StubSystemUpdate(float dt)
{
	// Tell the compiler that the 'dt' variable is unused.
	UNREFERENCED_PARAMETER(dt);
}

// Render the System.
static void StubSystemRender(void)
{
}

// Shutdown the System.
//   (HINT: Free any allocated resources and/or opened files).
static void StubSystemExit(void)
{
}

