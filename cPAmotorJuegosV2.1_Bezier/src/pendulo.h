/*
 * pendulo.h
 *
 *  Created on: Oct 21, 2017
 *      Author: francisco
 */

#ifndef PENDULO_H_
#define PENDULO_H_
#include "solido_elastico.h"

class Pendulo:public SolidoElastico {
public:
	Pendulo(){}
	Pendulo(Solid *p1,Solid *p2){
		p1->hazFija();
		FuerzaElastica *f=new FuerzaElastica();
		f->setParticula1(p1);
		f->setParticula2(p2);
		this->add(p1);
		this->add(p2);
		this->add(f);
		this->calculaLongitudReposo();
	}
	virtual ~Pendulo(){}
};

#endif /* PENDULO_H_ */
