//------------------------------------------------------------------------------
//
// File Name:	Level1Scene.c
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
#include "Stream.h"
#include "Mesh.h"
#include "Sprite.h"
#include "SpriteSource.h"
#include "Entity.h"
#include "EntityFactory.h"
#include "Physics.h"
#include "Transform.h"
#include "Animation.h"
#include "Vector2D.h"

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

typedef struct Level1Scene
{
	// WARNING: The base class must always be included first.
	Scene	base;

	// Add any scene-specific variables second.
	int numLives;

	enum MonkeyStates monkeyState;

	char livesBuffer[16];

	// meshes
	Mesh* mesh1x1;
	Mesh* mesh3x3;
	Mesh* mesh16x8;

	// planet
	Entity* planetEntity;

	SpriteSource* planetSpriteSource;

	// monkey
	Entity* monkeyEntity;

	SpriteSource* monkeyIdleSpriteSource;
	SpriteSource* monkeyJumpSpriteSource;
	SpriteSource* monkeyWalkSpriteSource;

	// lives text
	Entity* livesTextEntity;

	SpriteSource* livesTextSpriteSource;

} Level1Scene;

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

typedef enum MonkeyStates
{
	MonkeyInvalid = -1,
	MonkeyIdle,
	MonkeyWalk,
	MonkeyJump
}MonkeyStates;

static const float wallDistance = 462.0f;
static const float CheckSquareDistance = (75.0f * 75.0f);
static const float groundHeight = -150.0f;
static const float moveVelocity = 500.0f;
static const float jumpVelocity = 1000.0f;
static const Vector2D gravityNormal = { 0.0f, -1500.0f };
static const Vector2D gravityNone = { 0.0f, 0.0f };


//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

static void Level1SceneLoad(void);
static void Level1SceneInit(void);
static void Level1SceneUpdate(float dt);
static void Level1SceneExit(void);
static void Level1SceneUnload(void);
static void Level1SceneRender(void);
static void Level1SceneMovementController(Entity* entity);
static void Level1SceneBounceController(Entity* entity);
static bool Level1SceneIsColliding(const Entity* entityA, const Entity* entityB);
static void Level1SceneSetMonkeyState(Entity* entity, MonkeyStates newState);

//------------------------------------------------------------------------------
// Instance Variable:
//------------------------------------------------------------------------------

static Level1Scene instance =
{
	// Initialize the base structure:
	{ "Level1", Level1SceneLoad, Level1SceneInit, Level1SceneUpdate, Level1SceneRender, Level1SceneExit, Level1SceneUnload},

	// Initialize any scene-specific variables:
	0,						// numLives
	{ MonkeyInvalid },		// monkeyState
};

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

