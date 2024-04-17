//------------------------------------------------------------------------------
//
// File Name:	MeshLibrary.c
// Author(s):	Julian Artha Djaya (julian.arthadjaya)
// Project:		Project 0
// Course:		CS230S24
//
// Copyright © 2024 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "MeshLibrary.h"
#include "Mesh.h"
#include "Stream.h"

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

// You are free to change the contents of this structure as long as you do not
//   change the public interface declared in the header.
typedef struct MeshLibrary
{
	// This variable is not required but could be used for tracking the number
	//   of Mesh objects currently in the list.
	unsigned int meshCount;

	// A list of all currently loaded meshes.
	// This list can be a fixed-length array (minimum size of 10 entries)
	// or a dynamically-sized array, such as a linked list.
	const Mesh* meshList[10];

} MeshLibrary;

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

static MeshLibrary meshes;

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

static void MeshLibraryAdd(Mesh* mesh);

static Mesh* MeshLibraryFind(const char* name);

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Initialize the Mesh Manager.
// (NOTE: Make sure to initialize all memory to zero.)
void MeshLibraryInit()
{
	meshes.meshCount = 0;
	
	for (int i = 0; i < 10; ++i)
	{
		meshes.meshList[i] = NULL;
	}
}

// Create a mesh and add it to the mesh manager.
// 1: Use sprintf_s() to construct a path name using meshName
//	   (HINT: The correct path name should be constructed using "Data/%s.txt".)
// 2: Call StreamOpen(), passing the pathname
// 3: If the stream was opened successfully,
//	  a: Call MeshCreate() to create an empty Mesh object.
//    b: Call MeshRead() to construct a mesh using data read from the file
//	  c: Call MeshLibraryAdd(), passing the created mesh
//	  d: Close the stream
//	  e: Return the created mesh
// Params:
//	 meshName = The name of the mesh to be created.
// Returns:
//	 If the mesh was created successfully,
//	   then return a pointer to the created mesh,
//	   else return NULL.
const Mesh* MeshLibraryBuild(const char* meshName)
{
	Mesh* mesh = MeshLibraryFind(meshName);

	if (mesh)
		return mesh;

	char filePath[FILENAME_MAX] = "";
	sprintf_s(filePath, _countof(filePath), "./Data/%s.txt", meshName);

	Stream stream = StreamOpen(filePath);

	if (stream)
	{
		mesh = MeshCreate();

		if (mesh)
		{
			MeshRead(mesh, stream);
			MeshLibraryAdd(mesh);
			StreamClose(&stream);
			return mesh;
		}

		StreamClose(&stream);
	}

	return NULL;
}

// Free all Mesh objects in the Mesh Manager.
// (NOTE: You must call MeshFree() to free each Mesh object.)
// (HINT: The list should contain nothing but NULL pointers once this function is done.)
void MeshLibraryFreeAll()
{
	for (unsigned int i = 0; i < meshes.meshCount; ++i)
	{
		MeshFree((Mesh**) & (meshes.meshList[i]));
	}

	meshes.meshCount = 0;
}


//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

static void MeshLibraryAdd(Mesh* mesh)
{
	meshes.meshList[meshes.meshCount] = mesh;
	++meshes.meshCount;
}

static Mesh* MeshLibraryFind(const char* name)
{
	for (unsigned int i = 0; i < meshes.meshCount; ++i)
	{
		if (MeshIsNamed(meshes.meshList[i], name))
		{
			return (Mesh*)meshes.meshList[i];
		}
			
	}

	return NULL;
}