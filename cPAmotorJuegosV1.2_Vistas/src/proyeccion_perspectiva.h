/*
 * proyeccion_perspectiva.h
 *
 *  Created on: Oct 31, 2017
 *      Author: francisco
 */

#ifndef PROYECCION_PERSPECTIVA_H_
#define PROYECCION_PERSPECTIVA_H_
#include "opencv2/opencv.hpp"
#include "proyeccion.h"

using namespace cv;

class ProyeccionPerspectiva: public Proyeccion {
protected:
	double campoDeVision;
	double relacionDeAspecto;
	double width;
	double height;
	double zNear;
	double zFar;
public:
	ProyeccionPerspectiva(double fov=45.0,double aspecto=640/480):campoDeVision(fov),relacionDeAspecto(aspecto),zNear(0.1),zFar(2000){}
	virtual ~ProyeccionPerspectiva();
	void render(){
		 glMatrixMode(GL_PROJECTION);
		 glLoadIdentity();
		 gluPerspective(45.0f,relacionDeAspecto,zNear,zFar);
		 glMatrixMode(GL_MODELVIEW);
	}
};
class ProyeccionCamara: public ProyeccionPerspectiva{
	/* This class mimic a real camera from its intrinsic matrix */
	/* Intrinsic matrix */
	Mat camMtx;
	/* Projection matrix */
	double projectionMat[16];
public:
	ProyeccionCamara(){
		projectionMat[0]  = 2*camMtx.at<double>(0,0)/width;
		projectionMat[1]  = 0;
		projectionMat[2]  = 0;
		projectionMat[3]  = 0;
		projectionMat[4]  = 0;
		projectionMat[5]  = 2*camMtx.at<double>(1,1)/height;
		projectionMat[6]  = 0;
		projectionMat[7]  = 0;
		projectionMat[8]  = 1 - 2*camMtx.at<double>(0,2)/width;
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
