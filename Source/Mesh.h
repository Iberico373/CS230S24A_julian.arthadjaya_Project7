//------------------------------------------------------------------------------
//
// File Name:	Mesh.h
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

#include "DGL.h"
#include "Component.h"
#include "Stream.h"
#include <assert.h>

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

class Mesh : public Component
{
public:
	Mesh();
	Mesh(const Mesh& other);

	~Mesh();

private:
	// The name of the Mesh.  This will be used later in the semester.
	char name[32];

	// Pointer to the DGL_Mesh resource.
	const DGL_Mesh* meshResource;

	// The draw mode to use when rendering the mesh (Usually "DGL_DM_TRIANGLELIST").
	DGL_DrawMode drawMode;
};

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Dynamically allocate a new Mesh object but leave it empty.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
// Returns:
//	 If the mesh was created successfully,
//	   then return a pointer to the created Mesh,
//	   else return NULL.
Mesh* MeshCreate();

// Dynamically allocate a new Mesh object and create a quadrilateral mesh.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
// (Hint: The Mesh name can be stored using strcpy_s(). For example:
//    strcpy_s(mesh->name, _countof(mesh->name), name); )
// (NOTE: The drawMode should be set to DGL_DM_TRIANGLELIST.)
// Params:
//   mesh = Pointer to an existing, empty Mesh object.
//	 xHalfSize = The X half-size of the mesh.
//	 yHalfSize = The Y half-size of the mesh.
//   uSize = The U size of the mesh, relative to texture coordinates (0.0 .. 1.0).
//   vSize = The V size of the mesh, relative to texture coordinates (0.0 .. 1.0).
//	 name = A name for the mesh.
void MeshBuildQuad(Mesh* mesh, float xHalfSize, float yHalfSize, float uSize, float vSize, const char * name);

// Read the properties of a Mesh object from a file.
// (NOTE: First, read a token from the file and verify that it is "Mesh".)
// (NOTE: Second, read a token and store it in the Mesh's name variable.)
// (NOTE: Third, read an integer indicating the number of vertices to be read.)
// (NOTE: For each vertex, read a Vector2D (position), a DGL_Color (color), and a Vector2D (UV).)
// (HINT: Call DGL_Graphics_AddVertex() to add a single vertex to the mesh.)
// Params:
//   mesh = Pointer to the Mesh.
//	 stream = The data stream used for reading.
void MeshRead(Mesh* mesh, Stream stream);

// Determines if a Mesh has the specified name.
// (HINT: This function is similar to one in Entity.c.)
// Params:
//	 mesh = Pointer to the Mesh object.
//	 name = Pointer to the name to be compared.
// Returns:
//	 If the mesh and name pointers are valid,
//		then perform a string comparison and return the result (match = true),
//		else return false.
bool MeshIsNamed(const Mesh* mesh, const char* name);

// Render a mesh.
// (NOTE: This is done using DGL_Graphics_DrawMesh().)
// Params:
//   mesh = Pointer to a Mesh to be rendered.
void MeshRender(const Mesh* mesh);

// Free the memory associated with a mesh.
// (NOTE: The DGL_Mesh object must be freed using DGL_Graphics_FreeMesh().)
// (NOTE: The Mesh object must be freed using free().
// (NOTE: The Mesh pointer must be set to NULL.)
// Params:
//   mesh = Pointer to the Mesh pointer.
void MeshFree(Mesh** mesh);

//------------------------------------------------------------------------------