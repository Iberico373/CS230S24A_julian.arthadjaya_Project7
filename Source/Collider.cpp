//------------------------------------------------------------------------------
//
// File Name:	Collider.c
// Author(s):	Julian Artha Djaya (julian.arthadjaya)
// Project:		Project 0
// Course:		CS230S24
//
// Copyright © 2024 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Collider.h"
#include "ColliderCircle.h"
#include "ColliderLine.h"
#include "Entity.h"
#include "Transform.h"
#include "Vector2D.h"

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

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

static bool ColliderIsColliding(const Collider* collider1, const Collider* collider2);

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Dynamically allocate a clone of an existing Collider component.
// (Hint: Perform a shallow copy of the member variables.)
// Params:
//	 other = Pointer to the component to be cloned.
// Returns:
//	 If 'other' is valid and the memory allocation was successful,
//	   then return a pointer to the cloned component,
//	   else return NULL.
Collider* ColliderClone(const Collider* other)
{
	if (!other)
		return NULL;

	Collider* clone = calloc(1, other->memorySize);

	if (clone)
	{
		memcpy(clone, other, other->memorySize);
		clone->parent = NULL;
		return clone;
	}

	return NULL;
}

// Free the memory associated with a Collider component.
// (Also, set the collider pointer to NULL.)
// Params:
//	 collider = Pointer to the Collider component.
void ColliderFree(Collider** collider)
{
	free(*collider);
	*collider = NULL;
}

// Set the parent Entity for a Collider component.
// Params:
//	 collider = Pointer to the Collider component.
//	 parent = Pointer to the parent Entity.
void ColliderSetParent(Collider* collider, Entity* parent)
{
	if (collider && parent)
		collider->parent = parent;
}

// Check if two Entities are colliding.
// (Hint: Refer to the project instructions for implementation suggestions.)
// (Hint: Make sure to call the handler for both colliders, passing the 
//	  parent Entity pointers in the correct order!)
// Params:
//	 collider1 = Pointer to the first Collider component.
//	 collider2 = Pointer to the second Collider component.
void ColliderCheck(const Collider* collider, const Collider* other)
{
	// Check for collisions
	if (!ColliderIsColliding(collider, other))
		return;

	if (collider->handler)
		collider->handler(collider->parent, other->parent);

	if (other->handler)
		other->handler(other->parent, collider->parent);
}

// Set the collision event handler for a collider.
// (Hint: This allows other components, such as Behaviors, to respond to collision events.)
// (Note: It is acceptable for the handler to be NULL.  This allows an existing handler to be removed.)
// Params:
//	 collider = Pointer to the Collider component.
//	 handler = Pointer to the collision event handler (may be NULL).
void ColliderSetCollisionHandler(Collider* collider, CollisionEventHandler handler)
{
	if (!handler)
	{
		collider->handler = NULL;
		return;
	}

	collider->handler = handler;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

static bool ColliderIsColliding(const Collider* collider1, const Collider* collider2)
{
	if (collider1->type == ColliderTypeCircle && collider2->type == ColliderTypeCircle)
		return ColliderCircleIsCollidingWithCircle(collider1, collider2);

	else if (collider1->type == ColliderTypeCircle && collider2->type == ColliderTypeLine)
		return ColliderLineIsCollidingWithCircle(collider2, collider1);

	else if (collider1->type == ColliderTypeLine && collider2->type == ColliderTypeCircle)
		return ColliderLineIsCollidingWithCircle(collider1, collider2);

	else if (collider1->type == ColliderTypeLine && collider2->type == ColliderTypeLine)
		return false;

	return false;
}