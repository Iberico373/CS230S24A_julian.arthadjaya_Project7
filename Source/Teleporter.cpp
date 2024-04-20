//------------------------------------------------------------------------------
//
// File Name:	Teleporter.c
// Author(s):	Julian Artha Djaya (julian.arthadjaya)
// Project:		Project 0
// Course:		CS230S24
//
// Copyright © 2024 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Teleporter.h"
#include "Entity.h"
#include "DGL.h"
#include "Vector2D.h"
#include "Transform.h"
#include "Physics.h"

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

// When an Entity goes off-screen, teleport it to the other side.
// Params:
//	 entity = Pointer to the Entity to be checked.
void TeleporterUpdateEntity(Entity* entity)
{
	// Get halfsize of window
	Vector2D windowSize = DGL_Window_GetSize();
	Vector2DScale(&windowSize, &windowSize, 0.5f);

	// Get entity velocity and translation
	Physics* phys = entity->Has(Physics);
	Vector2D vel = *(phys->GetVelocity());

	Transform* trans = entity->Has(Transform);
	Vector2D pos = *(trans->GetTranslation());

	// Check collision with edge of screen
	if ((vel.x > 0) && (pos.x > windowSize.x))
		pos.x = -windowSize.x;	// Teleport to left side

	if ((vel.x < 0) && (pos.x < -windowSize.x))
		pos.x = windowSize.x;	// Teleport to right side

	if ((vel.y > 0) && (pos.y > windowSize.y))
		pos.y = -windowSize.y;	// Teleport to bottom side 

	if ((vel.y < 0) && (pos.y < -windowSize.y))
		pos.y = windowSize.y;	// Teleport to top side

	trans->SetTranslation(&pos);
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

