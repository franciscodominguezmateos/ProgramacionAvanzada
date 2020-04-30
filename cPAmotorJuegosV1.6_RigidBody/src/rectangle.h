/*
 * rectangulo.h
 *
 *  Created on: Oct 18, 2017
 *      Author: francisco
 */

#ifndef RECTANGLE_H_
#define RECTANGLE_H_

#include "texture.h"
#include "triangle.h"

class Rectangle: public Triangle {
	Vector3D p3;
	Texture tex;
	unsigned int nu;
	unsigned int nv;
public:
	Rectangle(Vector3D p0,Vector3D p1,Vector3D p2,Vector3D p3):Triangle(p0,p1,p2),p3(p3),nu(1),nv(1){}
	virtual ~Rectangle(){}
	Rectangle(const Rectangle &r):Triangle(r.p0,r.p1,r.p2),p3(r.p3),nu(1),nv(1){}
	virtual Rectangle *clone(){return new Rectangle(*this);}
	inline Vector3D getP3(){return p3;}
	inline Texture &getTex(){return tex;}
	inline void setTextura(Texture t){tex=t;}
	inline void setNU(unsigned int u){nu=u;}
	inline void setNV(unsigned int v){nv=v;}
	Triangle getTriangle0(){return *this;}
	Triangle getTriangle1(){return Triangle(getP0(),getP2(),getP3());}
	vector<Triangle> getTriangles(){
		vector<Triangle> vt;
		vt.push_back(getTriangle0());
		vt.push_back(getTriangle1());
		return vt;
	}
	void render(){	//Triangulo::render();
		Vector3D c=this->getCol();
		glColor3f(c.getX(),c.getY(),c.getZ());
		Vector3D vn=this->getNormal();
		float vnx=vn.getX();
		float vny=vn.getY();
		float vnz=vn.getZ();
		tex.activar();
		glBegin(GL_QUADS);
			glTexCoord2f(0, 0);
		    glNormal3f(vnx,vny,vnz);
		    glVertex3f(p0.getX(),p0.getY(),p0.getZ());
			glTexCoord2f(nu, 0);
		    glNormal3f(vnx,vny,vnz);
		    glVertex3f(p1.getX(),p1.getY(),p1.getZ());
			glTexCoord2f(nu, nv);
		    glNormal3f(vnx,vny,vnz);
		    glVertex3f(p2.getX(),p2.getY(),p2.getZ());
			glTexCoord2f(0, nv);
		    glNormal3f(vnx,vny,vnz);
		    glVertex3f(p3.getX(),p3.getY(),p3.getZ());
		glEnd();
		tex.desactivar();
	}
};

#endif /* RECTANGLE_H_ */
