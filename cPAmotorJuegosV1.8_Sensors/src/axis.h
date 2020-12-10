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
 Text name;
public:
 Axis():Axis(Vector3D(1,1,1)){}
 Axis(Vector3D v):
	 Solid(),
	 ax(v.getX()),ay(v.getY()),	 az(v.getZ()),
	 awX(ax*0.025,ax),awY(ay*0.025,ay),awZ(az*0.025,az),
	 name("Origin"){
	 awX.setRot(Vector3D(0,90,0));
	 awX.setCol(Vector3D(1,0,0));
	 awX.setName(Text("X"));
	 awY.setRot(Vector3D(-90,0,0));
	 awY.setCol(Vector3D(0,1,0));
	 awY.setName(Text("Y"));
	 awZ.setCol(Vector3D(0,0,1));
	 awZ.setName(Text("Z"));
	 name.setPos(Vector3D(ax*0.1,ay*0.1,az*0.1));
	 hazFija();
 }
 Axis *clone(){ return new Axis(*this); }
 void render(){
	 glPushMatrix();
		glTranslatef(getPos().getX(),getPos().getY(),getPos().getZ());
		glRotatef(getRot().getX(), 1, 0, 0);
		glRotatef(getRot().getY(), 0, 1, 0);
		glRotatef(getRot().getZ(), 0, 0, 1);
	    awX.render();
	    awY.render();
	    awZ.render();
	    name.render();
	 glPopMatrix();

 }
 friend std::ostream &operator << (std::ostream &os, const Axis &v);
	const Arrow& getAwX() const {return awX;}
	void setAwX(const Arrow &awX) {	this->awX = awX;}
	const Arrow& getAwY() const {	return awY;}
	void setAwY(const Arrow &awY) {	this->awY = awY;}
	const Arrow& getAwZ() const {	return awZ;}
	void setAwZ(const Arrow &awZ) {	this->awZ = awZ;}
	float getAx() const {	return ax;}
	void setAx(float ax) {	this->ax = ax;}
	float getAy() const {	return ay;}
	void setAy(float ay) {	this->ay = ay;	}
	float getAz() const {	return az;	}
	void setAz(float az) {	this->az = az;	}
	const Text& getName() const {	return name;	}
	void setName(const Text &name) {
		this->name = name;
		this->name.setPos(Vector3D(ax*0.1,ay*0.1,az*0.1));
	}
};
inline std::ostream &operator<<(std::ostream &os, const Axis &c){
	os << static_cast<const Solid&>( c );
	os <<",ax="<< c.ax<<",ay="<< c.ay<<",az="<< c.az;
    return os;
}
