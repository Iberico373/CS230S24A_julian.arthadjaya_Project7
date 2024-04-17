//------------------------------------------------------------------------------
//
// File Name:	Teleporter.h
// Author(s):	Doug Schilling (dschilling)
// Project:		Project 5
// Course:		CS230S24
//
// Copyright � 2024 DigiPen (USA) Corporation.
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

typedef struct Entity Entity;

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// When an Entity goes off-screen, teleport it to the other side.
// Params:
//	 entity = Pointer to the Entity to be checked.
void TeleporterUpdateEntity(Entity* entity);

//------------------------------------------------------------------------------