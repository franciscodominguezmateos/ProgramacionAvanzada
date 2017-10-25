/*
 * compuesto.cpp
 *
 *  Created on: 3 Nov 2016
 *      Author: francisco
 */

#include "compuesto.h"

Compuesto::Compuesto() {
	// TODO Auto-generated constructor stub

}

Compuesto::~Compuesto() {
	 for(Solido *s:solidos){
		delete s;
	 }
	 solidos.clear();//no es necesario se llama en el destructor de vector
}

