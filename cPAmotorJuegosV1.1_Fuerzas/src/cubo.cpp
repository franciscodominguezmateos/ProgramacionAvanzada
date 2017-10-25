/*
 * cubo.cpp
 *
 *  Created on: 13 Oct 2016
 *      Author: Francisco Dominguez
 */
#include "cubo.h"
void Cubo::render(){
   glPushMatrix();
     glColor3f(this->getCol().getX(),this->getCol().getY(),this->getCol().getZ());
     glTranslatef(this->getPos().getX(),this->getPos().getY(),this->getPos().getZ());
     glutSolidCube(s);
   glPopMatrix();
}





