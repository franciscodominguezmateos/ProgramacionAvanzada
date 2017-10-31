/*
 * proyeccion.h
 *
 *  Created on: Oct 31, 2017
 *      Author: francisco
 */

#ifndef PROYECCION_H_
#define PROYECCION_H_
#include <GL/glut.h>
class Proyeccion {
public:
	Proyeccion();
	virtual ~Proyeccion();
	virtual void render()=0;
};

#endif /* PROYECCION_H_ */
