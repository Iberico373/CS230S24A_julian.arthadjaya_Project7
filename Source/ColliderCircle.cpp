//------------------------------------------------------------------------------
//
// File Name:	ColliderCircle.c
// Author(s):	Julian Artha Djaya (julian.arthadjaya)
// Project:		Project 0
// Course:		CS230S24
//
// Copyright © 2024 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "ColliderCircle.h"
#include "Collider.h"
#include "Transform.h"
#include "Entity.h"
#include "Vector2D.h"
#include "Stream.h"

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

typedef struct ColliderCircle
{
	// Inherit the base collider structure.
	Collider	base;

	// Radius of the circle collider.
	float radius;

} ColliderCircle;

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Dynamically allocate a new (circle) collider component.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
// (Hint: Make sure to initialize the ColliderType and memorySize correctly.)
// (Note: Set the radius to a non-zero value.  Typical values are 0.5f or 1.0f.)
Collider* ColliderCircleCreate(void)
{
	ColliderCircle* collider = (ColliderCircle*)calloc(1, sizeof(ColliderCircle));
	
	if (collider)
	{
		collider->base.type = ColliderTypeCircle;
		collider->base.memorySize = sizeof(ColliderCircle);
		collider->radius = 1.0f; 
		return (Collider*)collider;
	}

	return NULL;
}

// Read the properties of a ColliderCircle component from a file.
// (NOTE: Read the radius value using StreamReadFloat.)
// Params:
//	 collider = Pointer to the Collider component.
//	 stream = Pointer to the data stream used for reading.
void ColliderCircleRead(Collider* collider, Stream stream)
{
	ColliderCircle* colliderCircle = (ColliderCircle*)collider;
	colliderCircle->radius = StreamReadFloat(stream);
}

// Get the circle collider's radius.
// Params:
//	 collider = Pointer to the circle collider component.
// Returns:
//	 If the pointer is valid,
//	   then return the circle collider's radius,
//	   else return 0.0f.
float ColliderCircleGetRadius(const Collider* collider)
{
	if (collider)
	{
		ColliderCircle* colliderCircle = (ColliderCircle*)collider;
		return colliderCircle->radius;
	}

	return 0.0f;
}

// Set the circle collider's radius.
// Params:
//	 collider = Pointer to the circle collider component.
//   radius = the circle collider's new radius.
void ColliderCircleSetRadius(Collider* collider, float radius)
{
	ColliderCircle* colliderCircle = (ColliderCircle*)collider;
	colliderCircle->radius = radius;
}

// Check for collision between two circle colliders.
// Params:
//	 collider1 = Pointer to the first circle collider component.
//	 collider2 = Pointer to the second circle collider component.
bool ColliderCircleIsCollidingWithCircle(const Collider* collider, const Collider* other)
{
	// Get transform components
	Transform* trans1 = EntityGetTransform(collider->parent);
	Transform* trans2 = EntityGetTransform(other->parent);

	// Get the distance between both objects
	Vector2D pos1 = *TransformGetTranslation(trans1);
	Vector2D pos2 = *TransformGetTranslation(trans2);
	float dist = Vector2DSquareDistance(&pos2, &pos1);	

	// Get the radius of both objects
	float radius = (TransformGetScale(trans1)->x / 2.0f) + (TransformGetScale(trans2)->x / 2.0f);
	radius *= radius;

	// Check for collision
	if (dist <= radius)
		return true;

	return false;
}
//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

