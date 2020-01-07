/*
 * Plano.h
 *
 *  Created on: Oct 5, 2015
 *      Author: francisco
 */

#ifndef PLANO_H_
#define PLANO_H_
#include "solido.h"

class Plano: public Solido {
	float a,b,c,d;
public:
	Plano(){a=b=c=d=0;}
	//vn must be normalized
	Plano(Vector3D vn,Vector3D p):a(vn.getX()),b(vn.getY()),c(vn.getZ()),d(-(vn*p)){}
	Plano(const Plano &p):Solido(p),a(p.a),b(p.b),c(p.c),d(p.d){}
	Plano &operator=(const Plano &&p){
		a=p.a;
		b=p.b;
		c=p.c;
		d=p.d;
		return *this;
	}
	virtual ~Plano(){}
	inline float getA() const {return a;}
	inline void setA(float a) {this->a = a;}
	inline float getB() const {return b;}
	inline void setB(float b) {this->b = b;}
	inline float getC() const {return c;}
	inline void setC(float c) {this->c = c;}
	inline float getD() const {return d;}
	inline void setD(float d) {this->d = d;}
	//bool colision(Solido *s);
	inline Vector3D getNormal(){
		Vector3D v(a,b,c);
		//v.normalize();
		return v;
	}
	double distancia(Vector3D p){
		double x=p.getX();
		double y=p.getY();
		double z=p.getZ();
		return a*x+b*y+c*z+d;
	}
	// Project the point p on this plane
	Vector3D project(Vector3D p){
		Vector3D vn=getNormal();
		// distance vector perpendicular to plane and parallel to vn
		double d=this->distancia(p);
		Vector3D vp=vn*d;
		// p=vt+vp then vt=p-vp
		Vector3D vt=p-vp;
		// the vt vector should be in the triangle plane
		//double dist=distancia(vt);
		//assert(nearZero(dist));
		return vt;
	}
};

#endif /* PLANO_H_ */
