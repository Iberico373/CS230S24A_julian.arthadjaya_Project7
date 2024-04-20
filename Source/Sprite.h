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
typedef class Transform Transform;

//------------------------------------------------------------------------------
// Public Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

class Sprite : public Component
{
public:
	// Dynamically allocate a new Sprite component.
	Sprite();
	Sprite(const Sprite& other);

	// Free the memory associated with a Sprite component.
	~Sprite();

	// Dynamically allocate a clone of an existing Sprite.
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

/*----------------------------------------------------------------------------*/