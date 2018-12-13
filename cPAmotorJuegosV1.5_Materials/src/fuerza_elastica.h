/*
 * fuerzaElastica.h
 *
 *  Created on: Oct 11, 2015
 *      Author: francisco
 */

#ifndef SRC_FUERZAELASTICA_H_
#define SRC_FUERZAELASTICA_H_
#include "solido.h"
#include "fuerza.h"

//Una fuerza elastica está formada por un muelle y un amortiguador
//el muelle proporciona fuerza proporcional a la distancia
//el amortiguador proporciona fuerza proporcional a la velocidad
//ambos en el sentido de la distantia entre ambos estremos del elastico
class FuerzaElastica: public Fuerza {
	double longRep;
	double Km;
	double Ka;
	Solido *p1,*p2;
public:
	inline FuerzaElastica():longRep(2.0/51.0),Km(10.0/1.0),Ka(0.55),p1(nullptr),p2(nullptr){};
	inline Solido *getParticula1(){return p1;}
	inline Solido *getParticula2(){return p2;}
	inline void setParticula1(Solido *p){p1=p;}
	inline void setParticula2(Solido *p){p2=p;}
	inline void setKm(double k){Km=k;}
	inline void setKa(double k){Ka=k;}
	inline void setLongitudReposo(double l){longRep=l;}
	inline Vector3D evalua(){
		//Muelle
		Vector3D pos1=p1->getPos();
		Vector3D pos2=p2->getPos();
		Vector3D deltaP=pos1-pos2;
		double dist=deltaP.length();
		//if p1 and p2 are together don't do anything
		if(dist<1e-3)
			return Vector3D();
		double magMuelle=(dist-longRep)*Km;
		//Amortiguador
		Vector3D v1=p1->getVel();
		Vector3D v2=p2->getVel();
		Vector3D deltaV=v1-v2;
		double magAmortiguador=deltaV*deltaP*Ka/dist;
		//double magAmortiguador=deltaV.length()*Ka;///dist;
		//Fuerza total
		Vector3D vectorFuerza=deltaP;
		vectorFuerza.normalize();
		double magFuerza=-(magMuelle+magAmortiguador);
		return vectorFuerza*magFuerza;
	}
};

#endif /* SRC_FUERZAELASTICA_H_ */
