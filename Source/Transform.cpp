//------------------------------------------------------------------------------
//
// File Name:	Transform.c
// Author(s):	Julian Artha Djaya (julian.arthadjaya)
// Project:		Project 0
// Course:		CS230S24
//
// Copyright © 2024 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Transform.h"
#include "Vector2D.h"
#include "Matrix2D.h"
#include "Stream.h"

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

// Dynamically allocate a new Transform component.
Transform::Transform()
	: Component(cTransform)
	, mRotation(0.0f)
	, mIsDirty(true)
{
	mTranslation = { 0.0f, 0.0f };
	mScale = { 1.0f, 1.0f };
	mMatrix = { 0.0f };
}

Transform::Transform(const Transform& other) : Component(other, Type())
{
	*this = other;
}

// Dynamically allocate a clone of an existing Transform.
Component* Transform::Clone() const
{
	return new Transform(*this);
}

// Read the properties of a Transform component from a file.
void Transform::Read(Stream stream)
{
	StreamReadVector2D(stream, &(mTranslation));
	mRotation =  StreamReadFloat(stream);
	StreamReadVector2D(stream, &(mScale));
}

// Get the transform matrix, based upon translation, rotation and scale settings.
const Matrix2D* Transform::GetMatrix()
{
	if (!mIsDirty)
		return &(mMatrix);

	Matrix2D translation = { 0.0f };
	Matrix2DTranslate(&translation, mTranslation.x, mTranslation.y);

	Matrix2D rotation = { 0.0f };
	Matrix2DRotRad(&rotation, mRotation);

	Matrix2D scale = { 0.0f };
	Matrix2DScale(&scale, mScale.x, mScale.y);

	Matrix2D result = { 0.0f };
	Matrix2DConcat(&result, &rotation, &scale);
	Matrix2DConcat(&mMatrix, &translation, &result);

	mIsDirty = false;
	return &(mMatrix);
}

// Get the translation of a Transform component.
const Vector2D* Transform::GetTranslation() const
{
	return &mTranslation;
}

// Get the rotation value of a Transform component.
float Transform::GetRotation() const
{
	return mRotation;
}

// Get the scale of a Transform component.
const Vector2D* Transform::GetScale() const
{
	return &mScale
}

// Set the translation of a Transform component.
void Transform::SetTranslation(const Vector2D* translation)
{
	mTranslation = *translation;
	mIsDirty = true;
}

// Set the rotation of a Transform component.
void Transform::SetRotation(float rotation)
{
	mRotation = rotation;
	mIsDirty = true;
}

// Set the scale of a Transform component.
void Transform::SetScale(const Vector2D* scale)
{
	mScale = *scale;
	mIsDirty = true;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

