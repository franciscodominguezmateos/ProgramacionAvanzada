/*
 * rectangulo.h
 *
 *  Created on: Oct 18, 2017
 *      Author: francisco
 */

#ifndef RECTANGULO_H_
#define RECTANGULO_H_

#include "triangulo.h"

class Rectangulo: public Triangulo {
	Vector3D p3;
public:
	Rectangulo(Vector3D p0,Vector3D p1,Vector3D p2,Vector3D p3);
	virtual ~Rectangulo();
	Rectangulo(const Rectangulo &r):Triangulo(r.p0,r.p1,r.p2),p3(r.p3){}
	virtual Rectangulo *clone(){return new Rectangulo(*this);}
	inline Vector3D getP3(){return p3;}
	void render();
};

#endif /* RECTANGULO_H_ */
