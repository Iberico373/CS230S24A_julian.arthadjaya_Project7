//------------------------------------------------------------------------------
//
// File Name:	Component.h
// Author(s):	Julian Artha Djaya (julian.arthadjaya)
// Project:		Project 0
// Course:		CS230S24
//
// Copyright � 2024 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "Vector2D.h"
#include "Matrix2D.h"
#include "DGL.h"
#include "MeshLibrary.h"
#include "SpriteSourceLibrary.h"
#include "Trace.h"
#include "Stream.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

typedef class Entity Entity;

//------------------------------------------------------------------------------
// Public Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Typedefs:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

class Component
{
public:
	typedef enum ComponentType
	{
		cAnimation,
		cBehavior,
		cCollider,
		cPhysics,
		cSprite,
		cTransform
	}ComponentType;

	Component(Component::ComponentType type);
	virtual ~Component() { };

	__inline Component::ComponentType Type() const { return mType; }

	__inline void Parent(Entity* parent) { mParent = parent; }
	__inline Entity* Parent() const { return mParent; }

	// Clone is used to invoke C++ copy constructor
	virtual Component* Clone() const = 0;

	// Component-specific read code.
	virtual void Read(Stream stream) { UNREFERENCED_PARAMETER(stream); };

	// Component-specific update code.
	virtual void Update(float dt) { UNREFERENCED_PARAMETER(dt); }

	// Component-specific render code.
	virtual void Render() { };

private:
	ComponentType mType;
	Entity* mParent;
};

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------