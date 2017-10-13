/*
 * cilindro.cpp
 *
 *  Created on: 25 Oct 2016
 *      Author: francisco
 */

#include "cilindro.h"

Cilindro::Cilindro(float bp,float hp) {
	 quadratic=gluNewQuadric();
	 b=bp;
	 h=hp;
}
Cilindro::~Cilindro() {
	// TODO Auto-generated destructor stub
}
#include "cubo.h"
void Cilindro::render(){
   glPushMatrix();
     glColor3f(this->getCol().getX(),this->getCol().getY(),this->getCol().getZ());
     glTranslatef(this->getPos().getX(),this->getPos().getY(),this->getPos().getZ());
     gluCylinder(quadratic,b,b,h,16,16);
     glPopMatrix();
}

