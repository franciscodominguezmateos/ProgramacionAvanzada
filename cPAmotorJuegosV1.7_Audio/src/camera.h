/*
 * Camara.h
 *
 *  Created on: 4 Oct 2017
 *      Author: Francisco Dominguez
 */

#pragma once
#include <cmath>
#include <GL/glut.h>
#include "opencv2/opencv.hpp"

#include "solid.h"
#include "vector3d.h"
#include "util.h"

using namespace cv;

class Camera:public Solid {
public:
	Camera(double x,double y,double z):Solid(x,y,z){}
	Camera():Camera(0,0,0){}
	virtual ~Camera(){}
	void render(){
		 glTranslatef(-getPos().getX(),-getPos().getY(),-getPos().getZ());
		 glRotatef(getRot().getX(), 1,0,0);
		 glRotatef(getRot().getY(), 0,1,0);
		 glRotatef(getRot().getZ(), 0,0,1);
	}
	virtual Mat getMat(){
		return posEulerAnglesToTransformationMatrix(getPos(),-getRot());
	}
};

