/*
 * rectangulo.cpp
 *
 *  Created on: Oct 18, 2017
 *      Author: francisco
 */

#include "rectangulo.h"

Rectangulo::Rectangulo(Vector3D p0,Vector3D p1,Vector3D p2,Vector3D p3):Triangulo(p0,p1,p2),p3(p3) {

}

Rectangulo::~Rectangulo() {
}
void Rectangulo::render(){
	Triangulo::render();
	Vector3D c=this->getCol();
	glColor3f(c.getX(),c.getY(),c.getZ());
	Vector3D vn=this->getNormal();
	float vnx=vn.getX();
	float vny=vn.getY();
	float vnz=vn.getZ();
	glBegin(GL_TRIANGLES);
	    glNormal3f(vnx,vny,vnz);
	    glVertex3f(p2.getX(),p2.getY(),p2.getZ());
	    glNormal3f(vnx,vny,vnz);
	    glVertex3f(p3.getX(),p3.getY(),p3.getZ());
	    glNormal3f(vnx,vny,vnz);
	    glVertex3f(p0.getX(),p0.getY(),p0.getZ());
    glEnd();
}
