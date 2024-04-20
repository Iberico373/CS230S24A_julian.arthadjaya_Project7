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
}

// Initialize the variables used by the scene.
static void Level2SceneInit()
{
	instance.entity = EntityFactoryBuild("SpaceshipHoming");

	if (instance.entity)
	{
		//SpriteSetMesh(EntityGetSprite(instance.entity), instance.mesh);
		DGL_Color bgColor = { 0.0f, 0.0f, 0.0f, 0.0f };
		DGL_Graphics_SetBackgroundColor(&bgColor);
		DGL_Graphics_SetBlendMode(DGL_BM_BLEND);
	}
}

static void Level2SceneMovementController(Entity* entity)
{
	Physics* physics = entity->Has(Physics);
	Transform* transform = entity->Has(Transform);

	if (physics && transform)
	{
		Vector2D mousePos = DGL_Input_GetMousePosition();
		Vector2D worldMousePos = DGL_Camera_ScreenCoordToWorld(&mousePos);

		Vector2D direction = { 0.0f, 0.0f };
		Vector2DSub(&direction, &worldMousePos, transform->GetTranslation());
		Vector2DNormalize(&direction, &direction);

		transform->SetRotation(Vector2DToAngleRad(&direction));	
		Vector2DScale(&direction, &direction, spaceshipSpeed);
		physics->SetVelocity(&direction);
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
	instance.entity->Update(dt);

	if (DGL_Input_KeyTriggered(VK_ESCAPE))
	{
		SceneSystemSetNext(NULL);
	}

	if (DGL_Input_KeyTriggered('Z'))
	{
		instance.entity->Has(Sprite)->SetAlpha(0.5f);
	}

	if (DGL_Input_KeyTriggered('X'))
	{
		instance.entity->Has(Sprite)->SetAlpha(1.0f);
	}
}

// Render the scene.
void Level2SceneRender(void)
{
	instance.entity->Render();
}

// Exit the scene.
static void Level2SceneExit()
{
	delete instance.entity;
}

// Unload any resources used by the scene.
static void Level2SceneUnload(void)
{
}

