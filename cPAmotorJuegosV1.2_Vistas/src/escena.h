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
	vector<Luz*> luces;
	vector<Solido*> solidos;
public:
	Escena();
	Escena(const Escena &e){
		for(Solido *s:e.solidos){
			solidos.push_back(s->clone());
		}
	}
	virtual ~Escena();
	void render(){
		for(Luz *l:luces)
			l->render();
		for(Solido *s:solidos)
			s->render();
	}
	void update(double dt){
		 for(Solido *s:solidos){
			 s->setF(Vector3D(0,-9.8/1,0));
			 s->update(dt);
		 }
	}
	vector<Solido*> getSolidos(){return solidos;}
	void add(Solido *s){solidos.push_back(s);}
	void add(Luz *l){luces.push_back(l);}
	friend std::ostream &operator << (std::ostream &os, const Escena &v);
};
inline std::ostream &operator<<(std::ostream &os, const Escena &e){
	for(Solido *s:e.solidos){
		os << *s <<endl;
	}
    return os;
}
#endif /* ESCENA_H_ */
