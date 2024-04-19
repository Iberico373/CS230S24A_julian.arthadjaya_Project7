//------------------------------------------------------------------------------
//
// File Name:	Sprite.h
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

typedef struct Mesh Mesh;
typedef struct SpriteSource SpriteSource;
typedef struct Sprite Sprite;
typedef struct Transform Transform;
typedef FILE* Stream;

//------------------------------------------------------------------------------
// Public Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

class Sprite : public Component
{
public:
	Sprite();
	Sprite(const Sprite& other);

	Component* Clone() const override;
	void Render() override;

	// Read the properties of a Sprite component from a file.
	void Read(Stream stream) override;

	// Get a Sprite's alpha value.
	float GetAlpha();

	// Set a Sprite's alpha value.
	void SetAlpha(float alpha);

	// Set a Sprite's current frame.
	void SetFrame(unsigned int frameIndex);

	// Set the Sprite's mesh.
	void SetMesh(const Mesh* mesh);

	// Set a new SpriteSource for the specified Sprite.
	void SetSpriteSource(const SpriteSource* spriteSource);

	// Assign a text string to a Sprite component.  This will allow a sequence of
	void SetText(const char* text);

private:
	// The frame currently being displayed (for sprite sheets).
	unsigned int mFrameIndex;

	// The alpha transparency to use when drawing the sprite.
	float mAlpha;

	// The sprite source used when drawing (NULL = simple colored mesh).
	const SpriteSource* mSpriteSource;

	// The mesh used to draw the sprite.
	const Mesh* mMesh;

	// Zero-terminated string used to display sprite text.
	const char* mText;
};

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Dynamically allocate a new Sprite component.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
// (NOTE: You must initialize the 'alpha' member variable to 1.0f.)
// Returns:
//	 If the memory allocation was successful,
//	   then return a pointer to the allocated memory,
//	   else return NULL.
Sprite* SpriteCreate(void);

// Dynamically allocate a clone of an existing Sprite.
// (Hint: Perform a shallow copy of the member variables.)
// Params:
//	 other = Pointer to the component to be cloned.
// Returns:
//	 If 'other' is valid and the memory allocation was successful,
//	   then return a pointer to the cloned component,
//	   else return NULL.
Sprite* SpriteClone(const Sprite* other);

// Free the memory associated with a Sprite component.
// (NOTE: The Sprite pointer must be set to NULL.)
// Params:
//	 sprite = Pointer to the Sprite pointer.
void SpriteFree(Sprite** sprite);

/*----------------------------------------------------------------------------*/