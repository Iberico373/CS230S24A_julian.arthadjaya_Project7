//------------------------------------------------------------------------------
//
// File Name:	Collider.h
// Author(s):	Doug Schilling (dschilling)
// Project:		Project 6
// Course:		CS230S24
//
// Copyright © 2024 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
	/* Assume C declarations for C++ */
#endif

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

typedef struct Collider Collider;
typedef struct Entity Entity;
typedef FILE* Stream;

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

typedef enum ColliderType
{
	ColliderTypeNone,
	ColliderTypeCircle,
	ColliderTypeLine,

} ColliderType;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

typedef void(*CollisionEventHandler)(Entity* entity1, Entity* entity2);

// This structure must now be declared publicly so that it may be included in
//   the private ColliderCircle and ColliderLine structures.
typedef struct Collider
{
	// Pointer to the collider's parent Entity.
	Entity* parent;

	// The type of collider used by this component.
	// (Currently, Circle or Line).
	ColliderType type;

	// Pointer to a function that handles collisions between entities.
	CollisionEventHandler	handler;

	// The amount of memory to be allocated when cloning a component of this type.
	unsigned int	memorySize;

} Collider;

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

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
Collider* ColliderClone(const Collider* other);

// Free the memory associated with a Collider component.
// (NOTE: The Collider pointer must be set to NULL.)
// Params:
//	 collider = Pointer to the Collider pointer.
void ColliderFree(Collider** collider);

// Set the parent Entity for a Collider component.
// Params:
//	 collider = Pointer to the Collider component.
//	 parent = Pointer to the parent Entity.
void ColliderSetParent(Collider* collider, Entity* parent);

// Check if two Entities are colliding.
// (Hint: Refer to the project instructions for implementation suggestions.)
// (Hint: Make sure to call the handler for both colliders, passing the 
//	  parent Entity pointers in the correct order!)
// Params:
//	 collider1 = Pointer to the first Collider component.
//	 collider2 = Pointer to the second Collider component.
void ColliderCheck(const Collider* collider, const Collider* other);

// Set the collision event handler for when two entities collide.
// (Hint: This allows other components, such as Behaviors, to respond to collision events.)
// (Note: It is acceptable for the handler to be NULL.  This allows an existing handler to be removed.)
// Params:
//	 collider = Pointer to the Collider component.
//	 handler = Pointer to the collision event handler (may be NULL).
void ColliderSetCollisionHandler(Collider* collider, CollisionEventHandler handler);

//------------------------------------------------------------------------------

#ifdef __cplusplus
}                       /* End of extern "C" { */
#endif

