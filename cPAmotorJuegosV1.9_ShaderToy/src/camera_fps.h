/*
 * camera_fps.h
 *
 *  Created on: 27 Nov 2020
 *      Author: Francisco Dominguez
 *  Just extracted from camera.h
 */
#pragma once
#include "camera.h"

class CameraFPS: public Camera {
public:
	CameraFPS(double x=0,double y=1.65,double z=0):Camera(x,y,z){}
	void update(double dt){
		double ry=deg2rad(getRot().getY());
		Vector3D vel={-sin(ry),0,cos(ry)};
		setPos(getPos()-vel*dt);
	}
	void updateLateral(double dt) {
		double ry = deg2rad(getRot().getY());
		Vector3D newV = { -cos(ry),0,sin(ry) };
		setPos(getPos() + newV * dt);
	}
	void render(){
		 glRotatef(getRot().getX(), 1,0,0);
		 glRotatef(getRot().getY(), 0,1,0);
		 glRotatef(getRot().getZ(), 0,0,1);
		 glTranslatef(-getPos().getX(),-getPos().getY(),-getPos().getZ());
	}
	virtual Mat getMat(){
		Mat m1=posEulerAnglesToTransformationMatrix(getPos(),Vector3D());
		Mat m2=posEulerAnglesToTransformationMatrix(Vector3D(),-getRot());
		return m2*m1;
	}
};

