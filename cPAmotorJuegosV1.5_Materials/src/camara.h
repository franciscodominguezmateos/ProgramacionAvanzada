/*
 * Camara.h
 *
 *  Created on: 4 Oct 2017
 *      Author: Francisco Dominguez
 */

#ifndef CAMARA_H_
#define CAMARA_H_
#include <cmath>
#include <GL/glut.h>
#include "opencv2/opencv.hpp"
#include "vector3d.h"
#include "solido.h"
#include "util.h"

using namespace cv;

class Camera:public Solido {
public:
	Camera(double x,double y,double z):Solido(x,y,z){}
	Camera():Camera(0,0,0){}
	virtual ~Camera();
	void render(){
		 glTranslatef(-getPos().getX(),-getPos().getY(),-getPos().getZ());
		 glRotatef(getRot().getX(), 1,0,0);
		 glRotatef(getRot().getY(), 0,1,0);
		 glRotatef(getRot().getZ(), 1,0,1);
	}
};
class CamaraFPS: public Camera {
public:
	CamaraFPS(double x=0,double y=1.65,double z=0):Camera(x,y,z){}
	void update(double dt){
		double ry=deg2rad(getRot().getY());
		Vector3D vel={-sin(ry),0,cos(ry)};
		setPos(getPos()-vel*dt);
	}
	void render(){
		 glRotatef(getRot().getX(), 1,0,0);
		 glRotatef(getRot().getY(), 0,1,0);
		 glRotatef(getRot().getZ(), 1,0,1);
		 glTranslatef(-getPos().getX(),-getPos().getY(),-getPos().getZ());
	}
};
class CamaraTPS : public Camera {
	Solido *s;
public:
	CamaraTPS(double x = 0, double y = 1.65, double z = 0) :Camera(x, y, z) {}
	void setSolido(Solido *s) { this->s = s; }
	Solido *getSolido() { return s; }
	void update(double dt) {
		double ry = deg2rad(s->getRot().getY());
		Vector3D vel = { sin(ry),0,cos(ry) };
		//cout << "vel=" << vel << endl;
		//s->setVel(vel);
		s->setPos(s->getPos() + vel * dt);
	}
	void updateLateral(double dt) {
		double ry = deg2rad(s->getRot().getY());
		Vector3D newV = { -cos(ry),0,sin(ry) };
		s->setPos(s->getPos() + newV * dt);
	}
	void render() {
		glTranslatef(-this->getPos().getX(), -this->getPos().getY(), -this->getPos().getZ());
		glRotatef(getRot().getX(), 1, 0, 0);
		glRotatef(getRot().getY(), 0, 1, 0);
		glRotatef(getRot().getZ(), 0, 0, 1);

		glRotatef(-s->getRot().getX(), 1, 0, 0);
		glRotatef(-s->getRot().getY(), 0, 1, 0);
		glRotatef(-s->getRot().getZ(), 1, 0, 1);
		glTranslatef(-s->getPos().getX(), -s->getPos().getY(), -s->getPos().getZ());
	}
};
class CamaraFly: public Camera {
public:
	CamaraFly(double x=0,double y=1.65,double z=0):Camera(x,y,z){}
	void update(double dt){
		double ry=deg2rad(getRot().getY());
		double rx=deg2rad(getRot().getX());
		Vector3D vel={-cos(rx)*sin(ry),sin(rx),cos(rx)*cos(ry)};
		setPos(getPos()-vel*dt);
	}
	void render(){
		 glRotatef(getRot().getX(), 1,0,0);
		 glRotatef(getRot().getY(), 0,1,0);
		 glRotatef(getRot().getZ(), 1,0,1);
		 glTranslatef(-getPos().getX(),-getPos().getY(),-getPos().getZ());
	}
};
/*
 * This camera set OpemGL model view Matrix
 * from a OpenCV rotation vector and
 * from a OpenCV translation vector
 */
class CamaraAR: public Camera {
	double modelviewMat[16];
public:
	CamaraAR(double x=0,double y=1.65,double z=0):Camera(x,y,z){}
	CamaraAR(Mat rvec,Mat tvec){setPose(rvec,tvec);}
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
		glRotatef(-90,1,0,0);
	}
};

#endif /* CAMARA_H_ */
