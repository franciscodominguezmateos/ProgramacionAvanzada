/*
 * axis.h
 *
 *  Created on: 10 may 2020
 *      Author: Francisco Dominguez
 */
#pragma once
#include <GL/glut.h>

#include "solid.h"
#include "vector3d.h"
class Axis: public Solid{
 float ax,ay,az;
public:
 Axis():Solid(),ax(1),ay(1),az(1){}
 Axis(Vector3D v):Solid(),ax(v.getX()),ay(v.getY()),az(v.getZ()){}
 Axis(const Axis &c):Solid(c),ax(c.ax),ay(c.ay),az(c.az){}
 Axis *clone(){
	 return new Axis(*this);
 }
 void render(){
	 glPopMatrix();
		glPushMatrix();
		glColor3f(1,0,0);
		glScalef(ax,ax/20.0,ax/20.0);
		glTranslatef(0.5,0,0);
		glutSolidCube(1);
		glPopMatrix();
		glPushMatrix();
		glColor3f(0,1,0);
		glScalef(ay/20.0,ay,ay/20.0);
		glTranslatef(0,0.5,0);
		glutSolidCube(1);
		glPopMatrix();
		glPushMatrix();
		glColor3f(0,0,1);
		glScalef(az/20.0,az/20.0,az);
		glTranslatef(0.0,0,0.5);
		glutSolidCube(1);
	glPopMatrix();
 }
 friend std::ostream &operator << (std::ostream &os, const Axis &v);
};
inline std::ostream &operator<<(std::ostream &os, const Axis &c){
	os << static_cast<const Solid&>( c );
	os <<",ax="<< c.ax<<",ay="<< c.ay<<",az="<< c.az;
    return os;
}
