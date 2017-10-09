/*
 * cilindro.h
 *
 *  Created on: 25 Oct 2016
 *      Author: francisco
 */

#ifndef CILINDRO_H_
#define CILINDRO_H_
#include <GL/glut.h>
#include "vector3d.h"
#include "solido.h"
class Cilindro: public Solido {
	GLUquadricObj *quadratic;
	float b;
	float h;
public:
	Cilindro(float bp=1,float hp=1);
	virtual ~Cilindro();
	void render();
};

#endif /* CILINDRO_H_ */
