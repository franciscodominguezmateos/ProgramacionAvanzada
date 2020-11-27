/*
 * camera_fly.h
 *
 *  Created on: 27 Nov 2020
 *      Author: Francisco Dominguez
 *  Just extracted from camera.h
 */
#pragma once
#include "camera_fps.h"
class CameraFly: public Camera {
public:
	CameraFly(double x=0,double y=1.65,double z=0):Camera(x,y,z){}
	void update(double dt){
		double ry=deg2rad(getRot().getY());
		double rx=deg2rad(getRot().getX());
		Vector3D vel={-cos(rx)*sin(ry),sin(rx),cos(rx)*cos(ry)};
		setPos(getPos()-vel*dt);
	}
	void render(){
		 glRotatef(getRot().getX(), 1,0,0);
		 glRotatef(getRot().getY(), 0,1,0);
		 glRotatef(getRot().getZ(), 1,0,1);
		 glTranslatef(-getPos().getX(),-getPos().getY(),-getPos().getZ());
	}
};
