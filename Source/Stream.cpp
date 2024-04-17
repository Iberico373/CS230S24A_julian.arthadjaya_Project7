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
#include "Stream.h"
#include "Trace.h"
#include "Vector2D.h"
#include "DGL.h"

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

Stream StreamOpen(const char* filePath)
{
	Stream fileStream;
	errno_t errorNo;

	errorNo = fopen_s(&fileStream, filePath, "rt");

	if (errorNo != 0)
	{
		char errorMsg[100] = {" "};
		strerror_s(errorMsg, _countof(errorMsg), errorNo);

		TraceMessage("Error: StreamOpen could not open file %s; %s", filePath, errorMsg);

		return NULL;
	}

	return fileStream;
}

// Read a single boolean value from a stream.
// (NOTE: Verify that the stream is valid first.)
// (NOTE: Use fscanf_s() to scan the input stream for an integer.)
// Params:
//	 stream = The file stream from which to read.
// Returns:
//	 If the stream was opened succesfully,
//	   then return a boolean value read from the file as an integer,
//	   else return 0.
bool StreamReadBoolean(Stream stream)
{
	if (stream != NULL)
	{
		int result = 0;
		fscanf_s(stream, "%d", &result);
		return result;
	}

	return 0;
}

int StreamReadInt(Stream stream)
{
	if (stream != NULL)
	{
		int result = 0;
		fscanf_s(stream, "%d", &result);
		return result;
	}

	return 0;
}

float StreamReadFloat(Stream stream)
{
	if (stream != NULL)
	{
		float result = 0.0f;
		fscanf_s(stream, "%f", &result);
		return result;
	}

	return 0;
}

void StreamReadVector2D(Stream stream, Vector2D* vector)
{
	if (stream != NULL && vector != NULL)
	{
		vector->x = StreamReadFloat(stream);
		vector->y = StreamReadFloat(stream);
	}
}

// Read the data for a Color from a stream.
// (NOTE: Verify that the stream and color pointers are valid first.)
// (HINT: Use StreamReadFloat() to read the RGBA values, in sequence.)
// Params:
//	 stream = The file stream from which to read.
// Returns:
//	 If the stream and color pointers are both valid,
//	   then read the RGBA components, in sequence,
//	   else fill the RGBA components with 0.
//		 (optionally, write an error message to the trace log).
void StreamReadColor(Stream stream, DGL_Color* color)
{
	if (!stream || !color)
		return;

	*color = (DGL_Color){ StreamReadFloat(stream) , StreamReadFloat(stream) , StreamReadFloat(stream), StreamReadFloat(stream) };
}

// Read a token (a single word) from a file.
// Suggested steps:
//	 - Set the first value in tokenBuffer to 0
//	 - If the stream was opened successfully,
//	   - Read a string ("%s") into tokenBuffer using fscanf_s()
//	 - Return tokenBuffer
// Params:
//	 stream = The file stream from which to read.
// Returns:
//	 Pointer to tokenBuffer
const char* StreamReadToken(Stream stream)
{
	static char tokenBuffer[1024];
	tokenBuffer[0] = 0;

	if (stream)
	{
		fscanf_s(stream, "%s", tokenBuffer, (unsigned int)_countof(tokenBuffer));

		return tokenBuffer;
	}

	return NULL;
}


void StreamClose(Stream* stream)
{
	if (stream != NULL)
	{
		fclose(*stream);
	}
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

