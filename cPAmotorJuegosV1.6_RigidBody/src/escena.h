/*
 * escena.h
 *
 *  Created on: 18 Oct 2016
 *      Author: francisco
 */

#ifndef ESCENA_H_
#define ESCENA_H_
#include <vector>
#include "vector3d.h"
#include "solido.h"
#include "luz.h"
using namespace std;

class Escena {
	vector<Solido*> solidos;
public:
	Escena(){}
	Escena(const Escena &e){
		for(Solido *s:e.solidos){
			solidos.push_back(s->clone());
		}
	}
	virtual ~Escena(){
		for(Solido *s:solidos){
			delete s;
		}
		solidos.clear();//no es necesario se llama en el destructor de vector
	}
	void render(){
		for(Solido *s:solidos)
			s->render();
	}
	void limpiaFuerzas(){
		for(Solido *s:solidos)
			s->limpiaFuerza();
	}
	void update(double dt){
		limpiaFuerzas();
		for(Solido *s:solidos){
			s->acumulaFuerza(Vector3D(0,-9.8/1.0,0));
			s->update(dt);
		}
	}
	vector<Solido*> getSolidos(){return solidos;}
	Solido* getLastSolido(){int last=solidos.size()-1;return solidos[last];}
	void setLastSolido(Solido *s){int last=solidos.size()-1;solidos[last]=s;}
	void add(Solido *s){solidos.push_back(s);}
	friend std::ostream &operator << (std::ostream &os, const Escena &v);
};
inline std::ostream &operator<<(std::ostream &os, const Escena &e){
	for(Solido *s:e.solidos){
		os << *s <<endl;
	}
    return os;
}
#endif /* ESCENA_H_ */
