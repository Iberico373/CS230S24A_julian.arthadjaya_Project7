//------------------------------------------------------------------------------
//
// File Name:	BehaviorBullet.c
// Author(s):	Julian Artha Djaya (julian.arthadjaya)
// Project:		Project 0
// Course:		CS230S24
//
// Copyright © 2024 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "BehaviorBullet.h"
#include "Behavior.h"
#include "Entity.h"
#include "Teleporter.h"
#include "Collider.h"

typedef enum BulletStates
{
	cBulletInvalid = -1,
	cBulletIdle
}BulletStates;

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

static const float bulletSpeedMax = 500.0f;

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

static void BehaviorBulletUpdateLifeTimer(Behavior* behavior, float dt);
static void BehaviorBulletCollisionHandler(Entity* entity1, Entity* entity2);

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Dynamically allocate a new (Bullet) behavior component.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
Behavior* BehaviorBulletCreate(void)
{
	Behavior* behavior = calloc(1, sizeof(Behavior));

	if (!behavior)
		return NULL;

	behavior->memorySize = sizeof(Behavior);
	behavior->stateCurr = cBulletInvalid;
	behavior->stateNext = cBulletInvalid;
	behavior->onInit = &BehaviorBulletInit;
	behavior->onUpdate = &BehaviorBulletUpdate;
	behavior->onExit = &BehaviorBulletExit;

	return behavior;
}

// Initialize the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
void BehaviorBulletInit(Behavior* behavior)
{
	if (behavior->stateCurr == cBulletIdle)
	{
		Collider* collider = EntityGetCollider(behavior->parent);

		if (collider)
			ColliderSetCollisionHandler(collider, &BehaviorBulletCollisionHandler);
	}
}

// Update the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
//	 dt = Change in time (in seconds) since the last game loop.
void BehaviorBulletUpdate(Behavior* behavior, float dt)
{
	switch (behavior->stateCurr)
	{
	case cBulletIdle:
		BehaviorBulletUpdateLifeTimer(behavior, dt);
		break;

	default:
		break;
	}

	TeleporterUpdateEntity(behavior->parent);
}

// Exit the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
//	 dt = Change in time (in seconds) since the last game loop.
void BehaviorBulletExit(Behavior* behavior)
{
	UNREFERENCED_PARAMETER(behavior);
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

static void BehaviorBulletUpdateLifeTimer(Behavior* behavior, float dt)
{
	if (behavior->timer > 0)
		behavior->timer -= dt;

	if (behavior->timer <= 0)
		EntityDestroy(behavior->parent);
}

static void BehaviorBulletCollisionHandler(Entity* entity1, Entity* entity2)
{
	if (!entity1 || !entity2)
		return;

	if (EntityIsNamed(entity2, "Asteroid"))
		EntityDestroy(entity1);
}