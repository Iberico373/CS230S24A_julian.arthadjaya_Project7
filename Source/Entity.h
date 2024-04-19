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

#define Has(type) GetComponent<type>(Component::c##type)

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include <vector>
#include "Component.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

typedef struct Animation Animation;
typedef struct Behavior Behavior;
typedef struct Collider Collider;
typedef struct Entity Entity;
typedef struct Physics Physics;
typedef struct Sprite Sprite;
typedef struct Transform Transform;
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

	void Add(Component* component);
	Component* Get(Component::ComponentType type);

	template<typename type>
	type* GetComponent(Component::ComponentType typeId)
	{
		return static_cast<type*>(Get(typeId));
	}

	// Read (and construct) the components associated with a entity.
	void Read(Stream stream);

	// Flag an Entity for destruction.
	void Destroy();

	// Check whether an Entity has been flagged for destruction.
	bool IsDestroyed();

	// Set the Entity's name.
	void SetName(const char* name);

	// Get the Entity's name.
	const char* GetName();

	// Compare the Entity's name with the specified name.
	bool IsNamed(const char* name);

	void Update(float dt);
	void Render();
private:
	// The name of the entity.
	char mName[32];

	// Flag to indicate that the Entity should be destroyed after it has been updated.
	bool mIsDestroyed;

	std::vector<Component*> mComponents;
};

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------