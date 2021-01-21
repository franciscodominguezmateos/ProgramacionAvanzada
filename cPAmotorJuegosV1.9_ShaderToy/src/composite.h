/*
 * compuesto.h
 *
 *  Created on: 3 Nov 2016
 *      Author: francisco
 */
#pragma once
#include <vector>
#include <algorithm>
#include "solid.h"

class Composite;
using CompositePtr=Composite*;

class Composite:public Solid {
	vector<SolidPtr> solids;
public:
	Composite(){}
	Composite(const Composite &c){
		//deep copy
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
	vector<SolidPtr> &getSolids(){return solids;}
	int size(){return solids.size();}
	SolidPtr &getSolid(int i){return solids[i];}
	SolidPtr &operator[](int i){return solids[i];}
	SolidPtr getLastSolido(){int last=solids.size()-1;return solids[last];}
	void setLastSolido(SolidPtr s){int last=solids.size()-1;solids[last]=s;}
	void add(SolidPtr s){solids.push_back(s);}
	void remove(SolidPtr sp){
		std::remove(solids.begin(),solids.end(),sp);
		delete sp;
	}
};
/*inline std::ostream &operator<<(std::ostream &os, const Composite &e){
	for(SolidPtr s:e.solids){
		os << *s <<endl;
	}
    return os;
}*/

