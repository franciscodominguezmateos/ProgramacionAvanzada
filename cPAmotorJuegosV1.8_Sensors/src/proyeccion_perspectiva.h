/*
 * proyeccion_perspectiva.h
 *
 *  Created on: Oct 31, 2017
 *      Author: Francisco Dominguez
 */

#ifndef PROYECCION_PERSPECTIVA_H_
#define PROYECCION_PERSPECTIVA_H_
#include "opencv2/opencv.hpp"
#include "proyeccion.h"

using namespace cv;

class ProyeccionPerspectiva: public Proyeccion {
public:
	double campoDeVision;
	double width;
	double height;
	double aspectRatio;
	double zNear;
	double zFar;
	double fov;
public:
	ProyeccionPerspectiva(double fov=30.0,double width=640,double height=480,double zNear=0.5,double zFar=2e12):
		campoDeVision(fov),
		width(width),height(height),
		aspectRatio(width/height),
		zNear(zNear),zFar(zFar),fov(fov){
	}
	virtual ~ProyeccionPerspectiva(){}
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
	inline void setWidth(double width) { aspectRatio=width/height; this->width = width;}
	virtual void reshape(double width,double height){setWidth(width),setHeight(height);}
};
class ProyeccionCamara: public ProyeccionPerspectiva{
	/* This class mimic a real camera from its intrinsic matrix */
	/* Intrinsic matrix */
	Mat camMtx;
	/* Projection matrix */
	double projectionMat[16];
public:
	ProyeccionCamara(Mat &K):camMtx(K){}
	void reshape(double width,double height){
		ProyeccionPerspectiva::reshape(width,height);
		projectionMat[0]  = 2*camMtx.at<double>(0,0)/width;
		projectionMat[1]  = 0;
		projectionMat[2]  = 0;
		projectionMat[3]  = 0;

		projectionMat[4]  = 0;
		projectionMat[5]  = 2*camMtx.at<double>(1,1)/height;
		projectionMat[6]  = 0;
		projectionMat[7]  = 0;

		projectionMat[8]  =  1 -  2*camMtx.at<double>(0,2)     /width;
		projectionMat[9]  = -1 + (2*camMtx.at<double>(1,2) + 2)/height;
		projectionMat[10] = (zNear + zFar)/(zNear - zFar);
		projectionMat[11] = -1;

		projectionMat[12] = 0;
		projectionMat[13] = 0;
		projectionMat[14] = 2*zNear*zFar/(zNear - zFar);
		projectionMat[15] = 0;
	}
	/* render method build OpenGL projection matrix from OpenCV intrinsic camera matrix */
	void render(){
		 glMatrixMode(GL_PROJECTION);
		 glLoadIdentity();
		 glLoadMatrixd(projectionMat);
		 glMatrixMode(GL_MODELVIEW);
	}
};

#endif /* PROYECCION_PERSPECTIVA_H_ */
