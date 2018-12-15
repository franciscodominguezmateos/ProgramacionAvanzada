/*
 * caja_elastico.h
 *
 *  Created on: 15/12/2017
 *      Author: Francisco Dominguez
 */

#pragma once
#include "solido_elastico.h"
#include "rectangulo.h"

class CajaElastica: public SolidoElastico {
	Textura tex;
	double w,h,d;
public:
	CajaElastica(double w2,double h2,double d2):w(w),h(h),d(d){
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
				Vector3D iy(0,h+1,0);
				//p->setM(1);
				p->setPos(p->getPos()+iy);
			}
	}
	void setTexture(Textura t){tex=t;}
	inline void render(){/*
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
		r3.render();*/
		SolidoElastico::render();
	}
};

