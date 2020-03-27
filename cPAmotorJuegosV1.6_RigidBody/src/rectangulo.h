/*
 * rectangulo.h
 *
 *  Created on: Oct 18, 2017
 *      Author: francisco
 */

#ifndef RECTANGULO_H_
#define RECTANGULO_H_

#include "textura.h"
#include "triangle.h"

class Rectangulo: public Triangle {
	Vector3D p3;
	Textura tex;
	unsigned int nu;
	unsigned int nv;
public:
	Rectangulo(Vector3D p0,Vector3D p1,Vector3D p2,Vector3D p3):Triangle(p0,p1,p2),p3(p3),nu(1),nv(1){}
	virtual ~Rectangulo(){}
	Rectangulo(const Rectangulo &r):Triangle(r.p0,r.p1,r.p2),p3(r.p3),nu(1),nv(1){}
	virtual Rectangulo *clone(){return new Rectangulo(*this);}
	inline Vector3D getP3(){return p3;}
	inline Textura &getTex(){return tex;}
	inline void setTextura(Textura t){tex=t;}
	inline void setNU(unsigned int u){nu=u;}
	inline void setNV(unsigned int v){nv=v;}
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

#endif /* RECTANGULO_H_ */
