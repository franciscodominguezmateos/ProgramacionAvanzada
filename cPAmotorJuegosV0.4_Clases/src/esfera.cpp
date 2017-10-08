/*
 * esfera.cpp
 *
 *  Created on: 13 Oct 2016
 *      Author: Francisco Dominguez
 */
#include "esfera.h"
void Esfera::render(){
   glPushMatrix();
     glColor3f(color.getX(),color.getY(),color.getZ());
     glTranslatef(pos.getX(),pos.getY(),pos.getZ());
     glutSolidSphere(r,10,10);
   glPopMatrix();
}





