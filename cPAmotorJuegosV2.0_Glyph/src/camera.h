/*
 * Camara.h
 *
 *  Created on: 4 Oct 2017
 *      Author: Francisco Dominguez
 */

#pragma once
#include <cmath>
#include "opencv2/opencv.hpp"

#include "solid.h"
#include "util.h"

using namespace cv;

class Camera;
using CameraPtr=Camera*;

class Camera:public Solid {
	Vector3D lookAtPos;
public:
	Camera(double x,double y,double z):Solid(x,y,z),lookAtPos(0,0,0){}
	Camera():Camera(0,0,0){}
	Camera(const Camera &c):Solid(c){}
	virtual ~Camera(){}
	void render(){
		 glTranslatef(-getPos().getX(),-getPos().getY(),-getPos().getZ());
		 glRotatef(getRot().getX(), 1,0,0);
		 glRotatef(getRot().getY(), 0,1,0);
		 glRotatef(getRot().getZ(), 0,0,1);
		 glTranslatef(-lookAtPos.getX(),-lookAtPos.getY(),-lookAtPos.getZ());
	}
	virtual Mat getMat(){
		Mat m1=posEulerAnglesToTransformationMatrix(-lookAtPos,Vector3D());
		Mat m=posEulerAnglesToTransformationMatrix(-getPos(),getRot());
		return m*m1;
}

	inline const Vector3D& getLookAtPos() const {return lookAtPos;}
	inline void setLookAtPos(const Vector3D &lookAtPos) {this->lookAtPos = lookAtPos;}
};

