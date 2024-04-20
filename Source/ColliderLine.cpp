//------------------------------------------------------------------------------
//
// File Name:	ColliderLine.c
// Author(s):	Julian Artha Djaya (julian.arthadjaya)
// Project:		Project 0
// Course:		CS230S24
//
// Copyright © 2024 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "ColliderLine.h"
#include "Entity.h"
#include "Collider.h"
#include "Physics.h"
#include "Transform.h"
#include "Vector2D.h"
#include "Stream.h"

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

typedef struct ColliderLineSegment
{
	// A single line segment (P0 and P1).
	Vector2D	point[2];
} ColliderLineSegment;

typedef struct ColliderLine
{
	// Inherit the base collider structure.
	Collider	base;

	// The number of current line segments in the list.
	unsigned int		lineCount;

	// The number of total line segments in the list
	unsigned int		lineSize;

	// The individual line segments.
	// (NOTE: Make sure to allocate enough memory for all line segments!)
	ColliderLineSegment	lineSegments[8];

} ColliderLine;

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

// Dynamically allocate a new (line) collider component.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
// (Hint: Make sure to initialize the ColliderType and memorySize correctly.)
Collider* ColliderLineCreate(void)
{
	ColliderLine* collider = (ColliderLine*)calloc(1, sizeof(ColliderLine));

	if (collider)
	{
		collider->base.type = ColliderTypeLine;
		collider->base.memorySize = sizeof(ColliderLine);
		return (Collider*)collider;
	}

	return NULL;
}

// Read the properties of a ColliderLine component from a file.
// (NOTE: First read an integer indicating the number of lines to be read.)
// (NOTE: For each line, read P0 and P1 using StreamReadVector2D.)
// (HINT: Call ColliderLineAddLineSegment() to add each line.)
// Params:
//	 collider = Pointer to the Collider component.
//	 stream = Pointer to the data stream used for reading.
void ColliderLineRead(Collider* collider, Stream stream)
{
	ColliderLine* colliderLine = (ColliderLine*)collider;

	// Read number of lines
	colliderLine->lineSize = StreamReadInt(stream);

	// Read lines
	for (unsigned int i = 0; i < colliderLine->lineSize; ++i)
	{
		Vector2D p0 = { 0.0f, 0.0f };
		StreamReadVector2D(stream, &p0);

		Vector2D p1 = { 0.0f, 0.0f };
		StreamReadVector2D(stream, &p1);

		ColliderLineAddLineSegment(collider, &p0, &p1);
	}
}

// Add a line segment to the line collider's line segment list.
// Params:
//	 collider = Pointer to the line collider component.
//	 p0 = The line segment's starting position.
//	 p1 = The line segment's ending position.
void ColliderLineAddLineSegment(Collider* collider, const Vector2D* p0, const Vector2D* p1)
{
	ColliderLine* colliderLine = (ColliderLine*)collider;

	if (colliderLine->lineCount >= colliderLine->lineSize)
		return;
	
	colliderLine->lineSegments[colliderLine->lineCount].point[0] = *p0;
	colliderLine->lineSegments[colliderLine->lineCount].point[1] = *p1;
	++colliderLine->lineCount;
}

// Check for collision between a line collider and a circle collider.
// Params:
//	 collider1 = Pointer to the line collider component.
//	 collider2 = Pointer to the circle collider component.
// Returns:
//	 If the pointers are valid,
//	   then return the results of a line-circle collision check,
//	   else return false.
bool ColliderLineIsCollidingWithCircle(const Collider* collider, const Collider* other)
{
	// Point-Line Collision
	// Get components
	ColliderLine* colliderLine = (ColliderLine*)collider;
	Physics* physics = EntityGetPhysics(other->parent);
	Transform* transform = EntityGetTransform(other->parent);

	// Get displacement of circle
	Vector2D startPoint = *PhysicsGetOldTranslation(physics); // bS
	Vector2D endPoint = *transform->GetTranslation();  // bE

	Vector2D displacement = { 0, 0 };  // v
	Vector2DSub(&displacement, &endPoint, &startPoint);

	// Check against every line
	for (unsigned int i = 0; i < colliderLine->lineCount; ++i)
	{
		// Get both points in line
		Vector2D p0 = colliderLine->lineSegments[i].point[0];
		Vector2D p1 = colliderLine->lineSegments[i].point[1];

		// Calculate edge vector
		Vector2D edge = { 0, 0 };
		Vector2DSub(&edge, &p1, &p0);

		// Calculate normal vector
		Vector2D normal = { edge.y, -edge.x };
		Vector2DNormalize(&normal, &normal);

		// Calculate dot products
		float nBs = Vector2DDotProduct(&normal, &startPoint);
		float nBe = Vector2DDotProduct(&normal, &endPoint);
		float nP0 = Vector2DDotProduct(&normal, &p0);
		float nV = Vector2DDotProduct(&normal, &displacement);

		// Check for non-collision
		if (nV == 0)
			continue;

		// Check for non-collision 2 electric bogaloo
		if ((nBs <= nP0) && (nBe < nP0))
			continue;

		// Check for non-collision 3 revenge of the sith
		if ((nBs >= nP0) && (nBe > nP0))
			continue;

		// Calculate time of intersection
		float timeIntersect = (nP0 - nBs) / nV;

		// Calculate point of intersection
		Vector2D pointIntersect = { 0, 0 };
		Vector2DScaleAdd(&pointIntersect, &displacement, &startPoint, timeIntersect);

		// Check for non-collision (agane)
		Vector2D p1P0 = { 0, 0 };
		Vector2DSub(&p1P0, &p1, &p0);

		Vector2D p0P1 = { 0, 0 };
		Vector2DSub(&p0P1, &p0, &p1);

		Vector2D bIP0 = { 0, 0 };
		Vector2DSub(&bIP0, &pointIntersect, &p0);

		Vector2D bIP1 = { 0, 0 };
		Vector2DSub(&bIP1, &pointIntersect, &p1);

		// Check outside p0
		if (Vector2DDotProduct(&p1P0, &bIP0) < 0)
			continue;

		// Check outside p1
		if (Vector2DDotProduct(&p0P1, &bIP1) < 0)
			continue; 

		// Calculate reflection

		// Calculate incident vector, i 
		Vector2D incident = { 0, 0 };
		Vector2DSub(&incident, &endPoint, &pointIntersect);

		// Calculate penetration vector, s
		Vector2D penetration = { 0, 0 };
		Vector2DScale(&penetration, &normal, Vector2DDotProduct(&incident, &normal));

		// Calculate reflection vector, r
		Vector2D reflection = { 0, 0 };
		Vector2DScaleAdd(&reflection, &penetration, &incident, -2.0f);

		// Calculate end point
		Vector2D reflectedPoint = { 0, 0 };
		Vector2DAdd(&reflectedPoint, &pointIntersect, &reflection);

		// Set new translation
		TransformSetTranslation(transform, &reflectedPoint);

		// Calculate new rotation
		float angle = Vector2DToAngleRad(&reflection);
		TransformSetRotation(transform, angle);

		// Calculate new velocity
		Vector2D oldVel = *PhysicsGetVelocity(physics);
		float speed = Vector2DLength(&oldVel);
		Vector2DNormalize(&reflection, &reflection);
		Vector2DScale(&reflection, &reflection, speed);
		PhysicsSetVelocity(physics, &reflection);

		return true;
	}

	return false;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

