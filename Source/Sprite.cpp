//------------------------------------------------------------------------------
//
// File Name:	Sprite.c
// Author(s):	Julian Artha Djaya (julian.arthadjaya)
// Project:		Project 0
// Course:		CS230S24
//
// Copyright © 2024 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Sprite.h"
#include "Stream.h"
#include "Trace.h"
#include "SpriteSource.h"
#include "Transform.h"
#include "Mesh.h"
#include "Matrix2D.h"
#include "DGL.h"
#include "MeshLibrary.h"
#include "SpriteSourceLibrary.h"

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


// Dynamically allocate a new Sprite component.
Sprite::Sprite()
	: Component(Component::cSprite)
	, mFrameIndex(0)
	, mAlpha(1.0f)
	, mSpriteSource(nullptr)
	, mMesh(nullptr)
	, mText("")
{
}

// Dynamically allocate a clone of an existing Sprite.
Sprite::Sprite(const Sprite& other) : Component(other.Type())
{
	*this = other;
}

Component* Sprite::Clone() const
{
	return new Sprite(*this);
}

// Render a Sprite (Sprite can be textured or untextured).
void Sprite::Render()
{
	if (mSpriteSource)
	{
		// Prepare to render a textured sprite
		DGL_Graphics_SetShaderMode(DGL_PSM_TEXTURE, DGL_VSM_DEFAULT);

		//Set texture and texture offsets
		SpriteSourceSetTexture(mSpriteSource);
		SpriteSourceSetTextureOffset(mSpriteSource, mFrameIndex);
	}

	else
	{
		// Prepare to render an untextured sprite
		DGL_Graphics_SetShaderMode(DGL_PSM_COLOR, DGL_VSM_DEFAULT);
		DGL_Graphics_SetTexture(NULL);
	}

	// Set transparency (range 0.0f - 1.0f)
	DGL_Graphics_SetCB_Alpha(mAlpha);

	// Set blend color (RGBA, A = "strength" of blend)
	DGL_Color tint = { 0.0f, 0.0f, 0.0f, 0.0f };
	DGL_Graphics_SetCB_TintColor(&tint);

	if (!(mText))
	{
		DGL_Graphics_SetCB_TransformMatrix(TransformGetMatrix(transform));

		// Render the mesh (list of triangles)
		MeshRender(mMesh);
	}

	else
	{
		Matrix2D matrix = *(TransformGetMatrix(transform));

		Matrix2D offset = { 0 };
		Matrix2DTranslate(&offset, TransformGetScale(transform)->x, 0.0f);

		const char* text = mText;

		while (*text != '\0')
		{
			// subtract by 32 for ascii
			unsigned int charIndex = *text - 32;
			SpriteSourceSetTextureOffset(mSpriteSource, charIndex);
			DGL_Graphics_SetCB_TransformMatrix(&matrix);
			MeshRender(mMesh);
			++text;
			Matrix2DConcat(&matrix, &offset, &matrix);
		}
	}

}

// Read the properties of a Sprite component from a file.
void Sprite::Read(Stream stream)
{
	mFrameIndex = StreamReadInt(stream);
	mAlpha = StreamReadFloat(stream);
	SetMesh(MeshLibraryBuild(StreamReadToken(stream)));
	SetSpriteSource(SpriteSourceLibraryBuild(StreamReadToken(stream)));
}

// Get a Sprite's alpha value.
float Sprite::GetAlpha()
{
	return mAlpha;
}

// Set a Sprite's alpha value.
void Sprite::SetAlpha(float alpha)
{
	mAlpha = max(min(alpha, 1.0f), 0.0f);
}

// Set a Sprite's current frame.
void Sprite::SetFrame(unsigned int frameIndex)
{
	unsigned int frameCount = SpriteSourceGetFrameCount(mSpriteSource);

	if (frameIndex >= 0 && frameIndex < frameCount)
	{
		mFrameIndex = frameIndex;
	}

	TraceMessage("SpriteSetFrame: frame index = %d", frameIndex);
}

// Set the Sprite's mesh.
void Sprite::SetMesh(const Mesh* mesh)
{
	mMesh = mesh;
}

// Set a new SpriteSource for the specified Sprite.
void Sprite::SetSpriteSource(const SpriteSource* spriteSource)
{
	mSpriteSource = spriteSource;
}

// Assign a text string to a Sprite component.  This will allow a sequence of
// characters to be displayed as text.
void Sprite::SetText(const char* text)
{
	if (text)
		mText = text;
	else
		mText = '\0';
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

