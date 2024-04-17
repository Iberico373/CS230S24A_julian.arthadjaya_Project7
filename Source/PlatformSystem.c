//------------------------------------------------------------------------------
//
// File Name:	PlatformSystem.c
// Author(s):	Doug Schilling (dschilling)
// Project:		Project 5
// Course:		CS230S24
//
// Copyright © 2024 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include <Windows.h>
#include <assert.h>

#include "DGL.h"

#include "BaseSystem.h"
#include "PlatformSystem.h"
#include "SceneSystem.h"
#include "Trace.h"

//------------------------------------------------------------------------------
// Libraries
//------------------------------------------------------------------------------

#ifdef _DEBUG
#pragma comment (lib, "DGL_d.lib")
#else
#pragma comment (lib, "DGL.lib")
#endif

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

static const DGL_Color DGL_Color_Black = { 0.0f, 0.0f, 0.0f, 1.0f };
static const DGL_Color DGL_Color_Grey = { 0.5f, 0.5f, 0.5f, 1.0f };
static const DGL_Color DGL_Color_White = { 1.0f, 1.0f, 1.0f, 1.0f };
static const DGL_Color DGL_Color_Red = { 1.0f, 0.0f, 0.0f, 1.0f };
static const DGL_Color DGL_Color_Green = { 0.0f, 1.0f, 0.0f, 1.0f };
static const DGL_Color DGL_Color_Blue = { 0.0f, 0.0f, 1.0f, 1.0f };

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

typedef struct PlatformSystem
{
	// WARNING: The base class must always be included first.
	BaseSystem	base;

	// Add any system-specific variables second.
	HINSTANCE	mAppInstance;
	bool		mCreateWindow;

} PlatformSystem;

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//  System-specific State functions:
static bool PlatformSystemInit(void);
static void PlatformSystemUpdate(float dt);
static void PlatformSystemRender(void);
static void PlatformSystemExit(void);

// Application-specific callback function for handling Windows messages.
static LRESULT CALLBACK PlatformSystemCallback(_In_ HWND hWnd, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam);

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

static PlatformSystem instance =
{
	{ "PlatformSystem", PlatformSystemInit, PlatformSystemUpdate, PlatformSystemRender, PlatformSystemExit },
	0
};

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Get the instance of the Platform System (Windows and DGL).
// Also stores the Windows settings passed in WinMain.
// Params:
//   hInstance	The handle for the Windows instance.
//   show		A boolean value: if true, then show the Window.
BaseSystem* PlatformSystemGetInstance(HINSTANCE hInstance, int show)
{
	// Initialize any system-specific variables here:
	instance.mAppInstance = hInstance;
	instance.mCreateWindow = show;

	// Initialize the DigiPen Graphics Library.
	DGL_SysInitInfo initInfo;
	initInfo.mAppInstance = instance.mAppInstance;
	initInfo.mShow = instance.mCreateWindow;
	initInfo.mWindowTitle = "CS230 Project 6 - Omega";
	initInfo.mWindowWidth = 1024;
	initInfo.mWindowHeight = 768;
	initInfo.mMaxFrameRate = 60;
	initInfo.mClassStyle = CS_HREDRAW | CS_VREDRAW;
	initInfo.mWindowStyle = WS_OVERLAPPEDWINDOW;
#ifdef _DEBUG
	initInfo.mCreateConsole = true;
#else
	initInfo.mCreateConsole = false;
#endif
	initInfo.pWindowsCallback = PlatformSystemCallback;
	initInfo.mWindowIcon = 0; // IDI_DGLTEMPLATE;

	HWND windowHandle = DGL_System_Init(&initInfo);
	if (windowHandle == NULL)
	{
		TraceMessage("PlatformSystemInit Error: The DigiPen Graphics Library could not be initialized!");
		return NULL;
	}

#ifdef _DEBUG
	// Write a message to the console window.
	printf("Starting %s\n", initInfo.mWindowTitle);
#endif

	// Return a reference to the instance of this system.
	return (BaseSystem*) &instance;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

// Initialize the System (Windows, Event Handlers).
static bool PlatformSystemInit(void)
{
	DGL_Graphics_SetBackgroundColor(&DGL_Color_Green);

	return true;
}

// Update the System (Windows, Event Handlers).
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void PlatformSystemUpdate(float dt)
{
	// Tell the compiler that the 'dt' variable is unused.
	UNREFERENCED_PARAMETER(dt);

	// Wait for the next frame.
	DGL_System_FrameControl();

	// Update the DGL system (Input, etc.)
	DGL_System_Update();

	// check if forcing the application to quit
	if (DGL_Input_KeyTriggered(VK_ESCAPE) || !DGL_System_DoesWindowExist())
	{
		SceneSystemSetNext(NULL);
	}
}

// Complete the draw process for the current game loop.
void PlatformSystemRender(void)
{
}

// Shutdown the System (Windows, Event Handlers).
void PlatformSystemExit(void)
{
	//// Shutdown the DigiPen Graphics Library.
	DGL_System_Exit();
}

static LRESULT CALLBACK PlatformSystemCallback(_In_ HWND hWnd, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	int result;
	if (DGL_System_HandleWindowsMessage(message, wParam, lParam, &result))
		return result;

	switch (message)
	{
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}
}

