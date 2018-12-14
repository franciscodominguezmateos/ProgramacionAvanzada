/*
 * triangulo.cpp
 *
 *  Created on: Nov 12, 2015
 *      Author: Francisco Dominguez
 */

#include "triangulo.h"
#include <cassert>

Triangulo::~Triangulo() {
	// TODO Auto-generated destructor stub
}
void Triangulo::render(){
	Vector3D c=this->getCol();
	glColor3f(c.getX(),c.getY(),c.getZ());
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
    if(drawNormals){
		//Draw normal
    	Vector3D vn=this->getNormal();
    	// triangle position is the center of the triangle
    	Vector3D center=getPos();
		glColor3f(1.0,0,0);
		glLineWidth(2);
		glBegin(GL_LINES);
		  Vector3D up=center+vn;
		  glVertex3f(center.getX(),center.getY(),center.getZ());
		  glVertex3f(up.getX(),up.getY(),up.getZ());
		glEnd();
		glColor3f(0,0,1);
		glBegin(GL_LINE_LOOP);
		  glVertex3f(p0.getX(),p0.getY(),p0.getZ());
		  glVertex3f(p1.getX(),p1.getY(),p1.getZ());
		  glVertex3f(p2.getX(),p1.getY(),p2.getZ());
		glEnd();
    }
    if(textura!=nullptr) textura->desactivar();
}
// I will use double check:
// - Area check
// - Normal check
bool Triangulo::isIn(Vector3D pt){
	// Project pt in triangle plane
	Vector3D p=project(pt);
	// p should be in the plane
	// then this should be always true
	double d=distancia(p);
	if(!nearZero(d))
		assert(nearZero(d));

	//accumulated areas
	double a=0;
	Vector3D vn=getNormal();

	// C H E C K I N G Triangle(p0,p1,p)
	Triangulo t01=Triangulo(p0,p1,p);
	//t01.area should be minor than triangle area
	a=t01.getArea();
	if(a>area)
		return false;
	double d0p=t01.getNormal()*vn;
	//On the other hand  if p is out of triangle then dot product with plane normal should be negative
	//if(d0p<0)
	//	return false;

	// C H E C K I N G Triangle(p1,p2,p)
	Triangulo t12=Triangulo(p1,p2,p);
	//t01.area+t12.area should be minor than triangle area
	a+=t12.getArea();
	if(a>area)
		return false;
	double d1p=t12.getNormal()*vn;
	//On the other hand If p is out of triangle then dot product with plane normal should be negative
	//if(d1p<0)
	//	return false;

	// C H E C K I N G Triangle(p2,p0,p)
	Triangulo t20=Triangulo(p2,p0,p);
	//t01.area+t12.area+t20.area should be EQUAL than triangle area
	a+=t20.getArea();
	if(!nearZero(a-area))
		return false;
	double d2p=t20.getNormal()*vn;
	//On the other hand if p is out of triangle then dot product with plane normal should be negative
	//if(d2p<0)
	//	return false;
	//Here we have double check that p is in the triangle
	return true;
}
