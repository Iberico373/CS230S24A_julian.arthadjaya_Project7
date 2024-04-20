//------------------------------------------------------------------------------
//
// File Name:	Physics.c
// Author(s):	Julian Artha Djaya (julian.arthadjaya)
// Project:		Project 0
// Course:		CS230S24
//
// Copyright © 2024 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Entity.h"
#include "Transform.h"
#include "Physics.h"

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

// Dynamically allocate a new Physics component.
Physics::Physics()
	: Component(cPhysics)
	, mRotationalVelocity(0.0f)
	, mDrag(1.0f)
{
	mOldTranslation = { 0.0f, 0.0f };
	mAcceleration = { 0.0f, 0.0f };
	mVelocity = { 0.0f, 0.0f };
}

Physics::Physics(const Physics& other) : Component(other.Type())
{
	*this = other;
}

// Dynamically allocate a clone of an existing Physics component.
Component* Physics::Clone() const
{	

	return new Physics(*this);
}

// Update the state of a Physics component using the Semi-Implicit Euler method
void Physics::Update(float dt)
{
	Transform* transform = Parent()->Has(Transform);

	if (transform)
	{
		// Set translation
		mOldTranslation = *(transform->GetTranslation());
		Vector2D newTranslation = { 0, 0 };

		Vector2DScaleAdd(&(mVelocity), &(mAcceleration), &(mVelocity), dt);
		Vector2DScale(&(mVelocity), &(mVelocity), mDrag);
		Vector2DScaleAdd(&newTranslation, &(mVelocity), &mOldTranslation, dt);
		transform->SetTranslation(&newTranslation);

		// Set rotation
		float rotation = transform->GetRotation() + (mRotationalVelocity * dt);
		transform->SetRotation(rotation);
	}
}

// Read the properties of a Physics component from a file.
void Physics::Read(Stream stream)
{
	StreamReadVector2D(stream, &(mAcceleration));
	StreamReadVector2D(stream, &(mVelocity));
}

// Get the acceleration of a Physics component.
const Vector2D* Physics::GetAcceleration() const
{
	return &mAcceleration;
}

// Get the velocity of a Physics component.
const Vector2D* Physics::GetVelocity() const
{
	return &mVelocity;
}

// Get the rotational velocity of a physics component.
float Physics::GetRotationalVelocity() const
{
	return mRotationalVelocity;
}

// Get the old translation (position) of a Physics component.
const Vector2D* Physics::GetOldTranslation() const
{
	return &mOldTranslation;
}

// Set the acceleration of a Physics component.
void Physics::SetAcceleration(const Vector2D* acceleration)
{
	mAcceleration = *acceleration;
}

// Set the velocity of a Physics component.
void Physics::SetVelocity(const Vector2D* velocity)
{
	mVelocity = *velocity;
}

// Set the rotational velocity of a physics component.
void Physics::SetRotationalVelocity(float rotationalVelocity)
{
	mRotationalVelocity = rotationalVelocity;
}

// Set the "drag" value of a Physics component.
void Physics::SetDrag( float drag)
{
	mDrag = drag;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

