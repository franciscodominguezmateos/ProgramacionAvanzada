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
using namespace std;

class Escena {
	vector<Solido*> solidos;
public:

	Escena();
	virtual ~Escena();

	void render(){
	 for(Solido *s:solidos)
	  s->render();
	}
	void update(float dt){
		 for(Solido *s:solidos)
		  s->update(dt);
	}
	vector<Solido*> getSolidos(){return solidos;}
	void add(Solido *s){solidos.push_back(s);}
};

#endif /* ESCENA_H_ */
