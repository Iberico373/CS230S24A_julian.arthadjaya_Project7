//------------------------------------------------------------------------------
//
// File Name:	Entity.h
// Author(s):	Doug Schilling (dschilling)
// Project:		Project 5
// Course:		CS230S24
//
// Copyright © 2024 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include <vector>
#include "Component.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

typedef FILE* Stream;

//------------------------------------------------------------------------------
// Public Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

class Entity
{
public:
	Entity(const Entity& other);

	// Free the memory associated with an Entity.
	// (NOTE: All attached components must be freed using the corresponding Free() functions.)
	// (NOTE: The Entity pointer must be set to NULL.)
	~Entity();

	// Read (and construct) the components associated with a entity.
	// Params:
	//	 stream = The data stream used for reading.
	void EntityRead(Stream stream);

	// Flag an Entity for destruction.
	// (Note: This is to avoid Entities being destroyed while they are being processed.)
	// Returns:
	//	 If 'entity' is valid,
	//	   then set the 'isDestroyed' flag,
	//	   else do nothing.
	void EntityDestroy();

	// Check whether an Entity has been flagged for destruction.
	// Returns:
	//	 If the Entity pointer is valid,
	//		then return the value in the "isDestroyed" flag,
	//		else return false.
	bool EntityIsDestroyed();

	// Add specified component
	void EntityAddComponent(Component* component);

	// Get specified component in entity
	Component* EntityGetComponent(ComponentType type);

	// Set the Entity's name.
	// [NOTE: Verify that both pointers are valid before setting the name.]
	// [NOTE: When setting the name, use strcpy_s() to reduce the risk of
	//	 buffer overruns. Additionally, do NOT hardcode the number "32" when
	//	 calling this function!  Instead, use the _countof() macro to get the
	//	 size of the "name" array.]
	// Params:
	//	 name = Pointer to the Entity's new name.
	void EntitySetName(const char* name);

	// Get the Entity's name.
	// Returns:
	//	 If the Entity pointer is valid,
	//		then return a pointer to the Entity's name,
	//		else return NULL.
	const char* EntityGetName();

	// Compare the Entity's name with the specified name.
	// Params:
	//   name = Pointer to the name to be checked.
	// Returns:
	//	 If the Entity pointer is valid and the two names match,
	//		then return true,
	//		else return false.
	bool EntityIsNamed(const char* name);

	// Update any components attached to the Entity.
	// (NOTE: You must first check for a valid pointer before calling this function.)
	// (HINT: Update the Animation first, as it might affect Behavior.)
	// (HINT: Update the Behavior second, as it might affect Physics.)
	// (HINT: Update the Physics last, before checking for collisions.)
	// Params:
	//	 dt = Change in time (in seconds) since the last game loop.
	void EntityUpdate(float dt);

	// Render any visible components attached to the Entity.
	void EntityRender();

private:
	// The name of the entity.
	char name[32];

	// Flag to indicate that the Entity should be destroyed after it has been updated.
	bool isDestroyed;

	// Container containing all components
	std::vector<Component*> components;
}

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------