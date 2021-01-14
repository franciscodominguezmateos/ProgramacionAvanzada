/*
 * vista.h
 *
 *  Created on: Oct 28, 2017
 *      Author: Francisco Dominguez
 */

#pragma once
#include <GL/glut.h>
#include "projection_perspective.h"
#include "projection.h"
class View;
using ViewPtr=View*;
class View {
	//relative values
	double x,y,w,h;
	//actual values in windows
	double px,py,width,height;
	int windowWidth, windowHeight;
	ProjectionPerspective *p;
public:
	View(double x=0,double y=0,double w=1,double h=1,ProjectionPerspective* p=nullptr):
		x(x),y(y),w(w),h(h),
		px(0),py(0),width(w*640),height(h*480),
		windowWidth(640),windowHeight(480),
		p(p){
		width=w*(double)windowWidth;
		height=h*(double)windowHeight;
		if(p==nullptr)
			this->p=new ProjectionPerspective(60);
	}
	inline void setProyeccion(ProjectionPerspective *py){p=py;}
	inline ProjectionPerspective* getProjection(){return p;}
	inline void reshape(int pwindowWidth,int pwindowHeight){
		windowWidth =pwindowWidth;
		windowHeight=pwindowHeight;
		px    =windowWidth *x;
		py    =windowHeight*y;
		width =windowWidth *w;
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
