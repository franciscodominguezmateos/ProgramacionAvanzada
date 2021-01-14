/*
 * compuesto.h
 *
 *  Created on: 3 Nov 2016
 *      Author: francisco
 */
#pragma once
#include <vector>
#include "solid.h"

class Composite;
using CompositePtr=Composite*;

class Composite:public Solid {
	vector<SolidPtr> solids;
public:
	Composite(){}
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
		for(Solid *s:c.solids){
			solids.push_back(s->clone());
		}
	}
	virtual ~Composite(){
		 for(Solid *s:solids){
			delete s;
		 }
		 solids.clear();//no es necesario se llama en el destructor de vector
	}
	CompositePtr clone(){
		return new Composite(*this);
	}
	void render(){
		glPushMatrix();
		//glColor3f(this->getCol().getX(),this->getCol().getY(),this->getCol().getZ());
		glTranslatef(this->getPos().getX(),this->getPos().getY(),this->getPos().getZ());
		for(Solid *s:solids)
			s->render();
		glPopMatrix();
	}
	template<class T=Solid>
	vector<T*> getSolids(){return solids;}
	SolidPtr &getSolid(int i){return solids[i];}
	SolidPtr &operator[](int i){return solids[i];}
	void add(Solid *s){solids.push_back(s);}
	int size(){return solids.size();}
};
