/*
 * cubo_elastico.h
 *
 *  Created on: Oct 22, 2017
 *      Author: francisco
 */

#ifndef CUBO_ELASTICO_H_
#define CUBO_ELASTICO_H_
#include "solido_elastico.h"
#include "rectangulo.h"

class CuboElastico: public SolidoElastico {
	Textura tex;
public:
	CuboElastico(double l){
			Solido *p;
			p=new Solido( l, l, l);
			particulas.push_back(p);
			p=new Solido( l, l,-l);
			particulas.push_back(p);
			p=new Solido( l,-l,-l);
			particulas.push_back(p);
			p=new Solido( l,-l, l);
			particulas.push_back(p);
			p=new Solido(-l, l, l);
			particulas.push_back(p);
			p=new Solido(-l, l,-l);
			particulas.push_back(p);
			p=new Solido(-l,-l,-l);
			particulas.push_back(p);
			p=new Solido(-l,-l, l);
			particulas.push_back(p);
			Solido *p1,*p2;
			for(int i=0;i<this->getParticulas().size();i++){
				for(unsigned int j=i+1;j<this->getParticulas().size();j++){
					p1=getParticulas()[i];
					p2=getParticulas()[j];
					Vector3D d=p2->getPos()-p1->getPos();
					double length=d.length();
					FuerzaElastica *f1=new FuerzaElastica();
					f1->setLongitudReposo(length);
					f1->setKm(300);
					f1->setParticula1(p1);
					f1->setParticula2(p2);
					fuerzasElasticas.push_back(f1);
				}
			}
			for(Solido *p:this->getParticulas()){
				Vector3D iy(0,2,0);
				//p->setM(1);
				p->setPos(p->getPos()+iy);
			}
	}
	void setTexture(Textura t){tex=t;}
	inline void render(){
	    glColor3f(1.0f, 1.0f, 0.0f);
		Rectangulo r0(
				getParticula(0)->getPos(),
				getParticula(1)->getPos(),
				getParticula(2)->getPos(),
				getParticula(3)->getPos());
		r0.getTex()=tex;
		r0.render();
		Rectangulo r1(
				getParticula(4)->getPos(),
				getParticula(5)->getPos(),
				getParticula(6)->getPos(),
				getParticula(7)->getPos());
		r1.getTex()=tex;
		r1.render();
		Rectangulo r2(
				getParticula(0)->getPos(),
				getParticula(3)->getPos(),
				getParticula(7)->getPos(),
				getParticula(4)->getPos());
		r2.getTex()=tex;
		r2.render();
		Rectangulo r3(
				getParticula(1)->getPos(),
				getParticula(2)->getPos(),
				getParticula(6)->getPos(),
				getParticula(5)->getPos());
		r3.getTex()=tex;
		r3.render();
		//SolidoElastico::render();
	}
};

#endif /* CUBO_ELASTICO_H_ */
