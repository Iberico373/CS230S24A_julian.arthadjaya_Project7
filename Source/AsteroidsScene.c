//------------------------------------------------------------------------------
//
// File Name:	AsteroidsScene.c
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
#include "AsteroidsScene.h"
#include "Entity.h"
#include "EntityFactory.h"

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

typedef struct AsteroidsScene
{
	// WARNING: The base class must always be included first.
	Scene	base;

	// Add any scene-specific variables second.
	unsigned asteroidSpawnCount;

} AsteroidsScene;

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

static const unsigned cAsteroidSpawnInitial = 8;
static const unsigned cAsteroidSpawnMaximum = 20;

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

static void AsteroidsSceneLoad(void);
static void AsteroidsSceneInit(void);
static void AsteroidsSceneUpdate(float dt);
static void AsteroidsSceneExit(void);
static void AsteroidsSceneUnload(void);
static void AsteroidsSceneRender(void);
static void AsteroidsSceneSpawnAsteroidWave(void);
static void AsteroidsSceneSpawnAsteroid(void);


//------------------------------------------------------------------------------
// Instance Variable:
//------------------------------------------------------------------------------

static AsteroidsScene instance =
{
	// Initialize the base structure:
	{ "AsteroidsScene", AsteroidsSceneLoad, AsteroidsSceneInit, AsteroidsSceneUpdate, AsteroidsSceneRender, AsteroidsSceneExit, AsteroidsSceneUnload },

	// Initialize any scene-specific variables:
};

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

const Scene* AsteroidsSceneGetInstance(void)
{
	return &(instance.base);
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

// Load any resources used by the scene.
static void AsteroidsSceneLoad(void)
{
	ScoreSystemClear();
}

// Initialize the variables used by the scene.
static void AsteroidsSceneInit()
{
	// Set background color
	DGL_Graphics_SetBackgroundColor(&((const DGL_Color) { 0.0f, 0.0f, 0.0f, 0.0f }));

	// Set blend mode
	DGL_Graphics_SetBlendMode(DGL_BM_BLEND);

	// Create spaceship
	Entity* entity = EntityFactoryBuild("Spaceship");
	if (entity)
		SceneAddEntity(entity);

	// Create score UI
	entity = EntityFactoryBuild("AsteroidsScore");
	if (entity)
		SceneAddEntity(entity);

	// Create high score UI 
	entity = EntityFactoryBuild("AsteroidsHighScore");
	if (entity)
		SceneAddEntity(entity);

	// Create wave UI
	entity = EntityFactoryBuild("AsteroidsWave");
	if (entity)
		SceneAddEntity(entity);

	ScoreSystemReset();
	instance.asteroidSpawnCount = cAsteroidSpawnInitial;
	AsteroidsSceneSpawnAsteroidWave();
}

// Update the the variables used by the scene and render objects (temporary).
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
static void AsteroidsSceneUpdate(float dt)
{
	// Tell the compiler that the 'dt' variable is unused.
	UNREFERENCED_PARAMETER(dt);

	if (!SceneFindEntityByName("Asteroid"))
		AsteroidsSceneSpawnAsteroidWave();
}

// Render the scene.
void AsteroidsSceneRender(void)
{
}

// Exit the scene.
static void AsteroidsSceneExit()
{
}

// Unload any resources used by the scene.
static void AsteroidsSceneUnload(void)
{
}

static void AsteroidsSceneSpawnAsteroidWave(void)
{
	ScoreSystemIncreaseWave();

	for (unsigned int i = 0; i < instance.asteroidSpawnCount; ++i)
	{
		AsteroidsSceneSpawnAsteroid();
	}

	if (instance.asteroidSpawnCount < cAsteroidSpawnMaximum)
		++instance.asteroidSpawnCount;
}

static void AsteroidsSceneSpawnAsteroid(void)
{
	Entity* asteroid = EntityFactoryBuild("Asteroid");
	if (asteroid)
		SceneAddEntity(asteroid);
}