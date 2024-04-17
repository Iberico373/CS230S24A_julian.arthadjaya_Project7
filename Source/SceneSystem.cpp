//------------------------------------------------------------------------------
//
// File Name:	SceneSystem.c
// Author(s):	Julian Artha Djaya (julian.arthadjaya)
// Project:		Project 0
// Course:		CS230S24
//
// Copyright © 2024 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include <Windows.h>
#include <assert.h>

#include "BaseSystem.h"
#include "Level1Scene.h"
#include "AsteroidsScene.h"
#include "OmegaScene.h"
#include "Scene.h"
#include "SceneSystem.h"
#include "StubScene.h"
#include "Trace.h"

//------------------------------------------------------------------------------
// Libraries
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

typedef struct Scene Scene;

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

typedef struct SceneSystem
{
	// WARNING: The base class must always be included first.
	BaseSystem	base;

	// Add any system-specific variables second.
	const Scene* activeScene;
	const Scene* nextScene;
	bool isRestarting;

} SceneSystem;

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//  System-specific State functions:
static bool SceneSystemInit(void);
static void SceneSystemUpdate(float dt);
static void SceneSystemRender(void);
static void SceneSystemExit(void);

//------------------------------------------------------------------------------
// Instance Variable:
//------------------------------------------------------------------------------

static SceneSystem instance =
{
	{ "SceneSystem", SceneSystemInit, SceneSystemUpdate, SceneSystemRender, SceneSystemExit},
};

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Get the instance of the Scene System.
BaseSystem* SceneSystemGetInstance()
{
	// Initialize any system-specific variables here:

	// Return a reference to the instance of this system.
	return (BaseSystem*)&instance;
}

// Check if the Scene System is still running (a scene is active).
// Returns:
//   bool	True is a scene is active, false otherwise.
bool SceneSystemIsRunning(void)
{
	return (instance.activeScene || instance.nextScene);
}

// Instruct the Scene System to restart the active scene.
void SceneSystemRestart(void)
{
	instance.isRestarting = true;
}

// Set a new active scene, replacing any existing active scene.
// Params:
//   scene	Pointer to the new scene.
void SceneSystemSetNext(const Scene* scene)
{
	if (scene)
	{
		assert(SceneIsValid(scene) && "SceneSystemSetNext Error: A scene is missing one or more functions.");
	}

	if (instance.nextScene == scene)
		SceneRestart();
	
	else
		instance.nextScene = scene;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

bool SceneSystemIsChanging(void)
{
	return (instance.isRestarting || (instance.activeScene != instance.nextScene));
}

// Initialize the System.
static bool SceneSystemInit(void)
{
	// Set the starting (Default) scene.	
	SceneSystemSetNext(Level1SceneGetInstance());

	return true;
}

// Update the System.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
static void SceneSystemUpdate(float dt)
{
	// Check for a scene change.
	if (SceneSystemIsChanging())
	{
		// Exit the current scene.
		SceneExit(instance.activeScene);

		// TODO: Add support for Load and Unload.
		if (!instance.isRestarting)
		{
			// Unload active scene
			SceneUnload(instance.activeScene);

			// Update the active scene.
			instance.activeScene = instance.nextScene;

			// Load new active scene
			SceneLoad(instance.activeScene);
		}	

		// Clear restart flag
		instance.isRestarting = false;

		// Initialize the new scene.
		SceneInit(instance.activeScene);
	}

	// Update the active scene.
	SceneUpdate(instance.activeScene, dt);
}

// Render the System.
static void SceneSystemRender(void)
{
	// Render the active scene.
	SceneRender(instance.activeScene);
}

// Shutdown the System.
//   (HINT: Free any allocated resources and/or opened files).
static void SceneSystemExit(void)
{
}

