/*
 * Camara.h
 *
 *  Created on: 4 Oct 2017
 *      Author: Francisco Dominguez
 */

#ifndef CAMARA_H_
#define CAMARA_H_
#include "vector3d.h"

class Camera {
	Vector3D pos;
	Vector3D rot;
public:
	Camera();
	Camera(double x,double y,double z):pos(Vector3D(x,y,z)){}
	virtual ~Camera();
	const Vector3D& getPos() const {
		return pos;
	}
	void setPos(const Vector3D& pos) {
		this->pos = pos;
	}
	const Vector3D& getRot() const {
		return rot;
	}
	void setRot(const Vector3D& rot) {
		this->rot = rot;
	}
};

#endif /* CAMARA_H_ */
