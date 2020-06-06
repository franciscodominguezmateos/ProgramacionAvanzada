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

class Camara:public Solido {
public:
	Camara(double x,double y,double z):Solido(x,y,z){}
	Camara():Camara(0,0,0){}
	virtual ~Camara(){}
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
class CamaraFPS: public Camara {
public:
	CamaraFPS(double x=0,double y=1.65,double z=0):Camara(x,y,z){}
	void update(double dt){
		double ry=deg2rad(getRot().getY());
		Vector3D vel={-sin(ry),0,cos(ry)};
		setPos(getPos()-vel*dt);
	}
	void updateLateral(double dt) {
		double ry = deg2rad(getRot().getY());
		Vector3D newV = { -cos(ry),0,sin(ry) };
		setPos(getPos() + newV * dt);
	}
	void render(){
		 glRotatef(getRot().getX(), 1,0,0);
		 glRotatef(getRot().getY(), 0,1,0);
		 glRotatef(getRot().getZ(), 0,0,1);
		 glTranslatef(-getPos().getX(),-getPos().getY(),-getPos().getZ());
	}
	virtual Mat getMat(){
		Mat m1=posEulerAnglesToTransformationMatrix(getPos(),Vector3D());
		Mat m2=posEulerAnglesToTransformationMatrix(Vector3D(),-getRot());
		return m2*m1;
	}
};
class CamaraFPSVR: public CamaraFPS {
	double baseline;
public:
	CamaraFPSVR(double x=0,double y=1.65,double z=0):CamaraFPS(x,y,z),baseline(0.25){}
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
class CamaraTPS : public Camara {
	Solido *s;
	bool lookSolido;
public:
	CamaraTPS(double x = 0, double y = 2.65, double z = 3) :Camara(x, y, z),s(nullptr),lookSolido(true) {}
	void setSolido(Solido *s) { this->s = s; }
	Solido *getSolido() { return s; }
	inline Vector3D getVectorOrienationY(){
		double ry = deg2rad(s->getRot().getY());
		Vector3D vel = { sin(ry),0,cos(ry) };
		return vel;
	}
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
		if (lookSolido)  glTranslatef(-this->getPos().getX(), -this->getPos().getY(), -this->getPos().getZ());
		glRotatef(getRot().getX(), 1, 0, 0);
		glRotatef(getRot().getY(), 0, 1, 0);
		glRotatef(getRot().getZ(), 0, 0, 1);
		if (!lookSolido) glTranslatef(this->getPos().getX(), -this->getPos().getY(), this->getPos().getZ());

		glRotatef(-s->getRot().getX(), 1, 0, 0);
		glRotatef(-s->getRot().getY(), 0, 1, 0);
		glRotatef(-s->getRot().getZ(), 0, 0, 1);
		glTranslatef(-s->getPos().getX(), -s->getPos().getY(), -s->getPos().getZ());
	}
	bool isLookSolido() const {return lookSolido;}
	void setLookSolido(bool lookSolido) {this->lookSolido = lookSolido;}
	virtual Mat getMat(){
		Mat m1=posEulerAnglesToTransformationMatrix(-s->getPos(),Vector3D());
		Mat m2=posEulerAnglesToTransformationMatrix(Vector3D(),-s->getRot());
		Mat m=posEulerAnglesToTransformationMatrix(-getPos(),getRot());
		return m*m2*m1;
	}
};
class CamaraTPSVR: public CamaraTPS {
	double baseline;
public:
	CamaraTPSVR(double x=0,double y=1.65,double z=0):CamaraTPS(x,y,z),baseline(0.25){}
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
class CamaraFly: public Camara {
public:
	CamaraFly(double x=0,double y=1.65,double z=0):Camara(x,y,z){}
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
class CamaraAR: public Camara {
	double modelviewMat[16];
public:
	CamaraAR(double x=0,double y=1.65,double z=0):Camara(x,y,z){}
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
