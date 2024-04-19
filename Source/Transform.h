//------------------------------------------------------------------------------
//
// File Name:	Transform.h
// Author(s):	Doug Schilling (dschilling)
// Project:		Project 4
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

typedef struct DGL_Mat4 Matrix2D;
typedef struct Transform Transform;
typedef struct DGL_Vec2 Vector2D;
typedef FILE* Stream;

//------------------------------------------------------------------------------
// Public Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

// You are free to change the contents of this structure as long as you do not
//   change the public interface declared in the header.
class Transform : public Component
{
public:
	// Dynamically allocate a new Transform component.
	Transform();
	Transform(const Transform& other);

	// Dynamically allocate a clone of an existing Transform.
	Component* Clone() const override;

	// Read the properties of a Transform component from a file.
	void Read(Stream stream) override;

	// Get the transform matrix, based upon translation, rotation and scale settings.
	const Matrix2D* GetMatrix();

	// Get the translation of a Transform component.
	const Vector2D* GetTranslation();

	// Get the rotation value of a Transform component.
	float GetRotation() const;

	// Get the scale of a Transform component.
	const Vector2D* GetScale() const;

	// Set the translation of a Transform component.
	void SetTranslation(const Vector2D* translation);

	// Set the rotation of a Transform component.
	void SetRotation(float rotation);

	// Set the scale of a Transform component.
	void SetScale(const Vector2D* scale);

private:
	// The translation (or world position) of an entity.
	Vector2D	mTranslation;

	// The rotation (or orientation) of an entity (in radians).
	float	mRotation;

	// The scale (or size) of an entity.
	// (Hint: This should be initialized to (1, 1).)
	Vector2D	mScale;

	// True if the transformation matrix needs to be recalculated.
	// (Hint: This should be initialized to true.)
	// (Hint: This should be set to true when the Transform data changes.)
	bool	mIsDirty;

	// The transformation matrix resulting from concatenating the matrices
	//   representing the translation, rotation, and scale transformations.
	//	 (e.g. matrix = Translation*Rotation*Scale matrices)
	Matrix2D	mMatrix;

};

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------