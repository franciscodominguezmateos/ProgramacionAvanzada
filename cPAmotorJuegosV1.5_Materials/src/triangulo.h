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
#include "textura.h"

using namespace std;

class Triangulo: public Plano {
protected:
	Vector3D p0,p1,p2;
	Vector3D n0,n1,n2;
	Vector3D t0,t1,t2;
	Vector3D v01,v12,v20;
	Textura* textura;
	double area;
	bool drawNormals;
public:
	Triangulo(Vector3D p0,Vector3D p1,Vector3D p2):
		Plano(),
		p0(p0),p1(p1),p2(p2),
        v01(p1-p0),v12(p2-p1),v20(p0-p2),
		textura(nullptr){
		// VERY IMPORTANT triangles points order is uncounterclock
		Vector3D vn=v01.crossProduct(v12);
		//Area of a triangle is half the lenght of the cross product
		area=vn.length()/2;
		try{
			vn.normalize();
		}
		catch (exception &e){

		}
		//Initial homogeneous vertex normals
		n0=n1=n2=vn;
		float d=vn*p2;
		this->setA(vn.getX());
		this->setB(vn.getY());
		this->setC(vn.getZ());
		this->setD(-d);
		//center of the triangle is pos field
		this->setPos((p0+p1+p2)/3);
		drawNormals=false;
	}
	Triangulo(const Triangulo &t):Plano(t),p0(t.p0),p1(t.p1),p2(t.p2),
			v01(t.v01),v12(t.v12),v20(t.v20),
			textura(t.textura),area(t.area),drawNormals(t.drawNormals){}
	virtual Triangulo *clone(){return new Triangulo(*this);}
	Triangulo& operator=(const Triangulo &t){
		p0=t.p0;
		p1=t.p1;
		p2=t.p2;
		v01=t.v01;
		v12=t.v12;
		v20=t.v20;
		textura=t.textura;
		area=t.area;
		drawNormals=t.drawNormals;
	}
	inline Vector3D &getP0(){return p0;}
	inline Vector3D &getP1(){return p1;}
	inline Vector3D &getP2(){return p2;}
	inline Vector3D &getN0(){return n0;}
	inline Vector3D &getN1(){return n1;}
	inline Vector3D &getN2(){return n2;}
	inline Vector3D &getT0(){return t0;}
	inline Vector3D &getT1(){return t1;}
	inline Vector3D &getT2(){return t2;}
	inline double &getArea(){return area;}
	inline void setP0(Vector3D &n){p0=n;}
	inline void setP1(Vector3D &n){p1=n;}
	inline void setP2(Vector3D &n){p2=n;}
	inline void setN0(Vector3D &n){n0=n;}
	inline void setN1(Vector3D &n){n1=n;}
	inline void setN2(Vector3D &n){n2=n;}
	inline void setT0(Vector3D &n){t0=n;}
	inline void setT1(Vector3D &n){t1=n;}
	inline void setT2(Vector3D &n){t2=n;}
	inline void setTextura(Textura* &t){textura=t;}
	inline Vector3D getCenter(){return getPos();}
	inline void setDrawNormals(bool b){drawNormals=b;}
	inline void doScale(double s){
		p0*=s;
		p1*=s;
		p2*=s;
		v01*=s;
		v12*=s;
		v20*=s;
		Vector3D vn=v01.crossProduct(v12);
		//Area of a triangle is half the lenght of the cross product
		area=vn.length()/2;
		float d=getNormal()*p2;
		this->setD(-d);
		//center of the triangle is pos field
		this->setPos((p0+p1+p2)/3);

	}
	virtual ~Triangulo();
	void render();
	bool isIn(Vector3D p);
};

#endif /* TRIANGULO_H_ */
