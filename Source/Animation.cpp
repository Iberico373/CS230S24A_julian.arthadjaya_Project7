//------------------------------------------------------------------------------
//
// File Name:	Stub.c
// Author(s):	Julian Artha Djaya (julian.arthadjaya)
// Project:		Project 0
// Course:		CS230S24
//
// Copyright © 2024 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Entity.h"
#include "Animation.h"
#include "Sprite.h"

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


// Dynamically allocate a new Animation component.
Animation::Animation()
	: Component(Component::cAnimation)
	, mFrameIndex(0)
	, mFrameCount(0)
	, mFrameDelay(0.0f)
	, mFrameDuration(0.0f)
	, mIsRunning(false)
	, mIsLooping(false)
	, mIsDone(false)
{
}

Animation::Animation(const Animation& other) : Component(other.Type())
{
	*this = other;
}

// Dynamically allocate a clone of an existing Animation component.
Component* Animation::Clone() const
{
	return new Animation(*this);
}

// Update the animation.
void Animation::Update(float dt)
{
	mIsDone = false;

	if (!mIsRunning)
		return;

	mFrameDelay -= dt;

	if (mFrameDelay <= 0.0f)
		AdvanceFrame();
}

// Read the properties of an Animation component from a file.
void Animation::Read(Stream stream)
{
	mFrameIndex =  StreamReadInt(stream);
	mFrameCount = StreamReadInt(stream);

	mFrameDelay = StreamReadFloat(stream);
	mFrameDuration = StreamReadFloat(stream);

	mIsRunning = StreamReadBoolean(stream);
	mIsLooping = StreamReadBoolean(stream);
}

// Play a simple animation sequence [0 .. frameCount - 1].
void Animation::Play(int frameCount, float frameDuration, bool isLooping)
{
	mFrameIndex = 0;
	mFrameCount = frameCount;
	mFrameDelay = 0.0f;
	mFrameDuration = frameDuration;
	mIsRunning = true; 
	mIsLooping = isLooping;
	mIsDone = false;

	Parent()->Has(Sprite)->SetFrame(mFrameIndex);
}

// Determine if the animation has reached the end of its sequence.
bool Animation::IsDone()
{
	return mIsDone;
}


//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

void Animation::AdvanceFrame()
{
	++mFrameIndex;

	if (mFrameIndex >= mFrameCount)
	{
		if (mIsLooping)
		{
			mFrameIndex = 0;
		}
			
		else
		{
			mFrameIndex = mFrameCount - 1;
			mIsRunning = false;
		}

		mIsDone = true;
	}

	if (!mIsRunning)
		mFrameDelay = 0.0f;

	else
	{
		Parent()->Has(Sprite)->SetFrame(mFrameIndex);
		mFrameDelay += mFrameDuration;
	}	
}