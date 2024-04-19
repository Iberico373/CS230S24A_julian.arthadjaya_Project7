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
Entity::Entity(const Entity& other)
{
	strcpy(mName, other.mName);
	mIsDestroyed = false;

	for (auto component : other.mComponents)
	{
		Add(component->Clone());
	}
}

void Entity::Add(Component* component)
{
	if (component)
	{
		// Set the component's parent
		component->Parent(this);

		// Add the component to the components list
		mComponents.push_back(component);
	}
}

Component* Entity::Get(Component::ComponentType type)
{
	return mComponents[type];
}

// Free the memory associated with an Entity.
void Entity::Free()
{
	for (auto component : mComponents)
	{
		delete component;
	}
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
				Animation* animation = AnimationCreate();
				AnimationRead(animation, stream);
				EntityAddAnimation(entity, animation);
			}

			else if (strncmp(token, "Physics", _countof("Physics")) == 0)
			{
				Physics* physics = PhysicsCreate();
				PhysicsRead(physics, stream);
				EntityAddPhysics(entity, physics);
			}

			else if (strncmp(token, "Sprite", _countof("Sprite")) == 0)
			{
				Sprite* sprite = SpriteCreate();
				SpriteRead(sprite, stream);
				EntityAddSprite(entity, sprite);
			}

			else if (strncmp(token, "Transform", _countof("Transform")) == 0)
			{
				Transform* transform = TransformCreate();
				TransformRead(transform, stream);
				EntityAddTransform(entity, transform);
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
	strcpy(mName, name);
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
		component->Update(dt);
	}
}

// Render any visible components attached to the Entity.
void Entity::Render()
{
	for (auto component : mComponents)
	{
		component->Render();
	}
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

