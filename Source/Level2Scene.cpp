//------------------------------------------------------------------------------
//
// File Name:	Level2Scene.c
// Author(s):	Doug Schilling (dschilling)
// Project:		Project 0
// Course:		CS230S24
//
// Copyright © 2024 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Scene.h"
#include "SceneSystem.h"
#include "Level1Scene.h"
#include "Level2Scene.h"
#include "SandboxScene.h"
#include "DemoScene.h"
#include "DGL.h"
#include "Stream.h"
#include "Mesh.h"
#include "Entity.h"
#include "EntityFactory.h"
#include "Sprite.h"
#include "Physics.h"
#include "Transform.h"
#include "Vector2D.h"

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

typedef struct Level2Scene
{
	// WARNING: The base class must always be included first.
	Scene	base;

	// Add any scene-specific variables second.
	Mesh* mesh;

	Entity* entity;

} Level2Scene;

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

static const float spaceshipSpeed = 500.0f;

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

static void Level2SceneLoad(void);
static void Level2SceneInit(void);
static void Level2SceneUpdate(float dt);
static void Level2SceneExit(void);
static void Level2SceneUnload(void);
static void Level2SceneRender(void);

//------------------------------------------------------------------------------
// Instance Variable:
//------------------------------------------------------------------------------

static Level2Scene instance =
{
	// Initialize the base structure:
	{ "Level2", Level2SceneLoad, Level2SceneInit, Level2SceneUpdate, Level2SceneRender, Level2SceneExit, Level2SceneUnload },

	// Initialize any scene-specific variables:
};

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

const Scene* Level2SceneGetInstance(void)
{
	return &(instance.base);
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

// Load any resources used by the scene.
static void Level2SceneLoad(void)
{
	instance.mesh = MeshCreate();
	MeshBuildSpaceship(instance.mesh);
}

// Initialize the variables used by the scene.
static void Level2SceneInit()
{
	instance.entity = EntityFactoryBuild("SpaceshipHoming");

	if (instance.entity)
	{
		SpriteSetMesh(EntityGetSprite(instance.entity), instance.mesh);
		DGL_Graphics_SetBackgroundColor(&(DGL_Color) { 0, 0, 0 });
		DGL_Graphics_SetBlendMode(DGL_BM_BLEND);
	}
}

static void Level2SceneMovementController(Entity* entity)
{
	Physics* physics = EntityGetPhysics(entity);
	Transform* transform = EntityGetTransform(entity);

	if (physics && transform)
	{
		Vector2D mousePos = DGL_Input_GetMousePosition();
		Vector2D worldMousePos = DGL_Camera_ScreenCoordToWorld(&mousePos);

		Vector2D* direction = &(Vector2D) { 0, 0 };
		Vector2DSub(direction, &worldMousePos, TransformGetTranslation(transform));
		Vector2DNormalize(direction, direction);

		TransformSetRotation(transform, Vector2DToAngleRad(direction));	
		Vector2DScale(direction, direction, spaceshipSpeed);
		PhysicsSetVelocity(physics, direction);
	}
}

// Update the the variables used by the scene and render objects (temporary).
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
static void Level2SceneUpdate(float dt)
{
	// Tell the compiler that the 'dt' variable is unused.
	UNREFERENCED_PARAMETER(dt);

	Level2SceneMovementController(instance.entity);
	EntityUpdate(instance.entity, dt);

	if (DGL_Input_KeyTriggered(VK_ESCAPE))
	{
		SceneSystemSetNext(NULL);
	}

	if (DGL_Input_KeyTriggered('Z'))
	{
		SpriteSetAlpha(EntityGetSprite(instance.entity), 0.5f);
	}

	if (DGL_Input_KeyTriggered('X'))
	{
		SpriteSetAlpha(EntityGetSprite(instance.entity), 1.0f);
	}
}

// Render the scene.
void Level2SceneRender(void)
{
	EntityRender(instance.entity);
}

// Exit the scene.
static void Level2SceneExit()
{
	EntityFree(&instance.entity);
}

// Unload any resources used by the scene.
static void Level2SceneUnload(void)
{
	MeshFree(&instance.mesh);
}

