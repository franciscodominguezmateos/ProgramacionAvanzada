/*
 * camera_tps.h
 *
 *  Created on: 27 Nov 2020
 *      Author: Francisco Dominguez
 *  Just extracted from camera.h
 */
#pragma once
#include "camera.h"
class CameraTPS : public Camera {
	Solid *s;
	bool lookSolido;
public:
	CameraTPS(double x = 0, double y = 2.65, double z = 3) :Camera(x, y, z),s(nullptr),lookSolido(true) {}
	void setSolido(Solid *s) { this->s = s; }
	Solid *getSolido() { return s; }
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
