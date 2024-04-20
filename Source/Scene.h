//------------------------------------------------------------------------------
//
// File Name:	Scene.h
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

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

typedef class Entity Entity;

//------------------------------------------------------------------------------
// Public Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Typedefs:
//------------------------------------------------------------------------------

typedef void(*SceneFunctionVoidVoid)(void);
typedef void(*SceneFunctionVoidFloat)(float dt);

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

// Structure to store the scene-specific State function pointers.
typedef struct Scene
{
	const char* name;
	SceneFunctionVoidVoid	load;
	SceneFunctionVoidVoid	init;
	SceneFunctionVoidFloat	update;
	SceneFunctionVoidVoid	render;
	SceneFunctionVoidVoid	exit;
	SceneFunctionVoidVoid	unload;
} Scene;

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Verify that a scene is valid (no NULL pointers).
// Params:
//   scene	Pointer to the scene to be checked.
bool SceneIsValid(const Scene* scene);

// Functions for executing the scene-specific State functions.
void SceneLoad(const Scene* scene);
void SceneInit(const Scene* scene);
void SceneUpdate(const Scene* scene, float dt);
void SceneRender(const Scene* scene);
void SceneExit(const Scene* scene);
void SceneUnload(const Scene* scene);

// Restart the active scene.
void SceneRestart(void);

// Add an Entity to the Scene.
// (NOTE: This is done by storing the Entity within an EntityContainer.)
// Params:
//   entity = Pointer to the Entity to be added.
void SceneAddEntity(Entity* entity);

// Find a named Entity within the Scene.
// (Hint: Use the function, EntityContainerFindByName, for this purpose.)
// Params:
//   entityName = The name of the Entity to be returned.
// Returns:
//	 If entityName is valid and the Entity was located successfully,
//		then return a pointer to the Entity,
//		else return NULL.
Entity* SceneFindEntityByName(const char* entityName);

//------------------------------------------------------------------------------