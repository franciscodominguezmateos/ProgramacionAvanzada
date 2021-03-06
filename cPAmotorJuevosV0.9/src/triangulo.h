/*
 * triangulo.h
 *
 *  Created on: Nov 12, 2015
 *      Author: Francisco Dominguez
 */
#ifndef TRIANGULO_H_
#define TRIANGULO_H_
#include <iostream>
#include <GL/glut.h>
#include "plano.h"
#include "vector3d.h"

using namespace std;

class Triangulo: public Plano {
	Vector3D p0,p1,p2;
public:
	Triangulo(Vector3D p0,Vector3D p1,Vector3D p2):Plano(),p0(p0),p1(p1),p2(p2){
		Vector3D v0=p1-p0;
		Vector3D v1=p2-p1;
		Vector3D vn=v0.crossProduct(v1);
		if(vn.length()<0.00001)
			cout << "longitud muy pequeña"<<endl;
		else
			vn.normalize();
		float d=vn*p2;
		this->setA(vn.getX());
		this->setB(vn.getY());
		this->setC(vn.getZ());
		this->setD(d);
	}
	Triangulo(const Triangulo &t):Plano(t),p0(t.p0),p1(t.p1),p2(t.p2){}
	virtual Triangulo *clone(){return new Triangulo(*this);}
	inline Vector3D getP0(){return p0;}
	inline Vector3D getP1(){return p1;}
	inline Vector3D getP2(){return p2;}
	virtual ~Triangulo();
	void render();
};

#endif /* TRIANGULO_H_ */
