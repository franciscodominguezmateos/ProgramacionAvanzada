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
#include "arrow.h"

class Axis: public Solid{
 float ax,ay,az;
 Arrow awX,awY,awZ;
public:
 Axis():Axis(Vector3D(1,1,1)){}
 Axis(Vector3D v):Solid(),ax(v.getX()),ay(v.getY()),az(v.getZ()),awX(ax*0.05,ax),awY(ay*0.05,ay),awZ(az*0.05,az){
	 awX.setRot(Vector3D(0,90,0));
	 awX.setCol(Vector3D(1,0,0));
	 awY.setRot(Vector3D(-90,0,0));
	 awY.setCol(Vector3D(0,1,0));
	 awZ.setCol(Vector3D(0,0,1));
	 hazFija();
 }
 Axis *clone(){
	 return new Axis(*this);
 }
 void render(){
	 glPushMatrix();
		glTranslatef(getPos().getX(),getPos().getY(),getPos().getZ());
		glRotatef(getRot().getX(), 1, 0, 0);
		glRotatef(getRot().getY(), 0, 1, 0);
		glRotatef(getRot().getZ(), 0, 0, 1);
	    awX.render();
	    awY.render();
	    awZ.render();
	 glPopMatrix();

 }
 friend std::ostream &operator << (std::ostream &os, const Axis &v);
};
inline std::ostream &operator<<(std::ostream &os, const Axis &c){
	os << static_cast<const Solid&>( c );
	os <<",ax="<< c.ax<<",ay="<< c.ay<<",az="<< c.az;
    return os;
}
