/*
 * luz.h
 *
 *  Created on: Oct 30, 2017
 *      Author: Francisco Dominguez
 */
#pragma once
#include <assert.h>
#include <GL/glut.h>

#include "solid.h"
#include "vector3d.h"

class Light:public Solid {
	// here you can see const vs constexpr
	static const unsigned char NLIGHTS=8;
	static constexpr GLenum ids[]={
			GL_LIGHT0,
			GL_LIGHT1,
			GL_LIGHT2,
			GL_LIGHT3,
			GL_LIGHT4,
			GL_LIGHT5,
			GL_LIGHT6,
			GL_LIGHT7};
	static unsigned char idCount;
	unsigned char idLight;
public:
	static void init(){
		idCount=0;
	}
	Light(Vector3D p):Solid(p),idLight(idCount++){
		assert(idCount<=NLIGHTS);
		glEnable(ids[idLight]);
		this->hazFija();
	}
	Light():Light(Vector3D(5,10,5)){}
	void render(){
	    GLfloat lightpos[]={(GLfloat)getPos().getX(),(GLfloat)getPos().getY(),(GLfloat)getPos().getZ(),0.0};
	    glLightfv(ids[idLight],GL_POSITION,lightpos);
	}
};
//Private static initialization
unsigned char Light::idCount=0;
constexpr GLenum Light::ids[];
