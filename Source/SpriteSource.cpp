//------------------------------------------------------------------------------
//
// File Name:	SpriteSource.c
// Author(s):	Julian Artha Djaya (julian.arthadjaya)
// Project:		Project 0
// Course:		CS230S24
//
// Copyright © 2024 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "SpriteSource.h"
#include "Stream.h"
#include "DGL.h"

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

// You are free to change the contents of this structure as long as you do not
//   change the public interface declared in the header.
typedef struct SpriteSource
{
	// The name of the sprite source.
	// A buffer is used to allow each sprite source to have a unique name.
	// The buffer is hardcoded to an arbitrary length that will be sufficient
	//	 for all CS230 assignments.  You may, instead, use dynamically-allocated
	//	 arrays for this purpose but the additional complexity is unnecessary
	//	 and it is your responsibility to ensure that the memory is successfully
	//	 allocated and deallocated in all possible situations.
	char name[32];
	
	// The dimensions of the sprite sheet.
	// (Hint: These should be initialized to (1, 1).)
	int	numRows;
	int	numCols;

	// Pointer to a texture resource created using the DigiPen Graphics Library (DGL).
	const DGL_Texture* texture;

} SpriteSource;

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

// Dynamically allocate a new SpriteSource object.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
// (Hint: numRows and numCols should be initialized to 1.)
// Returns:
//	 If the memory allocation was successful,
//	   then return a pointer to the allocated memory,
//	   else return NULL.
SpriteSource* SpriteSourceCreate()
{
	SpriteSource* spriteSource = (SpriteSource*)calloc(1, sizeof(SpriteSource));

	if (spriteSource)
	{
		spriteSource->numRows = 1;
		spriteSource->numCols = 1;

		return spriteSource;
	}

	return NULL;
}

// Free the memory associated with a SpriteSource object.
// (NOTE: The SpriteSource pointer must be set to NULL.)
// Params:
//	 spriteSource = Pointer to the SpriteSource pointer.
void SpriteSourceFree(SpriteSource** spriteSource)
{
	free(*spriteSource);
	*spriteSource = NULL;
}

// Load a texture from a file (may be an Col x Row sprite sheet).
// (NOTE: The folder path, "Assets/" should be prepended to the texture name before
//    opening the file.  The function sprintf_s can be used for this purpose.
//    https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/sprintf-s-sprintf-s-l-swprintf-s-swprintf-s-l)
// Params:
//	 numCols = The number of columns in the sprite sheet.
//	 numRows = The number of rows in the sprite sheet.
//	 textureName = The name of the texture to be loaded.
void SpriteSourceLoadTexture(SpriteSource* spriteSource, int numCols, int numRows, const char* textureName)
{
	// Set numCols and numRows 
	spriteSource->numCols = numCols;
	spriteSource->numRows = numRows;

	// Load texture 
	char filePath[200] = " ";
	sprintf_s(filePath, _countof(filePath), "./Assets/%s", textureName);
	spriteSource->texture = DGL_Graphics_LoadTexture(filePath);
}

// Read the properties of a SpriteSource object from a file.
// 1: Read a token from the stream.
// 2: If the token read was "SpriteSource",
//	  a: Read a token from the stream and store it in the name variable.
//       (Hint: use strcpy_s for this purpose.)
//    b: Read the numCols and numRows values from the stream.
//	  c: Read a token from the stream. This is the path name of a texture.
//	  d: Call DGL_Graphics_LoadTexture and store the texture resource.
// Params:
//	 spriteSource = Pointer to the SpriteSource object.
//	 stream = Pointer to the data stream used for reading.
void SpriteSourceRead(SpriteSource* spriteSource, Stream stream)
{
	if (strncmp(StreamReadToken(stream), "SpriteSource", _countof("SpriteSource")) == 0)
	{
		strcpy_s(spriteSource->name, _countof(spriteSource->name), StreamReadToken(stream));
		spriteSource->numCols = StreamReadInt(stream);
		spriteSource->numRows = StreamReadInt(stream);
		spriteSource->texture =  DGL_Graphics_LoadTexture(StreamReadToken(stream));
	}
}

// Determines if a SpriteSource has the specified name.
// Params:
//	 spriteSource = Pointer to the SpriteSource object.
//	 name = Pointer to the name to be compared.
// Returns:
//	 If the SpriteSource and name pointers are valid,
//		then perform a string comparison and return the result (match = true),
//		else return false.
bool SpriteSourceIsNamed(const SpriteSource* spriteSource, const char* name)
{
	return !strcmp(spriteSource->name, name);
}

// Returns the maximum number of frames possible, given the dimensions of the sprite sheet.
// (Hint: Frame count = numCols * numRows.)
// Params:
//	 spriteSource = Pointer to the SpriteSource object.
// Returns:
//	 If the SpriteSource pointer is valid,
//		then return the calculated frame count (numCols * numRows),
//		else return 0.
unsigned int SpriteSourceGetFrameCount(const SpriteSource* spriteSource)
{
	if (spriteSource)
	{
		return (unsigned int)(spriteSource->numCols * spriteSource->numRows);
	}

	return 0;
}

// Returns the UV coordinates of the specified frame in a sprite sheet.
// (Hint: Refer to the Week 2 lecture slides for the correct calculations.)
// Params:
//	 spriteSource = Pointer to the SpriteSource object.
//	 frameIndex = A frame index within a sprite sheet.
//   u = Pointer to a float to contain the U coordinate. 
//   v = Pointer to a float to contain the V coordinate. 
void SpriteSourceGetUV(const SpriteSource* spriteSource, unsigned int frameIndex, float* u, float* v)
{
	float uSize = 1.0f / spriteSource->numCols;
	float vSize = 1.0f / spriteSource->numRows;

	*u = uSize * (frameIndex % spriteSource->numCols);
	*v = vSize * (frameIndex / spriteSource->numCols);
}

// Sets a SpriteSource texture for rendering.
// Params:
//	 spriteSource = Pointer to the SpriteSource object.
void SpriteSourceSetTexture(const SpriteSource* spriteSource)
{
	DGL_Graphics_SetTexture(spriteSource->texture);
}

// Sets the texture UV offsets for rendering.
// Params:
//	 spriteSource = Pointer to the SpriteSource object.
void SpriteSourceSetTextureOffset(const SpriteSource* spriteSource, unsigned frameIndex)
{
	float u = 0.0f;
	float v = 0.0f;
	SpriteSourceGetUV(spriteSource, frameIndex, &u, &v);

	DGL_Vec2 texOffset = { u, v };
	DGL_Graphics_SetCB_TextureOffset(&texOffset);
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

