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
	if(textura!=nullptr) textura->activar();
	glBegin(GL_TRIANGLES);
	    glTexCoord2f(t0.getX(),t0.getY());
	    //glNormal3d(vnx,vny,vnz);
	    glNormal3d(n0.getX(),n0.getY(),n0.getZ());
	    glVertex3d(p0.getX(),p0.getY(),p0.getZ());

	    glTexCoord2f(t1.getX(),t1.getY());
	    //glNormal3d(vnx,vny,vnz);
	    glNormal3d(n1.getX(),n1.getY(),n1.getZ());
	    glVertex3d(p1.getX(),p1.getY(),p1.getZ());

	    glTexCoord2f(t2.getX(),t2.getY());
	    //glNormal3d(vnx,vny,vnz);
	    glNormal3d(n2.getX(),n2.getY(),n2.getZ());
	    glVertex3d(p2.getX(),p2.getY(),p2.getZ());
    glEnd();
    if(textura!=nullptr) textura->desactivar();
}

