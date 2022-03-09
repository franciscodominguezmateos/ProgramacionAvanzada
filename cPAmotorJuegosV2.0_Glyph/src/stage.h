/*
 * escena.h
 *
 *  Created on: 18 Oct 2016
 *      Author: Francisco Dominguez
 *  Lights are the first think to add to a stage
 */
#pragma once
#include "light.h"
#include "composite.h"

class Stage;
using StagePtr=Stage*;

class Stage: public Composite {
public:
	void limpiaFuerzas(){
		for(SolidPtr &s:getSolids())
			s->limpiaFuerza();
	}
	void update(double dt){
		limpiaFuerzas();
		for(SolidPtr &s:getSolids()){
			s->acumulaFuerza(Vector3D(0,-9.8/1.0,0));
			s->update(dt);
		}
	}
};
