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
#include "vector3d.h"

inline double rot2rad(double  a){return a/180*M_PI;}

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
class CamaraFPS: public Camara {
public:
	CamaraFPS(double x=0,double y=1.65,double z=0):Camara(x,y,z){}
	void update(double dt){
		double ry=rot2rad(getRot().getY());
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
class CamaraFly: public Camara {
public:
	CamaraFly(double x=0,double y=1.65,double z=0):Camara(x,y,z){}
	void update(double dt){
		double ry=rot2rad(getRot().getY());
		double rx=rot2rad(getRot().getX());
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

#endif /* CAMARA_H_ */
