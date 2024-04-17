//------------------------------------------------------------------------------
//
// File Name:	SpriteSourceLibrary.c
// Author(s):	Julian Artha Djaya (julian.arthadjaya)
// Project:		Project 0
// Course:		CS230S24
//
// Copyright © 2024 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "SpriteSourceLibrary.h"
#include "SpriteSource.h"
#include "Stream.h"

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

// You are free to change the contents of this structure as long as you do not
//   change the public interface declared in the header.
typedef struct SpriteSourceLibrary
{
	// The number of sprite sources currently in the list.
	unsigned int spriteSourceCount;

	// A list of all currently loaded sprite sources.
	// This list can be a fixed-length array (minimum size of 10 entries)
	// or a dynamically-sized array, such as a linked list.
	SpriteSource* spriteSourceList[10];

} SpriteSourceLibrary;

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

static SpriteSourceLibrary sprites;

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

static void SpriteSourceLibraryAdd(SpriteSource* SpriteSource);

static SpriteSource* SpriteSourceLibraryFind(const char* name);

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Initialize the SpriteSource Manager.
// (NOTE: Make sure to initialize all memory to zero.)
void SpriteSourceLibraryInit()
{
	sprites.spriteSourceCount = 0;
	
	for (int i = 0; i < 10; ++i)
	{
		sprites.spriteSourceList[i] = NULL;
	}
}

// Create a SpriteSource and add it to the SpriteSource manager.
// 1: Call SpriteSourceLibraryFind() to determine if the sprite source already exists
// 2: If the named sprite source does not already exist,
//	  a: Use sprintf_s() to construct a path name using spriteSourceName
//	     (HINT: The correct path name should be constructed using "Data/%s.txt".)
//	  b: Call StreamOpen(), passing the path name
//    c: If the stream was opened successfully,
//		 1: Call SpriteSourceCreate() to construct a new, empty SpriteSource.
//       2: Call SpriteSourceRead() to read the SpriteSource data from the file.
//		 3: Call SpriteSourceLibraryAdd(), passing the created SpriteSource.
//       4: Close the stream.
// 3: Return the SpriteSource (already existing or just created)
// Params:
//	 SpriteSourceName = The name of the SpriteSource to be created.
// Returns:
//	 If the SpriteSource already exists or was created successfully,
//	   then return a pointer to the SpriteSource,
//	   else return NULL.
const SpriteSource* SpriteSourceLibraryBuild(const char* spriteSourceName)
{
	SpriteSource* spriteSource = SpriteSourceLibraryFind(spriteSourceName);

	if (spriteSource)
		return spriteSource;

	char filePath[FILENAME_MAX] = "";
	sprintf_s(filePath, _countof(filePath), "./Data/%s.txt", spriteSourceName);

	Stream stream = StreamOpen(filePath);

	if (stream)
	{
		spriteSource = SpriteSourceCreate();

		if (spriteSource)
		{
			SpriteSourceRead(spriteSource, stream);
			SpriteSourceLibraryAdd(spriteSource);
			StreamClose(&stream);
			return spriteSource;
		}

		StreamClose(&stream);
	}

	return NULL;
}

// Free all SpriteSource objects in the SpriteSource Library.
// (NOTE: You must call SpriteSourceFree() to free each SpriteSource object.)
// (HINT: The list should contain nothing but NULL pointers once this function is done.)
void SpriteSourceLibraryFreeAll()
{
	for (unsigned int i = 0; i < sprites.spriteSourceCount; ++i)
	{
		SpriteSourceFree(&(sprites.spriteSourceList[i]));
	}

	sprites.spriteSourceCount = 0;
}


//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

static void SpriteSourceLibraryAdd(SpriteSource* SpriteSource)
{
	sprites.spriteSourceList[sprites.spriteSourceCount] = SpriteSource;
	++sprites.spriteSourceCount;
}

static SpriteSource* SpriteSourceLibraryFind(const char* name)
{
	for (unsigned int i = 0; i < sprites.spriteSourceCount; ++i)
	{
		if (SpriteSourceIsNamed(sprites.spriteSourceList[i], name))
			return sprites.spriteSourceList[i];
	}

	return NULL;
}