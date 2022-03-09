/*
 * walking_inverted_pendulum.h
 *
 *  Created on: 3 Oct 2019
 *      Author: Francisco Dominguez
 */

#ifndef WALKING_INVERTED_PENDULUM_H_
#define WALKING_INVERTED_PENDULUM_H_

#include "pendulo.h"

class WalkingInvertedPendulum: public Pendulo {
public:
	WalkingInvertedPendulum(Solid *pt1,Solid *pt2):Pendulo(pt1,pt2){getParticula(0)->setFija(true);}
	void update(double dt){
		Pendulo::update(dt/10.0);
		Solid* pt1=getParticula(0);
		Solid* pt2=getParticula(1);
		 // Walk (inverted pendulum)
		 Vector3D right=Vector3D(1,0,0);
		 Vector3D deltaP=pt2->getPos()-pt1->getPos();
		 double pX=deltaP*right;
		 if(abs(pX)>0.55){
			 //Vector3D newPos=pt1->getPos()+right*pX;
			 pt1->setPos(pt1->getPos()+Vector3D(pX*1.8,0,0));
		 }
	}
	virtual ~WalkingInvertedPendulum(){}
};

#endif /* WALKING_INVERTED_PENDULUM_H_ */
