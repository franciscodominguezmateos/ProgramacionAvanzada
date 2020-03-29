/*
 * vista.h
 *
 *  Created on: Oct 28, 2017
 *      Author: Francisco Dominguez
 */

#ifndef VIEW_H_
#define VIEW_H_
#include <GL/glut.h>
#include "proyeccion.h"
#include "proyeccion_perspectiva.h"
class View {
	//relative values
	double x,y,w,h;
	//actual values in windows
	double px,py,width,height;
	int windowWidth, windowHeight;
	ProyeccionPerspectiva *p;
public:
	View(double x=0,double y=0,double w=1,double h=1,ProyeccionPerspectiva *p=nullptr):
		x(x),y(y),w(w),h(h),
		px(0),py(0),width(w*640),height(h*480),
		windowWidth(640),windowHeight(480),
		p(p){
		width=w*(double)windowWidth;
		height=h*(double)windowHeight;
	}
	inline void setProyeccion(ProyeccionPerspectiva *py){p=py;}
	inline void reshape(int pwindowWidth,int pwindowHeight){
		windowWidth =pwindowWidth;
		windowHeight=pwindowHeight;
		px    =windowWidth*x;
		py    =windowHeight*y;
		width =windowWidth*w;
		height=windowHeight*h;
		p->reshape(width,height);
	}
	void render(){
	 glViewport(px,py,width,height);
	 p->render();
	}
	bool contain(int x,int y){
		y=windowHeight-y;
		return x>=px && x<px+width && y>=py && y<py+height;
	}
};

#endif /* VIEW_H_ */
