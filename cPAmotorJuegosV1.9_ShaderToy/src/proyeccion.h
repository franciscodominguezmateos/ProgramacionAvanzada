/*
 * proyeccion.h
 *
 *  Created on: Oct 31, 2017
 *      Author: Francisco Dominguez
 */
#pragma once
#include <vector>
#include <GL/glut.h>
#include "util.h"

using namespace std;

// FROM: http://www.songho.ca/opengl/gl_matrix.html
///////////////////////////////////////////////////////////////////////////////
// equivalent to glFrustum()
// PARAMS: (left, right, bottom, top, near, far)
///////////////////////////////////////////////////////////////////////////////
vector<GLfloat> setFrustum(float l, float r, float b, float t, float n, float f)
{
	vector<GLfloat> mat(16);
    mat[0]  = 2 * n / (r - l);
    mat[5]  = 2 * n / (t - b);
    mat[8]  = (r + l) / (r - l);
    mat[9]  = (t + b) / (t - b);
    mat[10] = -(f + n) / (f - n);
    mat[11] = -1;
    mat[14] = -(2 * f * n) / (f - n);
    mat[15] = 0;
    return mat;
}

///////////////////////////////////////////////////////////////////////////////
// equivalent to gluPerspective()
// PARAMS: (vertical FOV, aspect ratio = width/height, near, far)
///////////////////////////////////////////////////////////////////////////////
vector<GLfloat> getPerspectiveGLMatrix(float fovY, float aspect, float front, float back)
{
    float tangent = tanf(fovY/2 * DEG2RAD); // tangent of half fovY
    float height = front * tangent;         // half height of near plane
    float width  = height * aspect;          // half width of near plane

    // params: left, right, bottom, top, near, far
    return setFrustum(-width, width, -height, height, front, back);
}
///////////////////////////////////////////////////////////////////////////////
// equivalent to glOrtho()
// PARAMS: (left, right, bottom, top, near, far)
///////////////////////////////////////////////////////////////////////////////
vector<GLfloat> setOrthoFrustum(float l, float r, float b, float t, float n, float f)
{
	vector<GLfloat> mat(16);
    mat[0]  = 2 / (r - l);
    mat[5]  = 2 / (t - b);
    mat[10] = -2 / (f - n);
    mat[12] = -(r + l) / (r - l);
    mat[13] = -(t + b) / (t - b);
    mat[14] = -(f + n) / (f - n);
    return mat;
}
class Proyeccion {
public:
	Proyeccion(){}
	virtual ~Proyeccion(){}
	virtual void render()=0;
	virtual Mat getMat()=0;
};
