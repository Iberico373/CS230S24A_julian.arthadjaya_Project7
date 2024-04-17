//------------------------------------------------------------------------------
//
// File Name:	BehaviorAsteroid.c
// Author(s):	Julian Artha Djaya (julian.arthadjaya)
// Project:		Project 0
// Course:		CS230S24
//
// Copyright © 2024 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "BehaviorAsteroid.h"
#include "Behavior.h"
#include "Random.h"
#include "Collider.h"
#include "Entity.h"
#include "Teleporter.h"
#include "ScoreSystem.h"
#include "Transform.h"
#include "Physics.h"
#include "Vector2D.h"
#include "DGL.h"

typedef enum
{
	cAsteroidOriginTlc,
	cAsteroidOriginTrc,
	cAsteroidOriginBlc,
	cAsteroidOriginBrc,
	cAsteroidOriginCount,

} AsteroidOrigin;

typedef enum AsteroidStates
{
	cAsteroidInvalid = -1,
	cAsteroidIdle = 0
}AsteroidStates;

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

static const float asteroidSpeedMin = 50.0f;
static const float asteroidSpeedMax = 100.0f;

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

typedef struct BehaviorAsteroid
{
	// Inherit the base behavior structure.
	Behavior	base;

	// Add asteroid-specific behavior variables.
	AsteroidOrigin	origin;

} BehaviorAsteroid;

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

static void BehaviorAsteroidInit(Behavior* behavior);
static void BehaviorAsteroidUpdate(Behavior* behavior, float dt);
static void BehaviorAsteroidExit(Behavior* behavior);
static void BehaviorAsteroidSetPosition(BehaviorAsteroid* asteroid);
static void BehaviorAsteroidSetVelocity(BehaviorAsteroid* asteroid);
static void BehaviorAsteroidCollisionHandler(Entity* entity1, const Entity* entity2);

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Dynamically allocate a new (Asteroid) behavior component.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
Behavior* BehaviorAsteroidCreate(void)
{
	BehaviorAsteroid* behavior = (BehaviorAsteroid*)calloc(1, sizeof(BehaviorAsteroid));

	if (behavior)
	{
		behavior->base.memorySize = sizeof(BehaviorAsteroid);
		behavior->base.stateCurr = cAsteroidInvalid;
		behavior->base.stateNext = cAsteroidInvalid;
		behavior->base.onInit = &BehaviorAsteroidInit;
		behavior->base.onUpdate = &BehaviorAsteroidUpdate;
		behavior->base.onExit = &BehaviorAsteroidExit;

		return (Behavior*)behavior;
	}

	return NULL;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

static void BehaviorAsteroidInit(Behavior* behavior)
{
	BehaviorAsteroid* asteroid = (BehaviorAsteroid*)behavior;

	if (asteroid->base.stateCurr == cAsteroidIdle)
	{
		asteroid->origin = (AsteroidOrigin)RandomRange(0, 3);
		BehaviorAsteroidSetPosition(asteroid);
		BehaviorAsteroidSetVelocity(asteroid);

		Collider* collider = EntityGetCollider(behavior->parent);
		if (collider)
			ColliderSetCollisionHandler(collider, (CollisionEventHandler) & BehaviorAsteroidCollisionHandler);
	}
}

static void BehaviorAsteroidUpdate(Behavior* behavior, float dt)
{
	UNREFERENCED_PARAMETER(dt);

	TeleporterUpdateEntity(behavior->parent);
}

static void BehaviorAsteroidExit(Behavior* behavior)
{
	UNREFERENCED_PARAMETER(behavior);
}

static void BehaviorAsteroidSetPosition(BehaviorAsteroid* asteroid)
{
	Vector2D windowSize = DGL_Window_GetSize();
	windowSize.x -= 50.0f;
	windowSize.y -= 50.0f;
	Vector2DScale(&windowSize, &windowSize, 0.5f);

	Transform* trans = EntityGetTransform(asteroid->base.parent);
	Vector2D pos = *TransformGetTranslation(trans);

	switch (asteroid->origin)
	{
	case cAsteroidOriginTlc:
		pos.x = -windowSize.x;
		pos.y = windowSize.y;
		break;

	case cAsteroidOriginTrc:
		pos.x = windowSize.x;
		pos.y = windowSize.y;
		break;

	case cAsteroidOriginBlc:
		pos.x = -windowSize.x;
		pos.y = -windowSize.y;
		break;

	case cAsteroidOriginBrc:
		pos.x = windowSize.x;
		pos.y = -windowSize.y;
		break;

	default:
		break;
	}

	TransformSetTranslation(trans, &pos);
}

static void BehaviorAsteroidSetVelocity(BehaviorAsteroid* asteroid)
{
	Physics* phys = EntityGetPhysics(asteroid->base.parent);
	float angle = 0;

	switch (asteroid->origin)
	{
	case cAsteroidOriginTlc:
		angle = RandomRangeFloat(-10, -80);
		break;

	case cAsteroidOriginTrc:
		angle = RandomRangeFloat(-100, -170);
		break;

	case cAsteroidOriginBlc:
		angle = RandomRangeFloat(10, 80);
		break;

	case cAsteroidOriginBrc:
		angle = RandomRangeFloat(100, 170);
		break;

	default:
		break;
	}

	Vector2D velocity = { 0.0f, 0.0f };
	Vector2DFromAngleDeg(&velocity, (float)angle);
	float speed = RandomRangeFloat(asteroidSpeedMin, asteroidSpeedMax);
	Vector2DScale(&velocity, &velocity, speed);
	PhysicsSetVelocity(phys, &velocity);
}

static void BehaviorAsteroidCollisionHandler(Entity* entity1, const Entity* entity2)
{
	if (!entity1 || !entity2)
		return;

	if (EntityIsNamed(entity2, "Bullet") || EntityIsNamed(entity2, "Spaceship"))
	{
		ScoreSystemIncreaseScore(20);
		EntityDestroy(entity1);
	}
}