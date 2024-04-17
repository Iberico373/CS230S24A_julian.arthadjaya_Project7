//------------------------------------------------------------------------------
//
// File Name:	EntityContainer.c
// Author(s):	Julian Artha Djaya (julian.arthadjaya)
// Project:		Project 0
// Course:		CS230S24
//
// Copyright © 2024 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "EntityContainer.h"
#include "Entity.h"
#include "Collider.h"
#include "Trace.h"

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

typedef struct Node Node;

typedef struct EntityList EntityList;

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

typedef struct Node
{
	Entity* entity; // data portion
	Node* next; // pointer portion
}Node;

typedef struct EntityList
{
	Node* head; // head pointer portion
	unsigned int size;
}EntityList;

// You are free to change the contents of this structure as long as you do not
//   change the public interface declared in the header.
typedef struct EntityContainer
{
	// This variable is not required but could be used for tracking the number
	//   of Entities currently in the list.  Instructions on how to do this
	//   are included in the function headers.
	unsigned entityCount;

	// This variable is not required but could be used for different purposes.
	// - For storing the maximum size of the container.
	// - For tracking peak usage of the container, used for testing purposes.
	unsigned entityMax;

	// This list can be a fixed-length array (minimum size of 100 entries)
	// or a dynamically sized array, such as a linked list.
	// (NOTE: The implementation details are left up to the student.  However,
	//    it is your responsiblity to ensure that memory is handled correctly.)
	EntityList* entities;

} EntityContainer;

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

static Node* ConstructNode(Entity* data, Node* next);

static void DestructNode(Node** p_node);

static Entity* NodeGetEntity(Node* node);
 
static Node* NodeGetNext(Node* node);

static EntityList* EntityListCreate();

static void EntityListEmpty(EntityList* pList);

static void EntityListFree(EntityList** pList);

static void EntityListPushBack(EntityList* pList, Entity* value);
 
static unsigned int EntityListGetSize(EntityList const* pList);

static Node* EntityListGetHead(EntityList* pList);

static void EntityListDeleteNode(EntityList* pList, Entity* value);

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Dynamically allocate a new EntityContainer.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
// (Hint: If your implementation requires any variables to be initialized to
//    non-zero values, then do so here.)
// Returns:
//	 If the memory allocation was successful,
//	   then return a pointer to the allocated memory,
//	   else return NULL.
EntityContainer* EntityContainerCreate()
{
	EntityContainer* entityContainer = (EntityContainer*)calloc(1, sizeof(EntityContainer));

	if (entityContainer)
	{
		entityContainer->entities = EntityListCreate();
		entityContainer->entityMax = 100000;

		return entityContainer;
	}

	return NULL;
}

// Free the memory associated with an EntityContainer.
// (NOTE: If necessary, call EntityContainerFreeAll to free any existing Entities.)
// (NOTE: The EntityContainer pointer must be set to NULL.)
// Params:
//	 entities = Pointer to the EntityContainer pointer.
void EntityContainerFree(EntityContainer** entities)
{
	if ((*entities)->entityCount > 0)
		EntityContainerFreeAll(*entities);

	EntityListFree(&((*entities)->entities));
	free(*entities);
	*entities = NULL;
}

// Add an Entity to the EntityContainer.
// (NOTE: If the container is not full, then the Entity should be added to the list
//    and the entityCount incremented by 1.)
// Params:
//   entities = Pointer to the EntityContainer.
// Returns:
//	 If the EntityContainer pointer is valid and the Entity was added successfully,
//		then return true,
//		else return false.
bool EntityContainerAddEntity(EntityContainer* entities, Entity* entity)
{
	if (!entities || !entity)
		return false;

	if (EntityListGetSize(entities->entities) < entities->entityMax)
	{
		EntityListPushBack(entities->entities, entity);
		++entities->entityCount;

		return true;
	}

	return false;
}

// Find an Entity in the EntityContainer that has a matching name.
// (HINT: Use the new function, EntityIsNamed, to compare names.)
// Params:
//   entities = Pointer to the EntityContainer.
//   entityName = The name of the Entity to be returned.
// Returns:
//	 If the EntityContainer pointer is valid and the Entity was located successfully,
//		then return a pointer to the Entity,
//		else return NULL.
Entity* EntityContainerFindByName(const EntityContainer* entities, const char* entityName)
{
	Node* node = EntityListGetHead(entities->entities);

	while (node)
	{
		Entity* entity = NodeGetEntity(node);

		if (strcmp(EntityGetName(entity), entityName) == 0)
			return entity;

		node = NodeGetNext(node);
	}

	return NULL;
}

// Determines if the EntityContainer is empty (no Entities exist).
// Params:
//   entities = Pointer to the EntityContainer.
// Returns:
//	 If the EntityContainer pointer is valid and no Entities exist,
//		then return true,
//		else return false.
bool EntityContainerIsEmpty(const EntityContainer* entities)
{
	if (!entities)
		return false;

	if (entities->entityCount == 0)
		return true;

	return false;
}
// Update all Entities in the EntityContainer.
// (HINT: You must call EntityUpdate for all Entities.)
// (NOTE: After an Entity has been updated, if it has been flagged as
//    destroyed, then it must be removed from the list and freed properly.
//    Additionally, the entityCount should be decremented by 1.)
// Params:
//   entities = Pointer to the EntityContainer.
//	 dt = Change in time (in seconds) since the last game loop.
void EntityContainerUpdateAll(EntityContainer* entities, float dt)
{
	Node* node = EntityListGetHead(entities->entities);

	while (node)
	{
		Entity* entity = NodeGetEntity(node);
		node = NodeGetNext(node);

		EntityUpdate(entity, dt);

		if (EntityIsDestroyed(entity))
		{
			EntityListDeleteNode(entities->entities, entity);
			--entities->entityCount;
		}		
	}
}

