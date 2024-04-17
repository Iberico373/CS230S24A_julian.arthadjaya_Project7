//------------------------------------------------------------------------------
//
// File Name:	SpriteSourceLibrary.h
// Author(s):	Doug Schilling (dschilling)
// Project:		Project 5
// Course:		CS230S24
//
// Copyright © 2024 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
	/* Assume C declarations for C++ */
#endif

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

typedef struct SpriteSource SpriteSource;

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

// An example of the structure to be defined in SpriteSourceLibrary.c.
#if 0
// You are free to change the contents of this structure as long as you do not
//   change the public interface declared in the header.
	typedef struct SpriteSourceLibrary
	{
		// The number of sprite sources currently in the list.
		unsigned int spriteSourceCount;

		// A list of all currently loaded sprite sources.
		// This list can be a fixed-length array (minimum size of 10 entries)
		// or a dynamically-sized array, such as a linked list.
		SpriteSource*	spriteSourceList[spriteSourceListSize];

	} SpriteSourceLibrary;
#endif

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Initialize the SpriteSource Library.
// (NOTE: Make sure to initialize all memory to zero.)
void SpriteSourceLibraryInit();

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
const SpriteSource* SpriteSourceLibraryBuild(const char * spriteSourceName);

// Free all SpriteSource objects in the SpriteSource Library.
// (NOTE: You must call SpriteSourceFree() to free each SpriteSource object.)
// (HINT: The list should contain nothing but NULL pointers once this function is done.)
void SpriteSourceLibraryFreeAll();

//------------------------------------------------------------------------------

#ifdef __cplusplus
}                       /* End of extern "C" { */
#endif

