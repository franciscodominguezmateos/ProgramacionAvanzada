/*
 * projection_camera.h
 *
 *  Created on: 13 Jan 2021
 *      Author: Francisco Dominguez
 */
#pragma once
#include "projection_perspective.h"
class ProjectionCamera: public ProjectionPerspective{
	/* This class mimic a real camera from its intrinsic matrix */
	/* Intrinsic matrix */
	Mat K;
	/* Projection matrix */
	double projectionMat[16];
public:
	ProjectionCamera(Mat &K):K(K){}
	void reshape(double width,double height){
		ProjectionPerspective::reshape(width,height);
		double &fx=K.at<double>(0,0);
		double &fy=K.at<double>(1,1);
		double &cx=K.at<double>(0,2);
		double &cy=K.at<double>(1,2);
		projectionMat[0]  = 2 * fx/width;
		projectionMat[1]  = 0;
		projectionMat[2]  = 0;
		projectionMat[3]  = 0;

		projectionMat[4]  = 0;
		projectionMat[5]  = 2 * fy/height;
		projectionMat[6]  = 0;
		projectionMat[7]  = 0;

		projectionMat[8]  =  1 -  2 * cx     /width;
		projectionMat[9]  = -1 + (2 * cy + 2)/height;
		projectionMat[10] = (zNear + zFar)/(zNear - zFar);
		projectionMat[11] = -1;

		projectionMat[12] = 0;
		projectionMat[13] = 0;
		projectionMat[14] = 2*zNear*zFar/(zNear - zFar);
		projectionMat[15] = 0;
	}
	Mat getMat(){return K;}
	/* render method build OpenGL projection matrix from OpenCV intrinsic camera matrix */
	void render(){
		 glMatrixMode(GL_PROJECTION);
		 glLoadIdentity();
		 glLoadMatrixd(projectionMat);
		 glMatrixMode(GL_MODELVIEW);
	}
};