const Scene* Level1SceneGetInstance(void)
{
	return &(instance.base);
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

// Load any resources used by the scene.
static void Level1SceneLoad(void)
{
	Stream fileStream = StreamOpen("./Data/Level1_Lives.txt");  // Open file stream
	instance.numLives = StreamReadInt(fileStream);				// Set number of lives based on txt file
	StreamClose(&fileStream);									// Close file stream

	instance.mesh1x1 = MeshCreate();
	MeshBuildQuad(instance.mesh1x1, 0.5f, 0.5f, 1.0f, 1.0f, "Mesh1x1");

	instance.mesh3x3 = MeshCreate();
	MeshBuildQuad(instance.mesh3x3, 0.5f, 0.5f, 1.0f / 3.0f, 1.0f / 3.0f, "Mesh3x3");

	instance.mesh16x8 = MeshCreate();
	MeshBuildQuad(instance.mesh16x8, 0.5f, 0.5f, 1.0f / 16.0f, 1.0f / 8.0f, "Mesh16x8");

	instance.monkeyIdleSpriteSource =  SpriteSourceCreate();
	SpriteSourceLoadTexture(instance.monkeyIdleSpriteSource, 1, 1, "MonkeyIdle.png");

	instance.monkeyJumpSpriteSource = SpriteSourceCreate();
	SpriteSourceLoadTexture(instance.monkeyJumpSpriteSource, 1, 1, "MonkeyJump.png");

	instance.monkeyWalkSpriteSource = SpriteSourceCreate();
	SpriteSourceLoadTexture(instance.monkeyWalkSpriteSource, 3, 3, "MonkeyWalk.png");

	instance.livesTextSpriteSource = SpriteSourceCreate();
	SpriteSourceLoadTexture(instance.livesTextSpriteSource, 16, 8, "Roboto_Mono_black.png");

	instance.planetSpriteSource = SpriteSourceCreate();
	SpriteSourceLoadTexture(instance.planetSpriteSource, 1, 1, "PlanetTexture.png");
}

// Initialize the variables used by the scene.
static void Level1SceneInit()
{
	instance.planetEntity = EntityFactoryBuild("PlanetBounce");

	if (instance.planetEntity)
	{
		SpriteSetMesh(EntityGetSprite(instance.planetEntity), instance.mesh1x1);
		SpriteSetSpriteSource(EntityGetSprite(instance.planetEntity), instance.planetSpriteSource);
		SpriteSetFrame(EntityGetSprite(instance.planetEntity), 0);
	}

	instance.monkeyEntity = EntityFactoryBuild("Monkey");

	if (instance.monkeyEntity)
	{
		instance.monkeyState = MonkeyInvalid;
		Level1SceneSetMonkeyState(instance.monkeyEntity, MonkeyIdle);
	}

	instance.livesTextEntity = EntityFactoryBuild("MonkeyLivesText");

	if (instance.livesTextEntity)
	{
		Sprite* livesTextSprite = EntityGetSprite(instance.livesTextEntity);

		SpriteSetMesh(livesTextSprite, instance.mesh16x8);
		SpriteSetSpriteSource(livesTextSprite, instance.livesTextSpriteSource);
		sprintf_s(instance.livesBuffer, _countof(instance.livesBuffer), "Lives: %d", instance.numLives);
		SpriteSetText(livesTextSprite, instance.livesBuffer);
	}

	DGL_Graphics_SetBackgroundColor(&(DGL_Color) { 1, 1, 1 });
	DGL_Graphics_SetBlendMode(DGL_BM_BLEND);
}

// Update the the variables used by the scene and render objects (temporary).
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
static void Level1SceneUpdate(float dt)
{
	// Tell the compiler that the 'dt' variable is unused.
	UNREFERENCED_PARAMETER(dt);

	Level1SceneMovementController(instance.monkeyEntity);
	Level1SceneBounceController(instance.planetEntity);

	EntityUpdate(instance.monkeyEntity, dt);
	EntityUpdate(instance.planetEntity, dt);
	EntityUpdate(instance.livesTextEntity, dt);

	if (Level1SceneIsColliding(instance.monkeyEntity, instance.planetEntity))
	{
		--instance.numLives;

		if (instance.numLives <= 0)
		{
			SceneSystemSetNext(Level2SceneGetInstance());
		}

		else
			SceneRestart();
	}	
}

// Render the scene.
void Level1SceneRender(void)
{
	EntityRender(instance.planetEntity);
	EntityRender(instance.monkeyEntity);
	EntityRender(instance.livesTextEntity);
}

// Exit the scene.
static void Level1SceneExit()
{
	EntityFree(&instance.planetEntity);
	EntityFree(&instance.monkeyEntity);
	EntityFree(&instance.livesTextEntity);
}

// Unload any resources used by the scene.
static void Level1SceneUnload(void)
{
	SpriteSourceFree(&instance.planetSpriteSource);
	SpriteSourceFree(&instance.monkeyIdleSpriteSource);
	SpriteSourceFree(&instance.monkeyJumpSpriteSource);
	SpriteSourceFree(&instance.monkeyWalkSpriteSource);
	SpriteSourceFree(&instance.livesTextSpriteSource);

	MeshFree(&instance.mesh1x1);
	MeshFree(&instance.mesh3x3);
	MeshFree(&instance.mesh16x8);
}

static void Level1SceneMovementController(Entity* entity)
{
	Physics* physics = EntityGetPhysics(entity);
	Transform* transform = EntityGetTransform(entity);

	if (physics && transform)
	{
		// Get current velocity
		Vector2D currentVelocity = *PhysicsGetVelocity(physics);

		// Check for horizontal inputs 
		if (DGL_Input_KeyDown(VK_LEFT))
		{
			currentVelocity.x = -moveVelocity;

			if (instance.monkeyState != MonkeyJump)
				Level1SceneSetMonkeyState(entity, MonkeyWalk);
		}

		else if (DGL_Input_KeyDown(VK_RIGHT))
		{
			currentVelocity.x = moveVelocity;

			if (instance.monkeyState != MonkeyJump)
				Level1SceneSetMonkeyState(entity, MonkeyWalk);
		}

		else
		{
			currentVelocity.x = 0;
			if (instance.monkeyState != MonkeyJump)
				Level1SceneSetMonkeyState(entity, MonkeyIdle);
		}

		// Check for jumps
		if (DGL_Input_KeyTriggered(VK_UP))
		{
			Level1SceneSetMonkeyState(entity, MonkeyJump);
			currentVelocity.y = jumpVelocity;
			PhysicsSetAcceleration(physics, &gravityNormal);			
		}

		// Store current translation
		Vector2D currentTranslation = *TransformGetTranslation(transform);

		// Check if planet is grounded
		if (currentTranslation.y < groundHeight)
		{
			TransformSetTranslation(transform, &(Vector2D){currentTranslation.x, groundHeight});

			currentVelocity.y = 0;
			PhysicsSetAcceleration(physics, &gravityNone);

			Level1SceneSetMonkeyState(entity, MonkeyIdle);
		}

		// Set new velocity
		PhysicsSetVelocity(physics, &currentVelocity);
	}
}

static void Level1SceneBounceController(Entity* entity)
{
	Physics* physics = EntityGetPhysics(entity);
	Transform* transform = EntityGetTransform(entity);

	if (physics && transform)
	{
		Vector2D currentPos = *TransformGetTranslation(transform);
		Vector2D currentVelocity = *PhysicsGetVelocity(physics);

		if (currentPos.x <= -wallDistance)
		{
			currentPos.x = -wallDistance;
			currentVelocity.x = -currentVelocity.x;
		}

		if (currentPos.x >= wallDistance)
		{
			currentPos.x = wallDistance;
			currentVelocity.x = -currentVelocity.x;
		}

		if (currentPos.y <= groundHeight)
		{
			currentPos.y = groundHeight + (groundHeight - currentPos.y);
			currentVelocity.y = -currentVelocity.y;
		}

		TransformSetTranslation(transform, &currentPos);
		PhysicsSetVelocity(physics, &currentVelocity);
	}
}

static bool Level1SceneIsColliding(const Entity* entityA, const Entity* entityB)
{
	Vector2D posA = *TransformGetTranslation(EntityGetTransform(entityA));
	Vector2D posB = *TransformGetTranslation(EntityGetTransform(entityB));
	float distance = Vector2DSquareDistance(&posA, &posB);

	if (distance < CheckSquareDistance)
		return true;

	else
		return false;
}

static void Level1SceneSetMonkeyState(Entity* entity, MonkeyStates newState)
{
	if (instance.monkeyState != newState)
	{
		instance.monkeyState = newState;

		Sprite* sprite = EntityGetSprite(entity);
		Animation* animation = EntityGetAnimation(entity);

		switch (newState)
		{						
		case MonkeyIdle:
			SpriteSetMesh(sprite, instance.mesh1x1);
			SpriteSetSpriteSource(sprite, instance.monkeyIdleSpriteSource);
			AnimationPlay(animation, 1, 0.0f, false);
			break;

		case MonkeyWalk:
			SpriteSetMesh(sprite, instance.mesh3x3);
			SpriteSetSpriteSource(sprite, instance.monkeyWalkSpriteSource);
			AnimationPlay(animation, 8, 0.05f, true);
			break;

		case MonkeyJump:
			SpriteSetMesh(sprite, instance.mesh1x1);
			SpriteSetSpriteSource(sprite, instance.monkeyJumpSpriteSource);
			AnimationPlay(animation, 1, 0.0f, false);
			break;

		default:
			break;
		}
	}
}
