/*
 * projection_perspective.h
 *
 *  Created on: Oct 31, 2017
 *      Author: Francisco Dominguez
 */
#pragma once
#include "opencv2/opencv.hpp"
#include "projection.h"

using namespace cv;
class ProjectionPerspective;
using ProjectionPerspectivePtr=ProjectionPerspective*;

class ProjectionPerspective: public Projection {
public:
	double campoDeVision;
	double width;
	double height;
	double aspectRatio;
	double zNear;
	double zFar;
	double fov;
public:
	ProjectionPerspective(double fov=130.0,double width=640,double height=480,double zNear=0.5,double zFar=2e4):
		campoDeVision(fov),
		width(width),height(height),
		aspectRatio(width/height),
		zNear(zNear),zFar(zFar),fov(fov){
	}
	virtual ~ProjectionPerspective(){}
	virtual void render(){
		 glMatrixMode(GL_PROJECTION);
		 glLoadIdentity();
		 gluPerspective(fov,aspectRatio,zNear,zFar);
		 glMatrixMode(GL_MODELVIEW);
	}
	Mat getMat(){
		vector<GLfloat> vf=getPerspectiveGLMatrix(fov,aspectRatio,zNear,zFar);
		Mat m(4,4,CV_32F,vf.data());
		return m.t();
	}
	inline double getHeight() const {return height;	}
	inline void setHeight(double height) { aspectRatio=width/height; this->height = height;}
	inline double getWidth() const { return width;	}
	inline void setWidth (double width)  { aspectRatio=width/height; this->width  = width;}
	virtual void reshape(double width,double height){setWidth(width),setHeight(height);}
};
