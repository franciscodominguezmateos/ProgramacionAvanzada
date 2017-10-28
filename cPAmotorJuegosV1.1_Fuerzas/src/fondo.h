/*
 * fondo.h
 *
 *  Created on: Oct 28, 2017
 *      Author: francisco
 */

#ifndef FONDO_H_
#define FONDO_H_
#include <GL/glut.h>
#include "vector3d.h"
class Fondo {
	Vector3D col;
public:
	Fondo():col(0,0.5,1){}
	virtual ~Fondo();
	inline void setCol(Vector3D c){col=c;}
	virtual void render(){
	    //R E N D ER
	    glMatrixMode(GL_MODELVIEW);
	    glClearColor(col.getX(),col.getY(),col.getZ(),0);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	}
};

#endif /* FONDO_H_ */
