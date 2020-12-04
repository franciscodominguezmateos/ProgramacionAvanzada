/*
 * arrow.h
 *
 *  Created on: 1 Dec 2020
 *      Author: Francisco Dominguez
 */
#pragma once
#include "solid.h"

class Arrow: public Solid {
	GLUquadricObj *quadratic;
	double b,h;
	double tipB,tipH;
public:
	Arrow(double b=1,double h=1):Solid(),b(b),h(h),tipB(b*1.5),tipH(h*0.2){
		 quadratic=gluNewQuadric();
	}
	Arrow(const Arrow &other):Solid(other),b(other.b),h(other.h),tipB(other.b*1.1),tipH(other.h*0.1){
		 quadratic=gluNewQuadric();
	}
	void render(){
		glPushMatrix();
		    glColor3f(this->getCol().getX(),this->getCol().getY(),this->getCol().getZ());
		    glTranslatef(this->getPos().getX(),this->getPos().getY(),this->getPos().getZ());
			glRotatef(getRot().getX(), 1, 0, 0);
			glRotatef(getRot().getY(), 0, 1, 0);
			glRotatef(getRot().getZ(), 0, 0, 1);
		    gluCylinder(quadratic,b,b,h-tipH,16,16);
		    glTranslatef(0,0,h-tipH);
		    gluCylinder(quadratic,tipB,0,tipH,16,16);
		glPopMatrix();
	}
};
