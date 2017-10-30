/*
 * vista.h
 *
 *  Created on: Oct 28, 2017
 *      Author: Francisco Dominguez
 */

#ifndef VISTA_H_
#define VISTA_H_
#include <GL/glut.h>

class Vista {
	//relative values
	double x,y,w,h;
	//actual values in windows
	double px,py,width,height;
	int windowWidth, windowHeight;
public:
	Vista(double x=0,double y=0,double w=1,double h=1):x(x),y(y),w(w),h(h){}
	inline void reshape(int pwindowWidth,int pwindowHeight){
		windowWidth =pwindowWidth;
		windowHeight=pwindowHeight;
		px    =windowWidth*x;
		py    =windowHeight*y;
		width =windowWidth*w;
		height=windowHeight*h;
	}
	void render(){
	 glViewport(px,py,width,height);
	 glMatrixMode(GL_PROJECTION);
	 glLoadIdentity();
	 gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,200.0f);
	 glMatrixMode(GL_MODELVIEW);
	}
	bool contain(int x,int y){
		y=windowHeight-y;
		return x>=px && x<px+width && y>=py && y<py+height;
	}
};

#endif /* VISTA_H_ */
