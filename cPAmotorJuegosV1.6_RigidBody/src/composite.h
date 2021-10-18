/*
 * compuesto.h
 *
 *  Created on: 3 Nov 2016
 *      Author: francisco
 */

#ifndef COMPOSITE_H_
#define COMPOSITE_H_
#include <iostream>
#include <vector>
#include <GL/glut.h>
#include "vector3d.h"
#include "solido.h"

using namespace std;

class Composite:public Solido {
	vector<Solido*> solids;
public:
	Composite();
	Composite(const Composite &c){
		//cout<<"dentro de cc"<<endl;
		//Copia plana
		//solidos=c.solidos;
		//Copia Plana engañosa
		//por que parece profunca
		//for(Solido *s:c.solidos){
		//	solidos.push_back(s);
		//}
		//Copia profunda
		for(Solido *s:c.solids){
			solids.push_back(s->clone());
		}
	}
	virtual ~Composite(){
		 for(Solido *s:solids){
			delete s;
		 }
		 solids.clear();//no es necesario se llama en el destructor de vector
	}
	Composite *clone(){
		return new Composite(*this);
	}
	void render(){
		glPushMatrix();
		//glColor3f(this->getCol().getX(),this->getCol().getY(),this->getCol().getZ());
		glTranslatef(this->getPos().getX(),this->getPos().getY(),this->getPos().getZ());
		for(Solido *s:solids)
			s->render();
		glPopMatrix();
	}
	template<class T=Solido>
	vector<T*> getSolids(){return solids;}
	void add(Solido *s){solids.push_back(s);}
};

#endif /* COMPOSITE_H_ */
