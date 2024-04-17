//------------------------------------------------------------------------------
//
// File Name:	BehaviorSpaceship.c
// Author(s):	Julian Artha Djaya (julian.arthadjaya)
// Project:		Project 0
// Course:		CS230S24
//
// Copyright © 2024 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "BehaviorSpaceship.h"
#include "Behavior.h"
#include "Entity.h"
#include "EntityFactory.h"
#include "EntityContainer.h"
#include "Physics.h"
#include "Transform.h"
#include "Vector2D.h"
#include "Scene.h"
#include "DGL.h"
#include "Teleporter.h"
#include "Collider.h"
#include "Sprite.h"

#define _USE_MATH_DEFINES
#include <math.h>

typedef enum SpaceshipStates
{
	cSpaceshipInvalid = -1,
	cSpaceshipIdle,
	cSpaceshipThrust,
	cSpaceshipReverse,
	cSpaceshipDead

} SpaceshipStates;

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

static const float spaceshipAcceleration = 150.0f;
static const float spaceshipSpeedMax = 500.0f;
static const float spaceshipTurnRateMax = (float)M_PI / 1.5f;
static const float spaceshipWeaponCooldownTime = 0.25f;
static const float spaceshipWeaponBulletSpeed = 750.0f;
static const float spaceshipDeathDuration = 3.0f;


//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

static void BehaviorSpaceshipUpdateRotation(Behavior* behavior, float dt);
static void BehaviorSpaceshipUpdateVelocity(Behavior* behavior, float dt);
static void BehaviorSpaceshipUpdateWeapon(Behavior* behavior, float dt);
static void BehaviorSpaceshipSpawnBullet(Behavior* behavior);
static void BehaviorSpaceshipCollisionHandler(Entity* entity1, Entity* entity2);


//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Dynamically allocate a new (Spaceship) behavior component.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
Behavior* BehaviorSpaceshipCreate(void)
{
	Behavior* behavior = (Behavior*)calloc(1, sizeof(Behavior));

	if (!behavior)
		return NULL;

	behavior->memorySize = sizeof(Behavior);
	behavior->stateCurr = cSpaceshipInvalid;
	behavior->stateNext = cSpaceshipInvalid;
	behavior->onInit = &BehaviorSpaceshipInit;
	behavior->onUpdate = &BehaviorSpaceshipUpdate;
	behavior->onExit = &BehaviorSpaceshipExit;

	return behavior;
}

// Initialize the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
void BehaviorSpaceshipInit(Behavior* behavior)
{
	if (behavior->stateCurr == cSpaceshipIdle)
	{
		EntitySetName(behavior->parent, "Spaceship");
		Collider* collider = EntityGetCollider(behavior->parent);		
		if (collider)
			ColliderSetCollisionHandler(collider, &BehaviorSpaceshipCollisionHandler);
	}

	if (behavior->stateCurr == cSpaceshipDead)
	{
		behavior->timer = spaceshipDeathDuration;
		Vector2D zero = { 0.0f, 0.0f };
		PhysicsSetVelocity(EntityGetPhysics(behavior->parent), &zero);
		EntitySetName(behavior->parent, "DeadShip");
	}
		
}

// Update the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
//	 dt = Change in time (in seconds) since the last game loop.
void BehaviorSpaceshipUpdate(Behavior* behavior, float dt)
{
	switch (behavior->stateCurr)
	{
	case cSpaceshipIdle:
		BehaviorSpaceshipUpdateRotation(behavior, dt);
		BehaviorSpaceshipUpdateWeapon(behavior, dt);

		if (DGL_Input_KeyDown(VK_UP))
			behavior->stateNext = cSpaceshipThrust;

		if (DGL_Input_KeyDown(VK_DOWN))
			behavior->stateNext = cSpaceshipReverse;

		break;

	case cSpaceshipThrust:
		BehaviorSpaceshipUpdateRotation(behavior, dt);
		BehaviorSpaceshipUpdateVelocity(behavior, dt);
		BehaviorSpaceshipUpdateWeapon(behavior, dt);

		if (!DGL_Input_KeyDown(VK_UP))
			behavior->stateNext = cSpaceshipIdle;

		break;

	case cSpaceshipReverse:
		BehaviorSpaceshipUpdateRotation(behavior, dt);
		BehaviorSpaceshipUpdateVelocity(behavior, dt);
		BehaviorSpaceshipUpdateWeapon(behavior, dt);

		if (!DGL_Input_KeyDown(VK_DOWN))
			behavior->stateNext = cSpaceshipIdle;
		break;

	case cSpaceshipDead:
		behavior->timer -= dt;

		// Fade effect
		float alpha = SpriteGetAlpha(EntityGetSprite(behavior->parent)) - dt;
		SpriteSetAlpha(EntityGetSprite(behavior->parent), alpha);

		// Spinning effect
		PhysicsSetRotationalVelocity(EntityGetPhysics(behavior->parent), spaceshipTurnRateMax * 6.0f);

		if (behavior->timer < 0)
			SceneRestart();
		
		break;
	}

	TeleporterUpdateEntity(behavior->parent);
}

