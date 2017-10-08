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
#include "cubo.h"
#include "esfera.h"

using namespace std;

class Escena {
	vector<Cubo> cubos;
	vector<Esfera> esferas;
public:
	Escena();
	virtual ~Escena();
	void render(){
	 for(Cubo c:cubos)
	  c.render();
	 for(Esfera e:esferas)
	  e.render();
	}
	void update(double dt){
		for(unsigned int i=0;i<cubos.size();i++){
			cubos[i].update(dt);
		}
		for(unsigned int i=0;i<esferas.size();i++){
			esferas[i].update(dt);
		}
	}
	vector<Cubo> getCubos(){return cubos;}
	vector<Esfera> getEsferas(){return esferas;}
	void add(Cubo c){cubos.push_back(c);}
	void add(Esfera e){esferas.push_back(e);}
};

#endif /* ESCENA_H_ */
