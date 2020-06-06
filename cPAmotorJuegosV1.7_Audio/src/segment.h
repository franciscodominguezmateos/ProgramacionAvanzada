/*
 * segment.h
 *
 *  Created on: 15 May 2020
 *      Author: Francisco Dominguez
 */
#pragma once
#include "line.h"
class Segment:public Line {
	Vector3D p1;
	float length;
public:
	Segment(Vector3D p0,Vector3D p1):Line(p0,p1),p1(p1){length=l.length();}
	inline bool contains(Vector3D q){
		if(Line::contains(q)){
			Vector3D d=q-p;
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
};
