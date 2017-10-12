/*
 * Camara.h
 *
 *  Created on: 4 Oct 2017
 *      Author: Francisco Dominguez
 */

#ifndef CAMARA_H_
#define CAMARA_H_
#include "vector3d.h"

class Camara {
	Vector3D pos;
	Vector3D rot;
public:
	Camara();
	Camara(double x,double y,double z):pos(Vector3D(x,y,z)){}
	virtual ~Camara();
	Vector3D getPos() const {
		return pos;
	}
	void setPos(Vector3D pos) {
		this->pos = pos;
	}
	Vector3D getRot() const {
		return rot;
	}
	void setRot(Vector3D rot) {
		this->rot = rot;
	}
};

#endif /* CAMARA_H_ */
