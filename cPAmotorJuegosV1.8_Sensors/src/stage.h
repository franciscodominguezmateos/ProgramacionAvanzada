/*
 * escena.h
 *
 *  Created on: 18 Oct 2016
 *      Author: Francisco Dominguez
 *  Lights are the first think to add to a stage
 */

#ifndef STAGE_H_
#define STAGE_H_
#include <vector>
#include "vector3d.h"
#include "luz.h"
#include "solid.h"
using namespace std;

class Stage {
	vector<Solid*> solidos;
public:
	Stage(){}
	Stage(const Stage &e){
		for(Solid *s:e.solidos){
			solidos.push_back(s->clone());
		}
	}
	virtual ~Stage(){
		for(Solid *s:solidos){
			delete s;
		}
		solidos.clear();//no es necesario se llama en el destructor de vector
	}
	void render(){
		for(Solid *s:solidos)
			s->render();
	}
	void limpiaFuerzas(){
		for(Solid *s:solidos)
			s->limpiaFuerza();
	}
	void update(double dt){
		limpiaFuerzas();
		for(Solid *s:solidos){
			s->acumulaFuerza(Vector3D(0,-9.8/1.0,0));
			s->update(dt);
		}
	}
	vector<Solid*> getSolidos(){return solidos;}
	Solid* getLastSolido(){int last=solidos.size()-1;return solidos[last];}
	void setLastSolido(Solid *s){int last=solidos.size()-1;solidos[last]=s;}
	void add(Solid *s){solidos.push_back(s);}
	friend std::ostream &operator << (std::ostream &os, const Stage &v);
};
inline std::ostream &operator<<(std::ostream &os, const Stage &e){
	for(Solid *s:e.solidos){
		os << *s <<endl;
	}
    return os;
}
#endif /* STAGE_H_ */
