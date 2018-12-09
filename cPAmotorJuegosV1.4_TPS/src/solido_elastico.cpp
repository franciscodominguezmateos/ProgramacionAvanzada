/*
 * hilo.cpp
 *
 *  Created on: Oct 11, 2015
 *      Author: francisco
 */

#include "solido_elastico.h"

SolidoElastico::SolidoElastico() {
}
SolidoElastico::SolidoElastico(double maxX,double Y,double Z){
	filaX(maxX,Y,Z);
	particulas[0]->hazFija();
	particulas[9]->hazFija();
}
void SolidoElastico::filaX(double maxX,double Y,double Z) {
	for(int i=0;i<maxX;i++){
		Solido *p=new Solido();
		p->setPos(Vector3D(-1+i/maxX*2.0,Y,Z));
		particulas.push_back(p);
	}
	for(int i=0;i<maxX-1;i++){
		Solido *p1,*p2;
		p1=particulas[i+0];
		p2=particulas[i+1];
		Vector3D pos1=p1->getPos();
		Vector3D pos2=p2->getPos();
		FuerzaElastica *f=new FuerzaElastica();
		double l=(pos1-pos2).length();
		f->setParticula1(p1);
		f->setParticula2(p2);
		f->setLongitudReposo(l*0.5);
		fuerzasElasticas.push_back(f);
	}
}
SolidoElastico::~SolidoElastico() {
	// TODO Auto-generated destructor stub
}

