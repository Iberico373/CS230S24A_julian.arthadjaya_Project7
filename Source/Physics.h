//------------------------------------------------------------------------------
//
// File Name:	Physics.h
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

#include "Component.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

typedef class Transform Transform;

//------------------------------------------------------------------------------
// Public Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

class Physics : public Component
{
public:
	// Dynamically allocate a new Physics component.
	Physics();
	Physics(const Physics& other);

	~Physics() { };

	// Dynamically allocate a clone of an existing Physics component.
	Component* Clone() const override;

	// Update the state of a Physics component using the Semi-Implicit Euler method,
	void Update(float dt) override;

	// Read the properties of a Physics component from a file.
	void Read(Stream stream) override;

	// Get the acceleration of a Physics component.
	const Vector2D* GetAcceleration() const;

	// Get the velocity of a Physics component.
	const Vector2D* GetVelocity() const;

	// Get the rotational velocity of a physics component.
	float GetRotationalVelocity() const;

	// Get the old translation (position) of a Physics component.
	const Vector2D* GetOldTranslation() const;

	// Set the acceleration of a Physics component.
	void SetAcceleration(const Vector2D* acceleration);

	// Set the velocity of a Physics component.
	void SetVelocity(const Vector2D* velocity);

	// Set the rotational velocity of a physics component.
	void SetRotationalVelocity(float rotationalVelocity);

	// Set the "drag" value of a Physics component.
	void SetDrag(float drag);

private:
	// Previous position.  May be used for resolving collisions.
	Vector2D	mOldTranslation;

	// Acceleration = inverseMass * (sum of forces)
	Vector2D	mAcceleration;

	// Velocity may be stored as a direction vector and speed scalar, instead.
	Vector2D	mVelocity;

	// Rotational velocity (in radians).
	float mRotationalVelocity;

	// Drag to be applied to moving objects in order to slow them down.
	float mDrag;

	// Used when calculating acceleration due to forces.
	// Used when resolving collision between two dynamic objects.
	//float		inverseMass;

};

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------