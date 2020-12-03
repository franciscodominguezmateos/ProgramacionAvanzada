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

#include "solid.h"
#include "vector3d.h"

using namespace std;

class Composite:public Solid {
	vector<Solid*> solidos;
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
		for(Solid *s:c.solidos){
			solidos.push_back(s->clone());
		}
	}
	virtual ~Composite(){
		 for(Solid *s:solidos){
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
		for(Solid *s:solidos)
			s->render();
		glPopMatrix();
	}
	template<class T=Solid>
	vector<T*> getSolidos(){return solidos;}
	void add(Solid *s){solidos.push_back(s);}
};

#endif /* COMPOSITE_H_ */
