/*
 * segment.h
 *
 *  Created on: 15 May 2020
 *      Author: Francisco Dominguez
 */
#pragma once
#include "line.h"
class Segment:public Line {
	Vector3D p0,p1;
	float length;
public:
	Segment(Vector3D p0,Vector3D p1):Line(p0,p1),p0(p0),p1(p1){length=l.length();}
	Mat asMatHomogeneous(){
		Mat pts=Mat::zeros(4,2,CV_64F);
		Mat m=Line::asMatHomogeneous();
		Mat M0 = pts.col(0);
		Mat M1 = pts.col(1);
		p0.asH().copyTo(M0);
		p1.asH().copyTo(M1);
		hconcat(pts,m,pts);
		return pts;
	}
	inline bool contains(Vector3D q){
		if(Line::contains(q)){
			Vector3D d=q-p0;
			// NONO
			if(d.length()<length)
				return true;
			return false;
		}
		return false;
	}
	inline bool intersect(Segment s){
		if(Line::intersect(s)){
			try{
				Vector3D q=intersectionPoint(s);
				if(this->contains(q) && s.contains(q)){
					return true;
				}
				return false;
			}
			catch(runtime_error &e){
				//cout << "Error in Segment::intersect : " << e.what() <<endl;
				return false;
			}
		}
		return false;
	}
	inline Vector3D lerp(double t){
		if(t<0 || t>1) throw runtime_error("Segment::ler() param must be from 0 to 1");
		Vector3D r=p0*(1-t)+p1*t;
		return r;
	}
	void render(){
		Vector3D c=this->getCol();
		glColor3f(c.getX(),c.getY(),c.getZ());
		glLineWidth(3);
		glBegin(GL_LINES);
		  glVertex3f(p0.getX(),p0.getY(),p0.getZ());
		  glVertex3f(p1.getX(),p1.getY(),p1.getZ());
		glEnd();
	}
};
