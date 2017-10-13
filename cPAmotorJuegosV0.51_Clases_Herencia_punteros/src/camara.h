/*
 * Camara.h
 *
 *  Created on: 4 Oct 2017
 *      Author: Francisco Dominguez
 */

#ifndef CAMARA_H_
#define CAMARA_H_
#include <GL/glut.h>
#include "vector3d.h"

class Camara {
	Vector3D pos;
	Vector3D rot;
public:
	Camara();
	Camara(double x,double y,double z):pos(Vector3D(x,y,z)){}
	virtual ~Camara();
	inline Vector3D getPos() const {return pos;	}
	inline void setPos(Vector3D pos) {this->pos = pos;}
	inline Vector3D getRot() const {return rot;	}
	inline void setRot(Vector3D rot) {this->rot = rot;}
	void render(){
		 glTranslatef(-getPos().getX(),-getPos().getY(),-getPos().getZ());
		 glRotatef(getRot().getX(), 1,0,0);
		 glRotatef(getRot().getY(), 0,1,0);
		 glRotatef(getRot().getZ(), 1,0,1);
	}
};

#endif /* CAMARA_H_ */
