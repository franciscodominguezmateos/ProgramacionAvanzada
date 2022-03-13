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
class CameraAR;
using CameraARPtr=CameraAR*;
class CameraAR: public Camera {
	double modelviewMat[16];
	// Lie algebra filtered pose representation
	Mat rv,tv;
	double alpha;
	Mat rotMtx;
public:
	CameraAR(double x=0,double y=1.65,double z=0):
		Camera(x,y,z),
		rv(Mat::zeros(3,1,cv::DataType<double>::type)),
		tv(Mat::zeros(3,1,cv::DataType<double>::type)),
		alpha(0.3){}
	CameraAR(Mat rvec,Mat tvec){setPose(rvec,tvec);}
	void setPose(Mat rvec,Mat tvec){
		rv=rv*(1.0-alpha)+rvec*alpha;
		tv=tv*(1.0-alpha)+tvec*alpha;
		//cout << "tv"<<endl;
		//printMat(tv);
		//cout << "tvec" <<endl;
		//printMat(tvec);
		Rodrigues(rv, rotMtx);
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
		modelviewMat[12] =  tv.at<double>(0,0);
		modelviewMat[13] = -tv.at<double>(1,0);
		modelviewMat[14] = -tv.at<double>(2,0);
		modelviewMat[15] = 1;
	}
	void render(){
		//replace model view with the new matrix
		glLoadMatrixd(modelviewMat);
		//set y up vs z down
		glRotated(-90,1,0,0);
		//set x left
		glRotated( 90,0,1,0);
	}
	double getAlpha(){return alpha;}
	void   setAlpha(double d){alpha=d;}
};




