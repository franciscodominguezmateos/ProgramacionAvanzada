/*
 * escena.cpp
 *
 *  Created on: 18 Oct 2016
 *      Author: francisco
 */

#include "escena.h"

Escena::Escena() {
	// TODO Auto-generated constructor stub

}

Escena::~Escena() {
	for(Luz *l:luces)
		delete l;
	luces.clear();
	for(Solido *s:solidos){
		delete s;
	}
	solidos.clear();//no es necesario se llama en el destructor de vector
}

