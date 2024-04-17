//------------------------------------------------------------------------------
//
// File Name:	Entity.c
// Author(s):	Julian Artha Djaya (julian.arthadjaya)
// Project:		Project 0
// Course:		CS230S24
//
// Copyright © 2024 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Entity.h"
#include "Animation.h"
#include "Physics.h"
#include "Sprite.h"
#include "Transform.h"
#include "Stream.h"
#include "Behavior.h"
#include "BehaviorSpaceship.h"
#include "BehaviorBullet.h"
#include "BehaviorAsteroid.h"
#include "BehaviorHudText.h"
#include "Collider.h"
#include "ColliderCircle.h"
#include "ColliderLine.h"
#include "Scene.h"

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

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Dynamically allocate a clone of an existing Entity.
// (Hint: Make sure to perform a shallow copy or deep copy, as appropriate.)
// (WARNING: You should use the EntityAdd* functions when attaching cloned
//    components to the cloned Entity.  This will ensure that the 'parent'
//    variable is set properly.)
// Params:
//	 other = Pointer to the Entity to be cloned.
// Returns:
//	 If 'other' is valid and the memory allocation was successful,
//	   then return a pointer to the cloned Entity,
//	   else return NULL.
Entity::Entity(const Entity& other)
{
	EntitySetName(other.name);
	isDestroyed = false;

	for (auto component : other.components)
	{
		EntityAddComponent(component->Clone());
	}
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