// Check for collisions between all Entities in the EntityContainer.
// (HINT: Only check for collisions between Entities that contain a Collider.)
// (NOTE: There must never be more than 1 collision check between any two
//    Entities in the container.  Additionally, each Entity must never
//    check for a collision with itself.  To ensure this is the case, each time
//    an Entity with a Collider is found, check for collisions against only the
//    remaining Entities in the list (starting with the Entity following the
//    current Entity.)
// Params:
//   entities = Pointer to the EntityContainer.
void EntityContainerCheckCollisions(EntityContainer* entities)
{
	Node* node = EntityListGetHead(entities->entities);

	while (node)
	{
		Entity* entity = NodeGetEntity(node);
		Collider* collider = EntityGetCollider(entity);
		
		if (collider)
		{
			Node* otherNode = NodeGetNext(node);

			while (otherNode)
			{
				Entity* other = NodeGetEntity(otherNode);
				Collider* otherCollider = EntityGetCollider(other);

				if (otherCollider)
					ColliderCheck(collider, otherCollider);

				otherNode = NodeGetNext(otherNode);
			}
		}

		node = NodeGetNext(node);
	}
}

// Render all Entities in the EntityContainer.
// (HINT: You must call EntityRender for all Entities.)
// Params:
//   entities = Pointer to the EntityContainer.
void EntityContainerRenderAll(const EntityContainer* entities)
{
	Node* node = EntityListGetHead(entities->entities);

	while (node)
	{
		Entity* entity = NodeGetEntity(node);
		EntityRender(entity);

		node = NodeGetNext(node);
	}
}

// Free all Entities in the EntityContainer.
// (NOTE: You must call EntityFree for all Entities.)
// (HINT: The container must be completely empty after this function is called.)
// (HINT: The entityCount should be 0 after all of the Entities have been freed.)
// Params:
//   entities = Pointer to the EntityContainer.
void EntityContainerFreeAll(EntityContainer* entities)
{
	EntityListEmpty(entities->entities);
	entities->entityCount = 0; 
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

// Node Functions
static Entity* NodeGetEntity(Node* node)
{
	return node->entity;
}

static Node* NodeGetNext(Node* node)
{
	return node->next;
}

// Linked List Functions
// create a new empty List strcuture 
static EntityList* EntityListCreate()
{
	EntityList* ret_val = (EntityList*)calloc(1, sizeof(EntityList));     // Create list object

	if (!ret_val)
		return NULL;

	return ret_val;                                          // Return constructed object
}

// destroy a new empty List strcuture 
static void EntityListEmpty(EntityList* pList)
{
	Node* temp = pList->head;               // get the head from the List object

	while (temp)                            // while there are nodes
	{
		Node* next = temp->next;            // get next
		DestructNode(&temp);                // delete current
		temp = next;                        // move current forward
	}

	pList->head = NULL;
	pList->size = 0;
}

// free list
static void EntityListFree(EntityList** pList)
{
	if ((*pList)->head)
		EntityListEmpty(*pList);

	free(*pList);
	*pList = NULL;
}

// Adds a node to the end of the list
static void EntityListPushBack(EntityList* pList, Entity* value)
{
	// Set head to new node if head is NULL
	if (!pList->head)
	{
		pList->head = ConstructNode(value, NULL);
		++pList->size;
		return;
	}

	// Get head of list
	Node* temp = pList->head;

	// Iterate through list until the end of list
	while (temp)
	{
		if (!(temp->next))
		{
			// Set tail of list to new node
			temp->next = ConstructNode(value, NULL);
			++pList->size;
			break;
		}

		temp = temp->next;
	}
}

// Returns the number of nodes in the list
// we promise not to modify the list
static unsigned int EntityListGetSize(EntityList const* pList)
{
	return pList->size;
}

// allow access to the first element in the list
// if list is empty - throw an exception
static Node* EntityListGetHead(EntityList* pList)
{
	if (!pList->head) {
		TraceMessage("EntityList get_front: cannot access non-existing element");
		return NULL;
	}
	return pList->head;
}

// Delete node with specified value
static void EntityListDeleteNode(EntityList* pList, Entity* value)
{
	if (!(pList->head))
		return;

	Node* pHead = pList->head;

	while (pHead)
	{
		if (!(pHead->next))
		{
			if (pHead->entity == value)
			{
				DestructNode(&pHead);
				--pList->size;
			}

			break;
		}

		else if (pHead->next->entity == value)
		{
			if (!pHead->next->next)
			{
				Node* temp = NULL;
				DestructNode(&(pHead->next));
				pHead->next = temp;

				--pList->size;
			}

			else
			{
				Node* temp = pHead->next->next;
				DestructNode(&(pHead->next));
				pHead->next = temp;

				--pList->size;
			}

			value = NULL;
			break;
		}

		pHead = pHead->next;
	}
}

static Node* ConstructNode(Entity* data, Node* next)
{
	Node* pNode = (Node*)calloc(1, sizeof(Node));    // Allocate memory for node

	if (!pNode)
		return NULL;

	pNode->entity = data;                     // Set data field
	pNode->next = next;                       // Set next

	return pNode;
}

static void DestructNode(Node** p_node)
{
	EntityFree(&((*p_node)->entity));
	free(*p_node);
	*p_node = NULL;
}