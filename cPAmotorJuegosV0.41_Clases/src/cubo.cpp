/*
 * cubo.cpp
 *
 *  Created on: 13 Oct 2016
 *      Author: Francisco Dominguez
 */
#include "cubo.h"
void Cubo::render(){
   glPushMatrix();
     glColor3f(color.getX(),color.getY(),color.getZ());
     glTranslatef(pos.getX(),pos.getY(),pos.getZ());
     glutSolidCube(s);
   glPopMatrix();
}





