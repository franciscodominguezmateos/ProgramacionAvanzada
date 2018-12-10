/*
 * rosco.cpp
 *
 *  Created on: 27 Oct 2016
 *      Author: francisco
 */

#include "rosco.h"

Rosco::Rosco(float pr1,float pr2) {
	r1=pr1;
	r2=pr2;
}
Rosco::~Rosco() {
	// TODO Auto-generated destructor stub
}
void Rosco::render(){
glPushMatrix();
  glColor3f(this->getCol().getX(),this->getCol().getY(),this->getCol().getZ());
  glTranslatef(this->getPos().getX(),this->getPos().getY(),this->getPos().getZ());
  glutSolidTorus(r1,r2,16,16);
  glPopMatrix();
}

