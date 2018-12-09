/*
 * rozamientoVizcoso.h
 *
 *  Created on: Oct 12, 2015
 *      Author: francisco
 */

#ifndef SRC_ROZAMIENTOVISCOSO_H_
#define SRC_ROZAMIENTOVISCOSO_H_
#include "vector3d.h"
#include "solido.h"

class FuerzaAmortiguada:public Fuerza {
	Solido *p;
	double kv;
public:
	FuerzaAmortiguada():kv(0.515){}
	inline Vector3D getFuerza(Solido &p){
		Vector3D v=p.getVel();
		return -v*kv;
	}
	inline void setP(Solido *s){p=s;}
	inline Vector3D evalua(){return getFuerza(*p);}
};

#endif /* SRC_ROZAMIENTOVISCOSO_H_ */
