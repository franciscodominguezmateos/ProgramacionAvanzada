/*
 * fondo.h
 *
 *  Created on: Oct 28, 2017
 *      Author: francisco
 */

#ifndef BACKGROUND_H_
#define BACKGROUND_H_
#include <GL/glut.h>
#include "vector3d.h"

class Background;
using BackgroundPtr=Background*;
class Background {
	Vector3D col;
public:
	Background():col(0,0.5,1){}
	virtual ~Background(){}
	inline void setCol(Vector3D c){col=c;}
	virtual void render(){
	    //R E N D ER
	    glMatrixMode(GL_MODELVIEW);
	    glClearColor(col.getX(),col.getY(),col.getZ(),0);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	}
};

#endif /* BACKGROUND_H_ */
