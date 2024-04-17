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

#define _USE_MATH_DEFINES

#include "Matrix2D.h"
#include "Vector2D.h"
#include "math.h"
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

static float Matrix2DDotProduct(const Matrix2D* pMtx0, const Matrix2D* pMtx1, int row, int col);

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// This function sets the matrix equal to the identity matrix.
void Matrix2DIdentity(Matrix2D* pResult)
{
	for (int row = 0; row < 4; ++row)
	{
		for (int col = 0; col < 4; ++col)
		{
			if (row == col)
				Matrix2DRowCol(pResult, row, col) = 1.0f;

			else
				Matrix2DRowCol(pResult, row, col) = 0.0f;
		}
	}
}

// This function calculates the transpose matrix of Mtx and saves it in the result matrix.
// (NOTE: Care must be taken when pResult = pMtx.)
void Matrix2DTranspose(Matrix2D* pResult, const Matrix2D* pMtx)
{
	Matrix2D temp = { 0.0f };

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			Matrix2DRowCol(&temp, i, j) = Matrix2DRowCol(pMtx, j, i);
		}
	}

	*pResult = temp;
}

// This function multiplies Mtx0 with Mtx1 and saves the result in Result.
// Result = Mtx0*Mtx1
// (NOTE: Care must be taken when pResult = either pMtx0 or pMtx1.)
void Matrix2DConcat(Matrix2D* pResult, const Matrix2D* pMtx0, const Matrix2D* pMtx1)
{
	Matrix2D temp = { 0.0f };

	for (int row = 0; row < 4; ++row)
	{
		for (int col = 0; col < 4; ++col)
		{
			Matrix2DRowCol(&temp, row, col) = Matrix2DDotProduct(pMtx0, pMtx1, row, col);
		}
	}

	*pResult = temp;
}

// This function creates a translation matrix from x & y and saves it in Result.
// (Hint: This function must initialize all matrix values.)
void Matrix2DTranslate(Matrix2D* pResult, float x, float y)
{
	Matrix2DIdentity(pResult);
	Matrix2DRowCol(pResult, 0, 3) = x;
	Matrix2DRowCol(pResult, 1, 3) = y;
}

// This function creates a scaling matrix from x & y and saves it in Result.
// (Hint: This function must initialize all matrix values.)
void Matrix2DScale(Matrix2D* pResult, float x, float y)
{
	Matrix2DIdentity(pResult);
	Matrix2DRowCol(pResult, 0, 0) = x;
	Matrix2DRowCol(pResult, 1, 1) = y;
}

// This matrix creates a rotation matrix from "Angle" whose value is in degrees.
// (Hint: This function must initialize all matrix values.)
// Converting from degrees to radians can be performed as follows:
//	 radians = (angle * M_PI) / 180.0f
// M_PI is defined in "math.h", which may be included as follows:
//   #define _USE_MATH_DEFINES
//   #include <math.h>
void Matrix2DRotDeg(Matrix2D* pResult, float angle)
{
	angle = (angle * (float)M_PI) / 180.0f;

	Matrix2DIdentity(pResult);
	Matrix2DRowCol(pResult, 0, 0) = cosf(angle);
	Matrix2DRowCol(pResult, 0, 1) = -sinf(angle);
	Matrix2DRowCol(pResult, 1, 0) = sinf(angle);																
	Matrix2DRowCol(pResult, 1, 1) = cosf(angle);
}

// This matrix creates a rotation matrix from "Angle" whose value is in radians.
// (Hint: This function must initialize all matrix values.)
void Matrix2DRotRad(Matrix2D* pResult, float angle)
{
	Matrix2DIdentity(pResult);
	Matrix2DRowCol(pResult, 0, 0) = cosf(angle);
	Matrix2DRowCol(pResult, 0, 1) = -sinf(angle);
	Matrix2DRowCol(pResult, 1, 0) = sinf(angle);
	Matrix2DRowCol(pResult, 1, 1) = cosf(angle);
}

// This function multiplies the matrix Mtx with the vector Vec and saves the result in Result.
// Result = Mtx * Vec.
// (NOTE: Care must be taken when pResult = pVec.)
// (NOTE: Remember to treat the implied w component as 1.)
void Matrix2DMultVec(Vector2D* pResult, const Matrix2D* pMtx, const Vector2D* pVec)
{
	Vector2D temp = { 0.0f, 0.0f };

	temp.x = Matrix2DRowCol(pMtx, 0, 0) * pVec->x + Matrix2DRowCol(pMtx, 0, 1) * pVec->y + Matrix2DRowCol(pMtx, 0, 3);
	temp.y = Matrix2DRowCol(pMtx, 1, 0) * pVec->x + Matrix2DRowCol(pMtx, 1, 1) * pVec->y + Matrix2DRowCol(pMtx, 1, 3);

	*pResult = temp;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

static float Matrix2DDotProduct(const Matrix2D* pMtx0, const Matrix2D* pMtx1, int row, int col)
{
	float result = 0.0f;

	for (int i = 0; i < 4; ++i)
	{
		result += (Matrix2DRowCol(pMtx0, row, i) * Matrix2DRowCol(pMtx1, i, col));
	}

	return result;
}