//------------------------------------------------------------------------------
//
// File Name:	SandboxScene.c
// Author(s):	Doug Schilling (dschilling)
// Project:		Project 0
// Course:		CS230S24
//
// Copyright © 2024 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Scene.h"
#include "SceneSystem.h"
#include "SandboxScene.h"
#include "Stream.h"
#include "Trace.h"
#include "Vector2D.h"

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

typedef struct SandboxScene
{
	// WARNING: The base class must always be included first.
	Scene	base;

	// Add any scene-specific variables second.
} SandboxScene;

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

static void SandboxSceneTraceFloat(const char* text, float f);
static void SandboxSceneTraceVector(const char* text, const Vector2D* v);
static void SandboxSceneLoad(void);
static void SandboxSceneInit(void);
static void SandboxSceneUpdate(float dt);
static void SandboxSceneExit(void);
static void SandboxSceneUnload(void);
static void SandboxSceneRender(void);

//------------------------------------------------------------------------------
// Instance Variable:
//------------------------------------------------------------------------------

static SandboxScene instance =
{
	// Initialize the base structure:
	{ "Sandbox", SandboxSceneLoad, SandboxSceneInit, SandboxSceneUpdate, SandboxSceneRender, SandboxSceneExit, SandboxSceneUnload },

	// Initialize any scene-specific variables:
};

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

const Scene* SandboxSceneGetInstance(void)
{
	return &(instance.base);
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

// Write float values to trace file
static void SandboxSceneTraceFloat(const char* text, float f)
{
	TraceMessage("Vector Test: % s = %f", text, f);
}

// Write vector2D values to trace file
static void SandboxSceneTraceVector(const char* text, const Vector2D* v)
{
	TraceMessage("Vector Test: % s = [%f, %f]", text, v->x, v->y);
}

// Load any resources used by the scene.
static void SandboxSceneLoad(void)
{

}

// Initialize the variables used by the scene.
static void SandboxSceneInit()
{

}

// Update the the variables used by the scene and render objects (temporary).
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
static void SandboxSceneUpdate(float dt)
{
	// Tell the compiler that the 'dt' variable is unused.
	UNREFERENCED_PARAMETER(dt);

	Stream file = StreamOpen("./Data/VectorTests.txt");

	if (file)
	{
		Vector2D result;

		Vector2DZero(&result);
		SandboxSceneTraceVector("Vector2DZero", &result);

		Vector2DSet(&result, 1.5f, 1.0f);
		SandboxSceneTraceVector("Vector2DSet", &result);

		Vector2DNeg(&result, &result);
		SandboxSceneTraceVector("Vector2DNeg", &result);

		Vector2DAdd(&result, &result, &result);
		SandboxSceneTraceVector("Vector2DAdd", &result);

		Vector2DSub(&result, &result, &result);
		SandboxSceneTraceVector("Vector2DSub", &result);

		StreamReadVector2D(file, &result);
		SandboxSceneTraceVector("StreamReadVector2D", &result);

		Vector2DNormalize(&result, &result);
		SandboxSceneTraceVector("Vector2DNormalize", &result);

		float scale = StreamReadFloat(file);
		SandboxSceneTraceFloat("StreamReadFloat", scale);

		Vector2DScale(&result, &result, scale);
		SandboxSceneTraceVector("Vector2DScale", &result);

		Vector2DScaleAdd(&result, &result, &result, scale);
		SandboxSceneTraceVector("Vector2DScaleAdd", &result);

		Vector2DScaleSub(&result, &result, &result, scale);
		SandboxSceneTraceVector("Vector2DScaleSub", &result);

		SandboxSceneTraceFloat("Vector2DLength", Vector2DLength(&result));

		SandboxSceneTraceFloat("Vector2DSquareLength", Vector2DSquareLength(&result));

		Vector2D vec0;
		Vector2D vec1;

		StreamReadVector2D(file, &vec0);
		SandboxSceneTraceVector("StreamReadVector2D", &vec0);

		StreamReadVector2D(file, &vec1);
		SandboxSceneTraceVector("StreamReadVector2D", &vec1);

		SandboxSceneTraceFloat("Vector2DDistance", Vector2DDistance(&vec0, &vec1));

		SandboxSceneTraceFloat("Vector2DSquareDistance", Vector2DSquareDistance(&vec0, &vec1));

		SandboxSceneTraceFloat("Vector2DDotProduct", Vector2DDotProduct(&vec0, &vec1));

		float angle = StreamReadFloat(file);
		SandboxSceneTraceFloat("StreamReadFloat", angle);

		Vector2DFromAngleDeg(&result, angle);
		SandboxSceneTraceVector("Vector2DFromAngleDeg", &result);

		angle = StreamReadFloat(file);
		SandboxSceneTraceFloat("StreamReadFloat", angle);

		Vector2DFromAngleRad(&result, angle);
		SandboxSceneTraceVector("Vector2DFromAngleRad", &result);

		SandboxSceneTraceFloat("Vector2DToAngleRad", Vector2DToAngleRad(&result));
	}

	StreamClose(&file);

	SceneSystemSetNext(NULL);
}

// Render the scene.
void SandboxSceneRender(void)
{
}

// Exit the scene.
static void SandboxSceneExit()
{
}

// Unload any resources used by the scene.
static void SandboxSceneUnload(void)
{
}

