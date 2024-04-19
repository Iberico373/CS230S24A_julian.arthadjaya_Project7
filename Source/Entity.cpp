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
	this->Name(other.mName);
	mIsDestroyed = false;

	for (auto component : other.mComponents)
	{
		EntityAddComponent(component->Clone());
	}
}

void Entity::EntityDestroy()
{
	mIsDestroyed = true;
}

bool Entity::EntityIsDestroyed()
{
	return mIsDestroyed;
}

void Entity::EntityAddComponent(Component* component)
{
	if (component)
	{
		// Set the component's parent
		component->Parent(this);

		// Add the component to the components array
		Component::ComponentType type = component->Type();
		mComponents[type] = component;
	}
}

Component* Entity::EntityGet(Component::ComponentType type)
{
	return mComponents[type];
}

void Entity::Name(const char* name)
{
	strcpy(mName, name); 
}

const char* Entity::Name()
{
	return mName;
}

bool Entity::EntityIsNamed(const char* name)
{
	return !(strcmp(mName, name));
}

void Entity::EntityUpdate(float dt)
{
	for (auto component : mComponents)
	{
		component->Update(dt);
	}
}

void Entity::EntityRender()
{
	for (auto component : mComponents)
	{
		component->Render();
	}
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

