/*
 * Helper Matrix class with 4x4 matrix
 * provided by ZJ Wood for CSC 473
 * Heavily built on matrix class from Inventor SbMatrix class
 * available from Coin 3D (http://www.coin3d.org/)
 * modified 1/2010
 */
#ifndef _MATRIX_H_
#define _MATRIX_H_

#include <iostream>
#include <cstdio>
#include "Vector.h"
#include <GLFW/glfw3.h>
using namespace std;

/* helper type */
typedef float SbMat[4][4];

/* 4x4 Matrix class with only bare bone functions for ray tracing */
class Matrix {
	public:

		//constructors
		Matrix(); //public constructor
		Matrix(const float a11, const float a12, const float a13, const float a14, 
				const float a21, const float a22, const float a23, const float a24, 
				const float a31, const float a32, const float a33, const float a34, 
				const float a41, const float a42, const float a43, const float a44) ;
		~Matrix(){} //destructor

		//basic functions
		void setValue(const Matrix & m);
		GLfloat* toGLfloat(GLfloat* destination);
		void readGLfloat(GLfloat* other);
		Matrix& multRight(const Matrix &m);
		Matrix& multLeft(const Matrix & m);
		Vector3f multVec(Vector3f input, bool isPoint);
		Vector3f multVecRight(Vector3f input, bool isPoint);
		Matrix multScalar(const float multi);

		//
		Matrix inverse(void) const;
		Matrix transpose(void);
		void makeIdentity(void);
		void makeTranslate(Vector3f trans);
		void makeRotate(Vector3f rotation);
		void makeScale(Vector3f scale);
	
		void printDiagonalMag();
		float diagonalMagnitude();

		//interpolation functions

		float * operator [](int i);
		const float * operator [](int i) const;
	
		const SbMat & getValue(void) const;

		friend ostream& operator<<( ostream&, const Matrix& );

	private:
		SbMat m_Elem;
};

#endif
