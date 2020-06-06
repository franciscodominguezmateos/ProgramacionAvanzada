/*
 * hilo.h
 *
 *  Created on: Oct 11, 2015
 *      Author: francisco
 */

#ifndef SOLIDO_ELASTICO_H_
#define SOLIDO_ELASTICO_H_
#include <vector>

#include "solido.h"
#include "fuerza_elastica.h"
#include "fuerza_amortiguada.h"

using namespace std;

class SolidoElastico:public Solido {
protected:
	vector<Solido *> particulas;
	vector<FuerzaElastica *> fuerzasElasticas;
	FuerzaAmortiguada fa;
public:
	SolidoElastico(){}
	SolidoElastico(double maxX,double Y,double Z){
		filaX(maxX,Y,Z);
		particulas[0]->hazFija();
		particulas[9]->hazFija();
	}
	virtual ~SolidoElastico(){}
	void filaX(double maxX,double Y,double Z) {
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
	inline const vector<Solido *> &getParticulas(){return particulas;}
	inline Solido *getParticula(unsigned i){
		assert(i<particulas.size());
		return particulas[i];}
	inline vector<FuerzaElastica *> getFuerzasElasticas(){return fuerzasElasticas;}
	inline void setParticulas(vector<Solido *> pts){particulas=pts;}
	inline void add(Solido *s){particulas.push_back(s);}
	inline void add(FuerzaElastica *f){fuerzasElasticas.push_back(f);}
	inline Solido *getPrimeraParticula(){return particulas[0];}
	inline Solido *getUltimaParticula(){
		int ultimaPos=particulas.size()-1;
		return particulas[ultimaPos];
	}
	inline Vector3D getCenter(){
		Vector3D v;
		for(Solido* &s:particulas)
			v+=s->getPos();
		return v/particulas.size();
	}
	inline void glSetColor(Vector3D &col){
		double r=col.getX();
		double g=col.getY();
		double b=col.getZ();
	    glColor4f(r, g, b,0.9f);
	}
	inline void setF(Vector3D v){
		for(Solido *s:particulas)
			s->setF(v);
	}
	inline void render(){
	    glColor3f(1.0f, 1.0f, 0.0f);
	    //glBegin(GL_POINTS);
		for(Solido *p:particulas){
			p->render();
			//Vector3D pos1=p->getPos();
		    //glVertex3f(pos1.getX(),pos1.getY(),pos1.getZ());
		}
		//glEnd();
		int s=fuerzasElasticas.size();
		for(int i=0;i<s;i++){
			Solido *p1=fuerzasElasticas[i]->getParticula1();
			Solido *p2=fuerzasElasticas[i]->getParticula2();
			Vector3D pos1=p1->getPos();
			Vector3D pos2=p2->getPos();
			//Vector3D col=p1->getCol();
			//glSetColor(col);
			glColor3f(0,1,1);
		    glBegin(GL_LINES);
		      glVertex3f(pos1.getX(),pos1.getY(),pos1.getZ());
		      glVertex3f(pos2.getX(),pos2.getY(),pos2.getZ());
		    glEnd();
		}
	}
	inline void limpiaFuerza(){
		for(Solido *p:particulas)
			p->limpiaFuerza();
	}
	inline void acumulaFuerza(Vector3D f){
		for(Solido *p:particulas)
			p->acumulaFuerza(f);
	}
	inline void acumulaFuerzasElasticas(){
		for(FuerzaElastica *fe:fuerzasElasticas){
			Solido *p1=fe->getParticula1();
			Solido *p2=fe->getParticula2();
			Vector3D elastica=fe->evalua();
			p1->acumulaFuerza(elastica);
			p2->acumulaFuerza(-elastica);
			//fa.setP(p1);
			//p1->acumulaFuerza(fa.evalua());
			//fa.setP(p2);
			//p2->acumulaFuerza(fa.evalua());
		}
	}
	inline void updateParticles(double dt){
		for(Solido *p:particulas)
			p->update(dt);
	}
	inline void update(double dt){
		acumulaFuerzasElasticas();
		updateParticles(dt);
	}
	void calculaLongitudReposo(double s=1.0){
		for(FuerzaElastica *f:fuerzasElasticas){
			Solido *p1,*p2;
			p1=f->getParticula1();
			p2=f->getParticula2();
			Vector3D pos1=p1->getPos();
			Vector3D pos2=p2->getPos();
			double l=(pos1-pos2).length();
			f->setLongitudReposo(l*s);
		}
	}
};

#endif /* SOLIDO_ELASTICO_H_ */
