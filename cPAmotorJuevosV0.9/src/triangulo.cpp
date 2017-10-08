/*
 * triangulo.cpp
 *
 *  Created on: Nov 12, 2015
 *      Author: francisco
 */

#include "triangulo.h"

Triangulo::~Triangulo() {
	// TODO Auto-generated destructor stub
}
void Triangulo::render(){
	Vector3D c=this->getCol();
	glColor3f(c.getX(),c.getY(),c.getZ());
	Vector3D vn=this->getNormal();
	float vnx=vn.getX();
	float vny=vn.getY();
	float vnz=vn.getZ();
	glBegin(GL_TRIANGLES);
	    glNormal3f(vnx,vny,vnz);
	    glVertex3f(p0.getX(),p0.getY(),p0.getZ());
	    glNormal3f(vnx,vny,vnz);
	    glVertex3f(p1.getX(),p1.getY(),p1.getZ());
	    glNormal3f(vnx,vny,vnz);
	    glVertex3f(p2.getX(),p2.getY(),p2.getZ());
    glEnd();
}

