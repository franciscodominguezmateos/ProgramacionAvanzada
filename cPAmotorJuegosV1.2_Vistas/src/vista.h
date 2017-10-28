/*
 * vista.h
 *
 *  Created on: Oct 28, 2017
 *      Author: francisco
 */

#ifndef VISTA_H_
#define VISTA_H_
#include <GL/glut.h>

class Vista {
	//relative values
	double x,y,w,h;
	//actual values in windows
	double px,py,width,height;
public:
	Vista(double x=0,double y=0,double w=1,double h=1):x(x),y(y),w(w),h(h){}
	inline void reshape(int windowWidth,int windowHeight){
		px    =windowWidth*x;
		py    =windowHeight*y;
		width =windowWidth*w;
		height=windowHeight*h;
	}
	void render(){
	 width;
	 height;
	 glViewport(px,py,width,height);
	 glMatrixMode(GL_PROJECTION);
	 glLoadIdentity();
	 gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,200.0f);
	 glMatrixMode(GL_MODELVIEW);
	}
};

#endif /* VISTA_H_ */
