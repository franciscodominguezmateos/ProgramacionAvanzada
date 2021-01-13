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
	Rectangle(Vector3D p0,Vector3D p1,Vector3D p2,Vector3D p3):Triangle(p0,p1,p2),p3(p3),nu(1),nv(1){
		hazFija();
	}
	virtual ~Rectangle(){}
	Rectangle(const Rectangle &r):Triangle(r.p0,r.p1,r.p2),p3(r.p3),nu(1),nv(1){}
	virtual Rectangle *clone(){return new Rectangle(*this);}
	Mat asMatHomogeneous(){
		Mat pts=Mat::zeros(4,6,CV_64F);
		Mat M0 = pts.col(0);
		Mat M1 = pts.col(1);
		Mat M2 = pts.col(2);
		Mat M3 = pts.col(3);
		Mat M4 = pts.col(4);
		Mat M5 = pts.col(5);
		p0.asH().copyTo(M0);
		p1.asH().copyTo(M1);
		p2.asH().copyTo(M2);
		p3.asH().copyTo(M3);
		getNormal().asH0().copyTo(M4);
		getCenter().asH().copyTo(M5);
		return pts;
	}
	inline Vector3D getP3(){return p3;}
	inline Texture &getTex(){return tex;}
	inline Vector3D getCenter(){
		Vector3D vCenter=(p0+p1+p2+p3)/4;
		return vCenter;
	}
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
	  glPushMatrix();
		glTranslatef(getPos().getX(),getPos().getY(),getPos().getZ());
		glRotatef(getRot().getX(), 1, 0, 0);
		glRotatef(getRot().getY(), 0, 1, 0);
		glRotatef(getRot().getZ(), 0, 0, 1);
		Vector3D c=this->getCol();
		glColor3f(c.getX(),c.getY(),c.getZ());
		Vector3D vn=this->getNormal();
		float vnx=vn.getX();
		float vny=vn.getY();
		float vnz=vn.getZ();
		tex.bind();
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
		tex.unbind();
	    if(drawNormals){
			//Draw normal
	    	Vector3D vn=this->getNormal();
	    	Vector3D center=getCenter();//
			glColor3f(1,0,0);
			glLineWidth(3);
			glBegin(GL_LINES);
			  Vector3D up=center+vn;
			  glVertex3f(center.getX(),center.getY(),center.getZ());
			  glVertex3f(up.getX(),up.getY(),up.getZ());
			glEnd();
			glColor3f(1,0,0);
			glBegin(GL_LINE_LOOP);
			  glVertex3f(p0.getX(),p0.getY(),p0.getZ());
			  glVertex3f(p1.getX(),p1.getY(),p1.getZ());
			  glVertex3f(p2.getX(),p2.getY(),p2.getZ());
			  glVertex3f(p3.getX(),p3.getY(),p3.getZ());
			glEnd();
		    //Triangle t=getTriangle1();
		    //t.setDrawNormals();
		    //t.render();
	    }
	  glPopMatrix();
	}
};

#endif /* RECTANGLE_H_ */
