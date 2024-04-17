//------------------------------------------------------------------------------
//
// File Name:	OmegaScene.c
// Author(s):	Doug Schilling (dschilling)
// Project:		Project 0
// Course:		CS230S24
//
// Copyright © 2024 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "DGL.h"
#include "Scene.h"
#include "SceneSystem.h"
#include "ScoreSystem.h"
#include "OmegaScene.h"
#include "Entity.h"
#include "EntityFactory.h"

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

typedef struct OmegaScene
{
	// WARNING: The base class must always be included first.
	Scene	base;

	// Add any scene-specific variables second.
	unsigned OmegapawnCount;

} OmegaScene;

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

static const unsigned cOmegapawnInitial = 8;
static const unsigned cOmegapawnMaximum = 20;

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

const DGL_Color* arenaColor = &(DGL_Color){ 1.0f, 1.0f, 0.0f, 1.0f };

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

static void OmegaSceneLoad(void);
static void OmegaSceneInit(void);
static void OmegaSceneUpdate(float dt);
static void OmegaSceneExit(void);
static void OmegaSceneUnload(void);
static void OmegaSceneRender(void);
static void OmegaSceneSpawnAsteroidWave(void);
static void OmegaSceneSpawnAsteroid(void);


//------------------------------------------------------------------------------
// Instance Variable:
//------------------------------------------------------------------------------

static OmegaScene instance =
{
	// Initialize the base structure:
	{ "OmegaScene", OmegaSceneLoad, OmegaSceneInit, OmegaSceneUpdate, OmegaSceneRender, OmegaSceneExit, OmegaSceneUnload },

	// Initialize any scene-specific variables:
};

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

const Scene* OmegaSceneGetInstance(void)
{
	return &(instance.base);
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

// Load any resources used by the scene.
static void OmegaSceneLoad(void)
{
	ScoreSystemClear();
}

// Initialize the variables used by the scene.
static void OmegaSceneInit()
{
	// Set background color
	DGL_Graphics_SetBackgroundColor(&((const DGL_Color) { 0.0f, 0.0f, 0.0f, 0.0f }));

	// Set blend mode
	DGL_Graphics_SetBlendMode(DGL_BM_BLEND);

	// Create arena
	Entity* entity = EntityFactoryBuild("Arena");
	if (entity)
		SceneAddEntity(entity);

	// Create spaceship
	entity = EntityFactoryBuild("SpaceshipOmega");
	if (entity)
		SceneAddEntity(entity);

	// Create score UI
	entity = EntityFactoryBuild("OmegaScore");
	if (entity)
		SceneAddEntity(entity);

	// Create high score UI 
	entity = EntityFactoryBuild("OmegaHighScore");
	if (entity)
		SceneAddEntity(entity);

	// Create wave UI
	entity = EntityFactoryBuild("OmegaWave");
	if (entity)
		SceneAddEntity(entity);

	ScoreSystemReset();
	instance.OmegapawnCount = cOmegapawnInitial;
	OmegaSceneSpawnAsteroidWave();
}

// Update the the variables used by the scene and render objects (temporary).
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
static void OmegaSceneUpdate(float dt)
{
	// Tell the compiler that the 'dt' variable is unused.
	UNREFERENCED_PARAMETER(dt);

	if (!SceneFindEntityByName("Asteroid"))
		OmegaSceneSpawnAsteroidWave();
}

// Render the scene.
void OmegaSceneRender(void)
{
}

// Exit the scene.
static void OmegaSceneExit()
{
}

// Unload any resources used by the scene.
static void OmegaSceneUnload(void)
{
}

static void OmegaSceneSpawnAsteroidWave(void)
{
	ScoreSystemIncreaseWave();

	for (unsigned int i = 0; i < instance.OmegapawnCount; ++i)
	{
		OmegaSceneSpawnAsteroid();
	}

	if (instance.OmegapawnCount < cOmegapawnMaximum)
		++instance.OmegapawnCount;
}

static void OmegaSceneSpawnAsteroid(void)
{
	Entity* asteroid = EntityFactoryBuild("Asteroid");
	if (asteroid)
		SceneAddEntity(asteroid);
}