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


Sprite::Sprite()
	: Component(Component::Sprite)
	, mFrameIndex(0)
	, mAlpha(1.0f)
	, mSpriteSource(nullptr)
	, mMesh(nullptr)
	, mText("")
{
}

Sprite::Sprite(const Sprite& other) : Component(other)
{
	*this = other;
}

Component* Sprite::Clone() const
{
	return new Sprite(*this);
}

void Sprite::Render(Transform* transform)
{

}

void Sprite::SpriteRead(Stream stream)
{
	mFrameIndex = StreamReadInt(stream);
	mAlpha = StreamReadFloat(stream);
	SpriteSetMesh(sprite,  MeshLibraryBuild(StreamReadToken(stream)));
	SpriteSetSpriteSource(sprite, SpriteSourceLibraryBuild(StreamReadToken(stream)));
}

// Render a Sprite (Sprite can be textured or untextured).
// Params:
//	 sprite = Pointer to the Sprite component.
//   transform = Pointer to the Transform component.
void SpriteRender(const Sprite* sprite, Transform* transform)
{
	if (!sprite || !sprite->mesh)
		return;

	if (sprite->spriteSource)
	{
		// Prepare to render a textured sprite
		DGL_Graphics_SetShaderMode(DGL_PSM_TEXTURE, DGL_VSM_DEFAULT);

		//Set texture and texture offsets
		SpriteSourceSetTexture(sprite->spriteSource);
		SpriteSourceSetTextureOffset(sprite->spriteSource, sprite->frameIndex);		
	}

	else
	{
		// Prepare to render an untextured sprite
		DGL_Graphics_SetShaderMode(DGL_PSM_COLOR, DGL_VSM_DEFAULT);
		DGL_Graphics_SetTexture(NULL);
	}

	// Set transparency (range 0.0f - 1.0f)
	DGL_Graphics_SetCB_Alpha(sprite->alpha);

	// Set blend color (RGBA, A = "strength" of blend)
	DGL_Color tint = { 0.0f, 0.0f, 0.0f, 0.0f };
	DGL_Graphics_SetCB_TintColor(&tint);

	if (!(sprite->text))
	{
		DGL_Graphics_SetCB_TransformMatrix(TransformGetMatrix(transform));

		// Render the mesh (list of triangles)
		MeshRender(sprite->mesh);
	}

	else
	{
		Matrix2D matrix = *(TransformGetMatrix(transform));

		Matrix2D offset = { 0 };
		Matrix2DTranslate(&offset, TransformGetScale(transform)->x, 0.0f);

		const char* text = sprite->text;

		while (*text != '\0')
		{
			// subtract by 32 for ascii
			unsigned int charIndex = *text - 32;
			SpriteSourceSetTextureOffset(sprite->spriteSource, charIndex);
			DGL_Graphics_SetCB_TransformMatrix(&matrix);
			MeshRender(sprite->mesh);
			++text;
			Matrix2DConcat(&matrix, &offset, &matrix);
		}
	}
	
}

// Get a Sprite's alpha value.
// Params:
//	 sprite = Pointer to the Sprite component.
// Returns:
//	 If the pointer is valid,
//		then return the Sprite's alpha value (a value between 0.0f and 1.0f),
//		else return 0.0f.
float SpriteGetAlpha(const Sprite* sprite)
{
	return sprite->alpha;
}

// Set a Sprite's alpha value.
// (NOTE: Make sure to clamp the resulting alpha value between 0.0f and 1.0f, as the
//	input value might be outside of this range.)
// (HINT: The min() and max() macros can be combined to create a clamp function.)
// Params:
//	 sprite = Pointer to the Sprite component.
//   alpha = The Sprite's new alpha value.
void SpriteSetAlpha(Sprite* sprite, float alpha)
{

	sprite->alpha = max(min(alpha, 1.0f), 0.0f);
}

// Set a Sprite's current frame.
// (NOTE: You must verify that the frameIndex parameter is within the
//	range [0 to frame count - 1] before changing the Sprite's frame index!)
// Params:
//	 sprite = Pointer to the Sprite component.
//   frameIndex = New frame index for the Sprite (0 .. frame count - 1).
// ADDITIONAL REQUIREMENTS:
// - This function must make the following function call:
//     TraceMessage("SpriteSetFrame: frame index = %d", frameIndex);
void SpriteSetFrame(Sprite* sprite, unsigned int frameIndex)
{
	unsigned int frameCount = SpriteSourceGetFrameCount(sprite->spriteSource);

	if (frameIndex >= 0 && frameIndex < frameCount)
	{
		sprite->frameIndex = frameIndex;
	}

	TraceMessage("SpriteSetFrame: frame index = %d", frameIndex);
}

// Set the Sprite's mesh.
// (NOTE: This mesh may be textured or untextured.)
// (NOTE: This mesh may contain any number of triangles.)
// Params:
//	 sprite = Pointer to the Sprite component.
//   mesh = Pointer to a Mesh object.
void SpriteSetMesh(Sprite* sprite, const Mesh* mesh)
{
	sprite->mesh = mesh;
}

// Set a new SpriteSource for the specified Sprite.
// (NOTE: The spriteSource parameter may be NULL.  This will remove an existing
//	texture from a Sprite and cause the Sprite to be displayed as a colored mesh.)
// Params:
//	 sprite = Pointer to the Sprite component.
//	 spriteSource = Pointer to a SpriteSource (this pointer may be NULL).
void SpriteSetSpriteSource(Sprite* sprite, const SpriteSource* spriteSource)
{
	sprite->spriteSource = spriteSource;
}

// Assign a text string to a Sprite component.  This will allow a sequence of
//	 characters to be displayed as text.
// (NOTE: The text parameter may be NULL.  This will remove an existing text
//	 string from a sprite and cause the sprite to be displayed normally.)
// Params:
//	 sprite = Pointer to the Sprite component.
//	 text = Pointer to a zero-terminated array of characters.
void SpriteSetText(Sprite* sprite, const char* text)
{
	if (text)
		sprite->text = text;
	else
		sprite->text = '\0';
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

