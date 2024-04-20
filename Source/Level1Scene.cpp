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
#include "Mesh.h"
#include "Sprite.h"
#include "SpriteSource.h"
#include "Entity.h"
#include "EntityFactory.h"
#include "Physics.h"
#include "Transform.h"
#include "Animation.h"

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
static bool Level1SceneIsColliding(Entity* entityA, Entity* entityB);
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
		Sprite* planetSprite = instance.planetEntity->Has(Sprite);
		planetSprite->SetMesh(instance.mesh1x1);
		planetSprite->SetSpriteSource(instance.planetSpriteSource);
		planetSprite->SetFrame(0);
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
		Sprite* livesTextSprite = instance.livesTextEntity->Has(Sprite);

		livesTextSprite->SetMesh(instance.mesh16x8);
		livesTextSprite->SetSpriteSource(instance.livesTextSpriteSource);
		sprintf_s(instance.livesBuffer, _countof(instance.livesBuffer), "Lives: %d", instance.numLives);
		livesTextSprite->SetText(instance.livesBuffer);
	}

	DGL_Color bgColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	DGL_Graphics_SetBackgroundColor(&bgColor);
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


	instance.monkeyEntity->Update(dt);
	instance.planetEntity->Update(dt);
	instance.livesTextEntity->Update(dt);

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
	instance.planetEntity->Render();
	instance.monkeyEntity->Render();
	instance.livesTextEntity->Render();
}

// Exit the scene.
static void Level1SceneExit()
{
	delete instance.planetEntity;

	delete instance.monkeyEntity;

	delete instance.livesTextEntity;
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
	Physics* physics = entity->Has(Physics);
	Transform* transform = entity->Has(Transform);

	if (physics && transform)
	{
		// Get current velocity
		Vector2D currentVelocity = *(physics->GetVelocity());

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
			physics->SetAcceleration(&gravityNormal);
		}

		// Store current translation
		Vector2D currentTranslation = *transform->GetTranslation();

		// Check if planet is grounded
		if (currentTranslation.y < groundHeight)
		{
			Vector2D ground = { currentTranslation.x, groundHeight };
			transform->SetTranslation(&ground);

			currentVelocity.y = 0;
			physics->SetAcceleration(&gravityNone);

			Level1SceneSetMonkeyState(entity, MonkeyIdle);
		}

		// Set new velocity
		physics->SetVelocity(&currentVelocity);
	}
}

static void Level1SceneBounceController(Entity* entity)
{
	Physics* physics = entity->Has(Physics);
	Transform* transform = entity->Has(Transform);

	if (physics && transform)
	{
		Vector2D currentPos = *(transform->GetTranslation());
		Vector2D currentVelocity = *(physics->GetVelocity());

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

		transform->SetTranslation(&currentPos);
		physics->SetVelocity(&currentVelocity);
	}
}

static bool Level1SceneIsColliding(Entity* entityA, Entity* entityB)
{
	Vector2D posA = *(entityA->Has(Transform)->GetTranslation());
	Vector2D posB = *(entityB->Has(Transform)->GetTranslation());

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

		Sprite* sprite = entity->Has(Sprite);
		Animation* animation = entity->Has(Animation);

		switch (newState)
		{						
		case MonkeyIdle:
			sprite->SetMesh(instance.mesh1x1);
			sprite->SetSpriteSource(instance.monkeyIdleSpriteSource);
			animation->Play(1, 0.0f, false);
			break;

		case MonkeyWalk:
			sprite->SetMesh(instance.mesh3x3);
			sprite->SetSpriteSource(instance.monkeyWalkSpriteSource);
			animation->Play(8, 0.05f, true);
			break;

		case MonkeyJump:
			sprite->SetMesh(instance.mesh1x1);
			sprite->SetSpriteSource(instance.monkeyJumpSpriteSource);
			animation->Play(1, 0.0f, false);
			break;

		default:
			break;
		}
	}
}
