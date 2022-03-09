/*
 * camera_ar.h
 *
 *  Created on: 27 Nov 2020
 *      Author: Francisco Dominguez
 *  Just extracted from camera.h
 */
#pragma once
#include "camera.h"
/*
 * This camera set OpenGL model view Matrix
 * from a OpenCV rotation vector and
 * from a OpenCV translation vector
 */
class CameraAR: public Camera {
	double modelviewMat[16];
public:
	CameraAR(double x=0,double y=1.65,double z=0):Camera(x,y,z){}
	CameraAR(Mat rvec,Mat tvec){setPose(rvec,tvec);}
	void setPose(Mat RVec,Mat tVec){
		Mat rotMtx;
		Rodrigues(RVec, rotMtx);
		modelviewMat[0]  =  rotMtx.at<double>(0,0);
		modelviewMat[1]  = -rotMtx.at<double>(1,0);
		modelviewMat[2]  = -rotMtx.at<double>(2,0);
		modelviewMat[3]  = 0;
		modelviewMat[4]  =  rotMtx.at<double>(0,1);
		modelviewMat[5]  = -rotMtx.at<double>(1,1);
		modelviewMat[6]  = -rotMtx.at<double>(2,1);
		modelviewMat[7]  = 0;
		modelviewMat[8]  =  rotMtx.at<double>(0,2);
		modelviewMat[9]  = -rotMtx.at<double>(1,2);
		modelviewMat[10] = -rotMtx.at<double>(2,2);
		modelviewMat[11] = 0;
		modelviewMat[12] =  tVec.at<double>(0,0);
		modelviewMat[13] = -tVec.at<double>(1,0);
		modelviewMat[14] = -tVec.at<double>(2,0);
		modelviewMat[15] = 1;
	}
	void render(){
		//replace model view with the new matrix
		glLoadMatrixd(modelviewMat);
		//set y up vs z down
		glRotated(-90,1,0,0);
	}
};




