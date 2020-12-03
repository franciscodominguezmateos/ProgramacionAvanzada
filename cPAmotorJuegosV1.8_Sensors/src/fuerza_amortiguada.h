/*
 * rozamientoVizcoso.h
 *
 *  Created on: Oct 12, 2015
 *      Author: francisco
 */

#ifndef SRC_ROZAMIENTOVISCOSO_H_
#define SRC_ROZAMIENTOVISCOSO_H_
#include "solid.h"
#include "vector3d.h"

class FuerzaAmortiguada:public Fuerza {
	Solid *p;
	double kv;
public:
	FuerzaAmortiguada():kv(0.515){}
	inline Vector3D getFuerza(Solid &p){
		Vector3D v=p.getVel();
		return -v*kv;
	}
	inline void setP(Solid *s){p=s;}
	inline Vector3D evalua(){return getFuerza(*p);}
};

#endif /* SRC_ROZAMIENTOVISCOSO_H_ */
