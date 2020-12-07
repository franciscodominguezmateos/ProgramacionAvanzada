/*
 * arrow.h
 *
 *  Created on: 1 Dec 2020
 *      Author: Francisco Dominguez
 */
#pragma once
#include "solid.h"
#include "text.h"

class Arrow: public Solid {
	GLUquadricObj *quadratic;
	double b,h;
	double tipB,tipH;
	Text name;
public:
	Arrow(double b=0.05,double h=1):Solid(),b(b),h(h),tipB(b*1.5),tipH(0.2),name("Arrow"){
		 quadratic=gluNewQuadric();
		 hazFija();
		 name.setPos(Vector3D(b*1.5,0,h/2));
	}
	Arrow(Vector3D v,double b=0.05):Arrow(b,v.length()){
		Vector3D vn=v/h;
		//The arrow is default Z-oriented
		static const Vector3D Z(0,0,1);
		//Vector3D w=vn.X(Z);
		Vector3D w=Z.X(vn);
		double nw=w.norm();
		if(nearZero(nw)){
			double d=vn*Z;
			if(d>0)
				//It doesn't need to rotate
			    return;
			else //rotate 180
				setRot(Vector3D(180,0,0));
		}
		double a=asin(nw);
		Vector3D wn=w/nw;
		Mat R=exp(wn*a);
		setRot(rotationMatrixToEulerAngles(R));
	}
	Arrow(Vector3D v,Vector3D p):Arrow(v){
		setPos(p);
	}
	Arrow(const Arrow &other):Solid(other),b(other.b),h(other.h),tipB(other.b*1.1),tipH(other.h*0.1){
		 quadratic=gluNewQuadric();
	}
	void render(){
		glPushMatrix();
		    glColor3f(this->getCol().getX(),this->getCol().getY(),this->getCol().getZ());
		    glTranslatef(this->getPos().getX(),this->getPos().getY(),this->getPos().getZ());
			glRotatef(getRot().getX(), 1, 0, 0);
			glRotatef(getRot().getY(), 0, 1, 0);
			glRotatef(getRot().getZ(), 0, 0, 1);
		    gluCylinder(quadratic,b,b,h-tipH,16,16);
		    name.render();
		    glTranslatef(0,0,h-tipH);
		    gluCylinder(quadratic,tipB,0,tipH,16,16);
		glPopMatrix();
	}

	double getB() const {return b;	}
	void setB(double b) {this->b = b;	}
	double getH() const {return h;	}
	void setH(double h) {this->h = h;	}
	const Text& getName() const {return name;	}
	void setName(const Text &name) {
		this->name = name;
		this->name.setPos(Vector3D(b*1.5,0,h/2));
	}
	double getTipB() const {return tipB;}
	void setTipB(double tipB) {	this->tipB = tipB;	}
	double getTipH() const {	return tipH;}
	void setTipH(double tipH) {	this->tipH = tipH;	}
};
