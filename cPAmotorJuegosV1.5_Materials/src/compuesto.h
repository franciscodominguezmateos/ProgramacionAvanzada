/*
 * compuesto.h
 *
 *  Created on: 3 Nov 2016
 *      Author: francisco
 */

#ifndef COMPUESTO_H_
#define COMPUESTO_H_
#include <iostream>
#include <vector>
#include <GL/glut.h>
#include "vector3d.h"
#include "solido.h"

using namespace std;

class Compuesto:public Solido {
	vector<Solido*> solidos;
public:
	Compuesto();
	Compuesto(const Compuesto &c){
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
	virtual ~Compuesto();
	Compuesto *clone(){
		return new Compuesto(*this);
	}
	void render(){
		glPushMatrix();
		//glColor3f(this->getCol().getX(),this->getCol().getY(),this->getCol().getZ());
		glTranslatef(this->getPos().getX(),this->getPos().getY(),this->getPos().getZ());
		for(Solido *s:solidos)
			s->render();
		glPopMatrix();
	}
	vector<Solido*> getSolidos(){return solidos;}
	void add(Solido *s){solidos.push_back(s);}
};

#endif /* COMPUESTO_H_ */
