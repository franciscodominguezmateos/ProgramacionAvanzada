/*
 * camera_tps_vr.h
 *
 *  Created on: 27 Nov 2020
 *      Author: Francisco Dominguez
 *  Just extracted from camera.h
 */
#pragma once
#include "camera_tps.h"

class CameraTPSVR: public CameraTPS {
	double baseline;
public:
	CameraTPSVR(double x=0,double y=1.65,double z=0):CameraTPS(x,y,z),baseline(0.25){}
	inline double getBaseline(){return baseline;}
	inline void setBaseline(double b){baseline=b;}
	void renderLeft(){
		glTranslatef(-baseline,0,0);
		render();
	}
	void renderRight(){
		glTranslatef(baseline,0,0);
		render();
	}
};
