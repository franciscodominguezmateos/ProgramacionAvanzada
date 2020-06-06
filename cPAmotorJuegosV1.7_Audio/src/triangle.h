/*
 * triangulo.h
 *
 *  Created on: Nov 12, 2015
 *      Author: Francisco Dominguez
 */
#ifndef TRIANGLE_H_
#define TRIANGLE_H_
#include <iostream>
#include <GL/glut.h>
#include "plane.h"
#include "vector3d.h"
#include "texture.h"

using namespace std;

class Triangle: public Plane {
protected:
	Vector3D p0,p1,p2;
	Vector3D n0,n1,n2;
	Vector3D t0,t1,t2;
	Vector3D v01,v12,v20;
	//Care are to be taken
	//Texture are outside the object
	Texture* textura;
	double area;
	bool drawNormals;
public:
	Triangle(Vector3D p0,Vector3D p1,Vector3D p2):
		Plane(),
		p0(p0),p1(p1),p2(p2),
        v01(p1-p0),v12(p2-p1),v20(p0-p2),
		textura(nullptr),
		drawNormals(false){
		init();
	}
	// after changing p0, p1 or p2 this method should be called
	void init(){
        v01=p1-p0;
        v12=p2-p1;
        v20=p0-p2;

		// VERY IMPORTANT triangles points order is uncounterclock
		Vector3D vn=v01.X(v12);
		//Area of a triangle is half the length of the cross product
		area=vn.length()/2;
		try{
			vn.normalize();
		}
		catch (exception &e){
			//not normalize
			//cout << "not normalizzing at Triangle constructor"<< endl;
			/*
			cout << "p0="<<p0<<endl;
			cout << "p1="<<p1<<endl;
			cout << "p2="<<p2<<endl;
			cout << "vn="<<vn<<endl;
			cout << "v01="<<v01<<endl;
			cout << "v12="<<v12<<endl;
			cout << "v20="<<v20<<endl;*/
			throw runtime_error(" in Triangle::Triangle() :"+string(e.what()));
		}
		float d=vn*p2;
		this->setA(vn.getX());
		this->setB(vn.getY());
		this->setC(vn.getZ());
		this->setD(-d);
		//center of the triangle is pos field
		this->setPos((p0+p1+p2)/3);
	}
	virtual Triangle *clone(){return new Triangle(*this);}
	inline Vector3D &getP0(){return p0;}
	inline Vector3D &getP1(){return p1;}
	inline Vector3D &getP2(){return p2;}
	inline Vector3D &getN0(){return n0;}
	inline Vector3D &getN1(){return n1;}
	inline Vector3D &getN2(){return n2;}
	inline Vector3D &getT0(){return t0;}
	inline Vector3D &getT1(){return t1;}
	inline Vector3D &getT2(){return t2;}
	inline double &getArea(){return area;}
	inline void setP0(Vector3D &n){p0=n;}
	inline void setP1(Vector3D &n){p1=n;}
	inline void setP2(Vector3D &n){p2=n;}
	inline void setN0(Vector3D &n){n0=n;}
	inline void setN1(Vector3D &n){n1=n;}
	inline void setN2(Vector3D &n){n2=n;}
	inline void setT0(Vector3D &n){t0=n;}
	inline void setT1(Vector3D &n){t1=n;}
	inline void setT2(Vector3D &n){t2=n;}
	inline void setTextura(Texture* &t){textura=t;}
	inline Vector3D getCenter(){return getPos();}
	inline void setDrawNormals(bool b=true){drawNormals=b;}
	inline void doScale(double s){
		p0*=s;
		p1*=s;
		p2*=s;
		init();
	}
	inline void doTranslate(Vector3D &t){
		p0+=t;
		p1+=t;
		p2+=t;
		init();
	}
	inline void doRotate(Vector3D av){
		Mat r=eulerAnglesToRotationMatrix(av);
		Mat m0=r*p0.asMat();
		p0=asVector3D(m0);
		Mat m1=r*p1.asMat();
		p1=asVector3D(m1);
		Mat m2=r*p2.asMat();
		p2=asVector3D(m2);
		Mat mn0=r*n0.asMat();
		n0=asVector3D(mn0);
		Mat mn1=r*n1.asMat();
		n1=asVector3D(mn1);
		Mat mn2=r*n2.asMat();
		n2=asVector3D(mn2);
		init();
	}
	virtual ~Triangle(){}
	void render(){
		Vector3D c=this->getCol();
		glColor3f(c.getX(),c.getY(),c.getZ());
		if(textura!=nullptr) textura->activar();
		glBegin(GL_TRIANGLES);
		    glTexCoord2f(t0.getX(),t0.getY());
		    //glNormal3d(vnx,vny,vnz);
		    glNormal3d(n0.getX(),n0.getY(),n0.getZ());
		    glVertex3d(p0.getX(),p0.getY(),p0.getZ());

		    glTexCoord2f(t1.getX(),t1.getY());
		    //glNormal3d(vnx,vny,vnz);
		    glNormal3d(n1.getX(),n1.getY(),n1.getZ());
		    glVertex3d(p1.getX(),p1.getY(),p1.getZ());

		    glTexCoord2f(t2.getX(),t2.getY());
		    //glNormal3d(vnx,vny,vnz);
		    glNormal3d(n2.getX(),n2.getY(),n2.getZ());
		    glVertex3d(p2.getX(),p2.getY(),p2.getZ());
	    glEnd();
	    if(drawNormals){
			//Draw normal
	    	Vector3D vn=this->getNormal();
	    	// triangle position is the center of the triangle
	    	Vector3D center=getPos();
			glColor3f(1.0,0,0);
			glLineWidth(2);
			glBegin(GL_LINES);
			  Vector3D up=center+vn;
			  glVertex3f(center.getX(),center.getY(),center.getZ());
			  glVertex3f(up.getX(),up.getY(),up.getZ());
			glEnd();
			glColor3f(0,0,1);
			glBegin(GL_LINE_LOOP);
			  glVertex3f(p0.getX(),p0.getY(),p0.getZ());
			  glVertex3f(p1.getX(),p1.getY(),p1.getZ());
			  glVertex3f(p2.getX(),p2.getY(),p2.getZ());
			glEnd();
	    }
	    if(textura!=nullptr) textura->desactivar();
	}
	bool contact(Vector3D pt){
		if(nearZero(distance(pt)))
			return isOver(pt);
		return false;
	}
	// I will use double check:
	// - Area check
	// - Normal check
	bool isOver(Vector3D pt){
		// Project pt in triangle plane
		Vector3D p=project(pt);
		// p should be in the plane
		// then this should be always true
		double d=distance(p);
		assert(nearZero(d));

		//accumulated areas
		double a=0;
		Vector3D vn;
		try {
			vn=getNormal();
		}
		catch(runtime_error &e) {
			throw runtime_error(" in Triangle::isOver:"+string(e.what()));
		}

			// C H E C K I N G Triangle(p0,p1,p)
		try{
			Triangle t01=Triangle(p0,p1,p);
			//t01.area should be minor than triangle area
			a=t01.getArea();
			if(a>area)
				return false;
			double d0p=t01.getNormal()*vn;
			//On the other hand  if p is out of triangle then dot product with plane normal should be negative
			if(d0p<0)
				return false;
		}
		catch(runtime_error &e) {
			//throw runtime_error(" in Triangle::isOver:Triangle(p0,p1,p) "+string(e.what()));
		}

			// C H E C K I N G Triangle(p1,p2,p)
		try{
			Triangle t12=Triangle(p1,p2,p);
			//t01.area+t12.area should be minor than triangle area
			a+=t12.getArea();
			if(a>area)
				return false;
			double d1p=t12.getNormal()*vn;
			//On the other hand If p is out of triangle then dot product with plane normal should be negative
			if(d1p<0)
				return false;
		}
		catch(runtime_error &e) {
			//throw runtime_error(" in Triangle::isOver:Triangle(p1,p2,p) "+string(e.what()));
		}

			// C H E C K I N G Triangle(p2,p0,p)
		try{
			Triangle t20=Triangle(p2,p0,p);
			//t01.area+t12.area+t20.area should be EQUAL than triangle area
			a+=t20.getArea();
			if(!nearZero(a-area))
				return true;
			double d2p=t20.getNormal()*vn;
			//On the other hand if p is out of triangle then dot product with plane normal should be negative
			if(d2p<0)
				return false;
		}
		catch(runtime_error &e) {
			//throw runtime_error(" in Triangle::isOver:Triangle(p2,p0,p) "+string(e.what()));
		}
		//Here we have double check that p is in the triangle
		return true;
	}
};

#endif /* TRIANGLE_H_ */
