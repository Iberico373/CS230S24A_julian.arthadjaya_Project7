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

// You are free to change the contents of this structure as long as you do not
//   change the public interface declared in the header.
typedef struct Transform
{
	// The translation (or world position) of an entity.
	Vector2D	translation;

	// The rotation (or orientation) of an entity (in radians).
	float	rotation;

	// The scale (or size) of an entity.
	// (Hint: This should be initialized to (1, 1).)
	Vector2D	scale;

	// True if the transformation matrix needs to be recalculated.
	// (Hint: This should be initialized to true.)
	// (Hint: This should be set to true when the Transform data changes.)
	bool	isDirty;

	// The transformation matrix resulting from concatenating the matrices
	//   representing the translation, rotation, and scale transformations.
	//	 (e.g. matrix = Translation*Rotation*Scale matrices)
	Matrix2D	matrix;

} Transform;

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
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
// (Hint: You must initialize the scale values to non-zero values.)
// Returns:
//	 If the memory allocation was successful,
//	   then return a pointer to the allocated memory,
//	   else return NULL.
Transform* TransformCreate(void)
{
	Transform* transform = (Transform*)calloc(1, sizeof(Transform));

	if (transform)
	{
		transform->scale = { 1.0f, 1.0f };
		transform->isDirty = true;

		return transform;
	}
	
	return NULL;
}

// Dynamically allocate a clone of an existing Transform.
// (Hint: Perform a shallow copy of the member variables.)
// Params:
//	 other = Pointer to the component to be cloned.
// Returns:
//	 If 'other' is valid and the memory allocation was successful,
//	   then return a pointer to the cloned component,
//	   else return NULL.
Transform* TransformClone(const Transform* other)
{
	if (!other)
		return NULL;

	Transform* transform = TransformCreate();

	if (!transform)
		return NULL;

	*transform = *other;

	return transform;	
}

// Free the memory associated with a Transform component.
// (NOTE: The Transform pointer must be set to NULL.)
// Params:
//	 transform = Pointer to the Transform pointer.
void TransformFree(Transform** transform)
{
	free(*transform);
	*transform = NULL;
}

// Read the properties of a Transform component from a file.
// [NOTE: Read the translation value using StreamReadVector2D.]
// [NOTE: Read the rotation value using StreamReadFloat.]
// [NOTE: Read the scale value using StreamReadVector2D.]
// Params:
//	 transform = Pointer to the Transform component.
//	 stream = The data stream used for reading.
void TransformRead(Transform* transform, Stream stream)
{
	StreamReadVector2D(stream, &(transform->translation));
	transform->rotation =  StreamReadFloat(stream);
	StreamReadVector2D(stream, &(transform->scale));
}

// Get the transform matrix, based upon translation, rotation and scale settings.
// (HINT: If the isDirty flag is true, then recalculate the transform matrix.)
// Params:
//	 transform = Pointer to the Transform component.
// Returns:
//	 If the Transform pointer is valid,
//		then return a pointer to the component's matrix structure,
//		else return a NULL pointer.
const Matrix2D* TransformGetMatrix(Transform* transform)
{
	if (!transform)
		return NULL;

	if (!transform->isDirty)
		return &(transform->matrix);

	Matrix2D translation = { 0.0f };
	Matrix2DTranslate(&translation, transform->translation.x, transform->translation.y);

	Matrix2D rotation = { 0.0f };
	Matrix2DRotRad(&rotation, transform->rotation);

	Matrix2D scale = { 0.0f };
	Matrix2DScale(&scale, transform->scale.x, transform->scale.y);

	Matrix2D result = { 0.0f };
	Matrix2DConcat(&result, &rotation, &scale);
	Matrix2DConcat(&transform->matrix, &translation, &result);

	transform->isDirty = false;
	return &(transform->matrix);
}

// Get the translation of a Transform component.
// Params:
//	 transform = Pointer to the Transform component.
// Returns:
//	 If the Transform pointer is valid,
//		then return a pointer to the component's translation,
//		else return a NULL pointer.
const Vector2D* TransformGetTranslation(const Transform* transform)
{
	if (transform)
	{
		return &(transform->translation);
	}

	return NULL;
}

// Get the rotation value of a Transform component.
// Params:
//	 transform = Pointer to the Transform component.
// Returns:
//	 If the Transform pointer is valid,
//		then return the component's rotation value (in radians),
//		else return 0.0f.
float TransformGetRotation(const Transform* transform)
{
	if (transform)
	{
		return transform->rotation;
	}

	return 0.0f;
}

// Get the scale of a Transform component.
// Params:
//	 transform = Pointer to the Transform component.
// Returns:
//	 If the Transform pointer is valid,
//		then return a pointer to the component's scale,
//		else return a NULL pointer.
const Vector2D* TransformGetScale(const Transform* transform)
{
	if (transform)
	{
		return &(transform->scale);
	}

	return NULL;
}

// Set the translation of a Transform component.
// Params:
//	 transform = Pointer to the Transform component.
//	 translation = Pointer to the new translation.
void TransformSetTranslation(Transform* transform, const Vector2D* translation)
{
	if (transform && translation)
	{
		transform->translation = *translation;
		transform->isDirty = true;
	}
}

// Set the rotation of a Transform component.
// Params:
//	 transform = Pointer to the Transform component.
//	 rotation = The rotation value (in radians).
void TransformSetRotation(Transform* transform, float rotation)
{
	if (transform)
	{
		transform->rotation = rotation;
		transform->isDirty = true;
	}
}

// Set the scale of a Transform component.
// Params:
//	 transform = Pointer to the Transform component.
//	 translation = Pointer to the new scale.
void TransformSetScale(Transform* transform, const Vector2D* scale)
{
	if (transform && scale)
	{
		transform->scale = *scale;
		transform->isDirty = true;
	}
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

