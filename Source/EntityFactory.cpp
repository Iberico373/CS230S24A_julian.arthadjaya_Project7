//------------------------------------------------------------------------------
//
// File Name:	EntityFactory.c
// Author(s):	Julian Artha Djaya (julian.arthadjaya)
// Project:		Project 0
// Course:		CS230S24
//
// Copyright © 2024 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "EntityFactory.h"
#include "Entity.h"
#include "Stream.h"
#include "EntityContainer.h"

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

static EntityContainer* archetypes = NULL;

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Build a single instance of the specified game object.
// Params:
//	 filename = The name of the file to be deserialized.
// Returns:
//	 If the filename is valid
//	   then return a pointer to a new instance of the specified game object,
//	   else NULL.
Entity* EntityFactoryBuild(const char* entityName)
{
	if (!entityName)
		return NULL;

	if (!archetypes)
		archetypes = EntityContainerCreate();

	Entity* archetype = EntityContainerFindByName(archetypes, entityName);

	if (archetype)
		return EntityClone(archetype);

	char pathName[FILENAME_MAX] = "";
	sprintf_s(pathName, _countof(pathName), "./Data/%s.txt", entityName);

	Stream stream = StreamOpen(pathName);

	if (stream)
	{
		const char* token = StreamReadToken(stream);

		if (strncmp(token, "Entity", _countof("Entity")) == 0)
		{
			Entity* entity = EntityCreate();
			EntityRead(entity, stream);

			if (EntityContainerAddEntity(archetypes, entity))
			{
				StreamClose(&stream);
				return EntityClone(entity);
			}
		}

		StreamClose(&stream);
	}

	return NULL;
}

// Free all archetype Entities.
// (Hint: If the "archetypes" container exists, then the EntityContainerFreeAll
//    function must be called.)
void EntityFactoryFreeAll()
{
	if (archetypes)
		EntityContainerFreeAll(archetypes);
}
//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

