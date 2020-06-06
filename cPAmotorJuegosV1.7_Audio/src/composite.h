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
	vector<Solido*> solidos;
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
		for(Solido *s:c.solidos){
			solidos.push_back(s->clone());
		}
	}
	virtual ~Composite(){
		 for(Solido *s:solidos){
			delete s;
		 }
		 solidos.clear();//no es necesario se llama en el destructor de vector
	}
	Composite *clone(){
		return new Composite(*this);
	}
	void render(){
		glPushMatrix();
		//glColor3f(this->getCol().getX(),this->getCol().getY(),this->getCol().getZ());
		glTranslatef(this->getPos().getX(),this->getPos().getY(),this->getPos().getZ());
		for(Solido *s:solidos)
			s->render();
		glPopMatrix();
	}
	template<class T=Solido>
	vector<T*> getSolidos(){return solidos;}
	void add(Solido *s){solidos.push_back(s);}
};

#endif /* COMPOSITE_H_ */
