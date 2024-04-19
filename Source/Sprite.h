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
#include "Mesh.h"
#include "MeshLibrary.h"
#include "SpriteSource.h"
#include "SpriteSourceLibrary.h"
#include "Transform.h"
#include "Stream.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

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
	
	~Sprite() { }

	Component* Clone() const override;

	void Render(Transform* transform) const override;

	void SpriteRead(Stream stream);

	void SpriteSetMesh(Mesh* mesh);

	float Alpha() const { return mAlpha; }
	void Alpha(float alpha) { mAlpha = alpha; }

	void FrameIndex(unsigned int frameIndex);

	void Mesh(const Mesh* mesh) { mMesh = mesh; }

	void Text(const char* text);

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