/*
 * caja_elastico.h
 *
 *  Created on: 15/12/2017
 *      Author: Francisco Dominguez
 */

#pragma once
#include "rectangle.h"
#include "solido_elastico.h"

class CajaElastica: public SolidoElastico {
protected:
	Texture tex;
	double w,h,d;
public:
	CajaElastica(double w2,double h2,double d2):w(w2),h(h2),d(d2){
			Solido *p;
			double w=w2/2;
			double h=h2/2;
			double d=d2/2;
			//     5---1
			//    /   /|
			//   4---0 |
			//   |   | 2
			//   |   |/
			//   7---3
			//square of the right
			//right/up/front point
			p=new Solido( w, h, d);
			particulas.push_back(p);
			//right/up/back point
			p=new Solido( w, h,-d);
			particulas.push_back(p);
			//right/bottom/back point
			p=new Solido( w,-h,-d);
			particulas.push_back(p);
			//right/bottom/front point
			p=new Solido( w,-h, d);
			particulas.push_back(p);
			//squre of the left
			p=new Solido(-w, h, d);
			particulas.push_back(p);
			p=new Solido(-w, h,-d);
			particulas.push_back(p);
			p=new Solido(-w,-h,-d);
			particulas.push_back(p);
			p=new Solido(-w,-h, d);
			particulas.push_back(p);
			Solido *p1,*p2;
			for(unsigned int i=0;i<this->getParticulas().size();i++){
				for(unsigned int j=i+1;j<this->getParticulas().size();j++){
					p1=getParticulas()[i];
					p2=getParticulas()[j];
					Vector3D d=p2->getPos()-p1->getPos();
					double length=d.length();
					FuerzaElastica *f1=new FuerzaElastica();
					f1->setLongitudReposo(length);
					f1->setKm(200);
					f1->setParticula1(p1);
					f1->setParticula2(p2);
					fuerzasElasticas.push_back(f1);
				}
			}
			for(Solido *p:this->getParticulas()){
				Vector3D iy(0,h+1,0);
				//p->setM(1);
				p->setPos(p->getPos()+iy);
			}
	}
	void setTexture(Texture t){tex=t;}
	inline void render(){/*
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
		Rectangulo r4(
				getParticula(1)->getPos(),
				getParticula(0)->getPos(),
				getParticula(4)->getPos(),
				getParticula(5)->getPos());
		r4.getTex()=tex;
		r4.render();
		Rectangulo r5(
				getParticula(3)->getPos(),
				getParticula(7)->getPos(),
				getParticula(6)->getPos(),
				getParticula(2)->getPos());
		r5.getTex()=tex;
		r5.render();*/
		SolidoElastico::render();
	}
};

