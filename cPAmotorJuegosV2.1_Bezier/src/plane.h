/*
 * Plano.h
 *
 *  Created on: Oct 5, 2015
 *      Author: Francisco Dominguez
 */
#pragma once
#include "solid.h"

class Plane: public Solid {
	float a,b,c,d;
public:
	Plane(){a=b=c=d=0;}
	//vn must be normalized
	Plane(Vector3D vn,Vector3D p):a(vn.getX()),b(vn.getY()),c(vn.getZ()),d(-(vn*p)){}
/*	Plane(const Plane &p):Solido(p),a(p.a),b(p.b),c(p.c),d(p.d){}
	Plane &operator=(const Plane &&p){
		a=p.a;
		b=p.b;
		c=p.c;
		d=p.d;
		return *this;
	}*/
	virtual ~Plane(){}
	Mat asMatHomogeneous(){
		Vector3D p=getNearestPoint();
		Vector3D v=getNormal();
		Mat pts=Mat::zeros(4,2,CV_64F);
		Mat M0 = pts.col(0);
		Mat M1 = pts.col(1);
		p.asH().copyTo(M0);
		v.asH0().copyTo(M1);
		return pts;
	}
	inline float getA() const {return a;}
	inline void setA(float a) {this->a = a;}
	inline float getB() const {return b;}
	inline void setB(float b) {this->b = b;}
	inline float getC() const {return c;}
	inline void setC(float c) {this->c = c;}
	inline float getD() const {return d;}
	inline void setD(float d) {this->d = d;}
	inline Vector3D getNearestPoint(){return Vector3D(-getNormal()*d);}
	//bool colision(Solido *s);
	inline Vector3D getNormal(){
		Vector3D v(a,b,c);
		try{
			v.normalize();
		}
		catch(runtime_error &e){
			throw runtime_error(" in Plane::getNormal(): "+string(e.what()));
		}
		return v;
	}
	double distance(Vector3D p){
		double x=p.getX();
		double y=p.getY();
		double z=p.getZ();
		return a*x+b*y+c*z+d;
	}
	// Project the point p on this plane
	Vector3D project(Vector3D p){
		Vector3D vn=getNormal();
		// distance vector perpendicular to plane and parallel to vn
		double d=this->distance(p);
		Vector3D vp=vn*d;
		// p=vt+vp then vt=p-vp
		Vector3D vt=p-vp;
		// the vt vector should be in the triangle plane
		//double dist=distancia(vt);
		//assert(nearZero(dist));
		return vt;
	}
};
