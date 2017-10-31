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

class ProyeccionPerspectiva: public Proyeccion {
protected:
	double campoDeVision;
	double relacionDeAspecto;
	double width;
	double height;
public:
	ProyeccionPerspectiva(double fov=45.0,double aspecto=640/480):campoDeVision(fov),relacionDeAspecto(aspecto){}
	virtual ~ProyeccionPerspectiva();
	void render(){
		 glMatrixMode(GL_PROJECTION);
		 glLoadIdentity();
		 gluPerspective(45.0f,relacionDeAspecto,0.1f,200.0f);
		 glMatrixMode(GL_MODELVIEW);
	}
};
class ProyeccionCamara: public ProyeccionPerspectiva{
	/* This class mimic a real camera from its intrinsic matrix */
	/* Intrinsic matrix */
	double K[3][3];
	/* Projection matrix */
	double m[4][4];
public:
	ProyeccionCamara(){
	    double fx= K[0][0];
	    double fy=-K[1][1];
	    double cx= K[0][2];
	    double cy= K[1][2];
	    double znear=0.1;
	    double zfar =100.0;
	    //m=Mat.eye(4,4);
	    m[0][0] = 2.0 * fx / width;
	    m[0][1] = 0.0;
	    m[0][2] = 0.0;
	    m[0][3] = 0.0;

	    m[1][0] = 0.0;
	    m[1][1] = -2.0 * fy / height;
	    m[1][2] = 0.0;
	    m[1][3] = 0.0;

	    m[2][0] = 1.0 - 2.0 * cx / width;
	    m[2][1] = 2.0 * cy / height - 1.0;
	    m[2][2] = (zfar + znear) / (znear - zfar);
	    m[2][3] = -1.0;

	    m[3][0] = 0.0;
	    m[3][1] = 0.0;
	    m[3][2] = 2.0 * zfar * znear / (znear - zfar);
	    m[3][3] = 0.0;
	}
	/* render method build OpenGL projection matrix from OpenCV intrinsic camera matrix */
	void render(){
		 glMatrixMode(GL_PROJECTION);
		 glLoadIdentity();
		 //glLoadMatrixd(m);
		 glMatrixMode(GL_MODELVIEW);
	}
};

#endif /* PROYECCION_PERSPECTIVA_H_ */
