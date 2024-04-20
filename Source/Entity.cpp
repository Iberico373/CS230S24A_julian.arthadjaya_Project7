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
#include "Scene.h"
//#include "Behavior.h"
//#include "BehaviorSpaceship.h"
//#include "BehaviorBullet.h"
//#include "BehaviorAsteroid.h"
//#include "BehaviorHudText.h"
//#include "Collider.h"
//#include "ColliderCircle.h"
//#include "ColliderLine.h"

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

Entity::Entity()
	: mName("")
	, mIsDestroyed(false)
	, mComponents(std::vector<Component*>(6))
{
}

// Dynamically allocate a clone of an existing Entity.
Entity::Entity(const Entity& other)
	: mIsDestroyed(false)
	, mComponents(std::vector<Component*>(6))
{
	strcpy_s(mName, _countof(mName), other.mName);

	for (auto component : other.mComponents)
	{
		if (component)
			Add(component->Clone());
	}
}

// Free the memory associated with an Entity.
Entity::~Entity()
{
	for (auto component : mComponents)
	{
		if (component)
		{
			delete component;
			component = nullptr;
		}
	}
}

void Entity::Add(Component* component)
{
	if (component)
	{
		// Set the component's parent
		component->Parent(this);

		// Add the component to the components list
		mComponents[component->Type()] = component;
	}
}

Component* Entity::Get(Component::ComponentType type)
{
	return mComponents[type];
}

// Read (and construct) the components associated with a entity.
void Entity::Read(Stream stream)
{
	if (stream)
	{
		SetName(StreamReadToken(stream));

		while (true)
		{
			const char* token = StreamReadToken(stream);

			if (strncmp(token, "Animation", _countof("Animation")) == 0)
			{
				Animation* animation = new Animation();
				animation->Read(stream);
				Add(animation);
			}

			else if (strncmp(token, "Physics", _countof("Physics")) == 0)
			{
				Physics* physics = new Physics();
				physics->Read(stream);
				Add(physics);
			}

			else if (strncmp(token, "Sprite", _countof("Sprite")) == 0)
			{
				Sprite* sprite = new Sprite();
				sprite->Read(stream);
				Add(sprite);
			}

			else if (strncmp(token, "Transform", _countof("Transform")) == 0)
			{
				Transform* transform = new Transform();
				transform->Read(stream);
				Add(transform);
			}

			else
			{
				break;
			}

			/*else if (strncmp(token, "BehaviorSpaceship", _countof("BehaviorSpaceship")) == 0)
			{
				Behavior* behavior = BehaviorSpaceshipCreate();
				BehaviorRead(behavior, stream);
				EntityAddBehavior(entity, behavior);
			}

			else if (strncmp(token, "BehaviorBullet", _countof("BehaviorBullet")) == 0)
			{
				Behavior* behavior = BehaviorBulletCreate();
				BehaviorRead(behavior, stream);
				EntityAddBehavior(entity, behavior);
			}

			else if (strncmp(token, "BehaviorAsteroid", _countof("BehaviorAsteroid")) == 0)
			{
				Behavior* behavior = BehaviorAsteroidCreate();
				BehaviorRead(behavior, stream);
				EntityAddBehavior(entity, behavior);
			}

			else if (strncmp(token, "BehaviorHudText", _countof("BehaviorHudText")) == 0)
			{
				Behavior* behavior = BehaviorHudTextCreate();
				BehaviorHudTextRead(behavior, stream);
				EntityAddBehavior(entity, behavior);
			}

			else if (strncmp(token, "ColliderCircle", _countof("ColliderCircle")) == 0)
			{
				Collider* collider = ColliderCircleCreate();
				ColliderCircleRead(collider, stream);
				EntityAddCollider(entity, collider);
			}

			else if (strncmp(token, "ColliderLine", _countof("ColliderLine")) == 0)
			{
				Collider* collider = ColliderLineCreate();
				ColliderLineRead(collider, stream);
				EntityAddCollider(entity, collider);
			}*/
		}
	}
}

// Flag an Entity for destruction.
void Entity::Destroy()
{
	mIsDestroyed = true;
}

// Check whether an Entity has been flagged for destruction.
bool Entity::IsDestroyed()
{
	return mIsDestroyed;
}

// Set the Entity's name.
void Entity::SetName(const char* name)
{
	strcpy_s(mName, _countof(mName), name);
}

// Get the Entity's name.
const char* Entity::GetName()
{
	return mName;
}

// Compare the Entity's name with the specified name.
bool Entity::IsNamed(const char* name)
{
	return !strcmp(mName, name);
}

// Update any components attached to the Entity.
void Entity::Update(float dt)
{
	for (auto component : mComponents)
	{
		if (component)
			component->Update(dt);
	}
}

// Render any visible components attached to the Entity.
void Entity::Render()
{
	for (auto component : mComponents)
	{
		if (component)
			component->Render();
	}
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

