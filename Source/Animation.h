//------------------------------------------------------------------------------
//
// File Name:	Animation.h
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

#include "Component.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

typedef class Entity Entity;
typedef class Animation Animation;
typedef class Sprite Sprite;
typedef FILE* Stream;

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

// You are free to change the contents of this structure as long as you do not
//   change the public interface declared in the header.
class Animation : public Component
{
public:
	// Dynamically allocate a new Animation component.
	Animation();
	Animation(const Animation& other);

	~Animation() { };

	// Dynamically allocate a clone of an existing Animation component.
	Component* Clone() const override;

	// Update the animation.
	void Update(float dt) override;

	// Read the properties of an Animation component from a file.
	void Read(Stream stream) override;

	// Play a simple animation sequence [0 .. frameCount - 1].
	void Play(int frameCount, float frameDuration, bool isLooping);

	// Determine if the animation has reached the end of its sequence.
	bool IsDone();

private: 
	void AdvanceFrame();

	// The current frame being displayed.
	unsigned int mFrameIndex;

	// The maximum number of frames in the sequence.
	unsigned int mFrameCount;

	// The time remaining for the current frame.
	float mFrameDelay;

	// The amount of time to display each successive frame.
	float mFrameDuration;

	// True if the animation is running; false if the animation has stopped.
	bool mIsRunning;

	// True if the animation loops back to the beginning.
	bool mIsLooping;

	// True if the end of the animation sequence has been reached, false otherwise.
	// (Hint: This should be true for only one game loop.)
	bool mIsDone;
};

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

/*----------------------------------------------------------------------------*/