// Exit the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
//	 dt = Change in time (in seconds) since the last game loop.
void BehaviorSpaceshipExit(Behavior* behavior)
{
	if (behavior->stateCurr == cSpaceshipInvalid)
	{
		PhysicsSetDrag(EntityGetPhysics(behavior->parent), 0.99f);
	}
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

static void BehaviorSpaceshipUpdateRotation(Behavior* behavior, float dt)
{
	UNREFERENCED_PARAMETER(dt);

	if (DGL_Input_KeyDown(VK_LEFT))
		PhysicsSetRotationalVelocity(EntityGetPhysics(behavior->parent), spaceshipTurnRateMax);

	else if (DGL_Input_KeyDown(VK_RIGHT))
		PhysicsSetRotationalVelocity(EntityGetPhysics(behavior->parent), -spaceshipTurnRateMax);

	else
		PhysicsSetRotationalVelocity(EntityGetPhysics(behavior->parent), 0);
}

static void BehaviorSpaceshipUpdateVelocity(Behavior* behavior, float dt)
{
	Transform* transform = EntityGetTransform(behavior->parent);
	Physics* physics = EntityGetPhysics(behavior->parent);

	if (transform && physics)
	{
		Vector2D rotation = { 0.0f, 0.0f };
		Vector2DFromAngleRad(&rotation, TransformGetRotation(transform));

		Vector2D velocity = *PhysicsGetVelocity(physics);
		float acceleration = spaceshipAcceleration;

		if (behavior->stateCurr == cSpaceshipReverse)
			acceleration = -acceleration;

		Vector2DScaleAdd(&velocity, &rotation, &velocity, acceleration * dt);
		float speed = Vector2DLength(&velocity);

		if (speed > spaceshipSpeedMax)
			Vector2DScale(&velocity, &velocity, spaceshipSpeedMax / speed);

		PhysicsSetVelocity(physics, &velocity);
	}
}

static void BehaviorSpaceshipUpdateWeapon(Behavior* behavior, float dt)
{
	if (behavior->timer > 0)
		behavior->timer -= dt;

	if (behavior->timer < 0)
		behavior->timer = 0;

	if (DGL_Input_KeyDown(' '))
	{
		if (behavior->timer <= 0)
		{
			BehaviorSpaceshipSpawnBullet(behavior);
			behavior->timer = spaceshipWeaponCooldownTime;
		}
	}
}

static void BehaviorSpaceshipSpawnBullet(Behavior* behavior)
{
	Entity* bullet = EntityFactoryBuild("Bullet");

	if (bullet)
	{
		Vector2D position = *TransformGetTranslation(EntityGetTransform(behavior->parent));
		float rotation = TransformGetRotation(EntityGetTransform(behavior->parent));
		
		TransformSetTranslation(EntityGetTransform(bullet), &position);
		TransformSetRotation(EntityGetTransform(bullet), rotation);

		Vector2D direction = { 0.0f, 0.0f };
		Vector2DFromAngleRad(&direction, rotation);
		Vector2DScale(&direction, &direction, spaceshipWeaponBulletSpeed);
		PhysicsSetVelocity(EntityGetPhysics(bullet), &direction);

		SceneAddEntity(bullet);
	}
}

static void BehaviorSpaceshipCollisionHandler(Entity* entity1, Entity* entity2)
{
	if (!entity1 || !entity2)
		return;

	if (EntityIsNamed(entity2, "Asteroid"))
		EntityGetBehavior(entity1)->stateNext = cSpaceshipDead;
}