/*
 * line.h
 *
 *  Created on: 15 May 2020
 *      Author: Francisco Dominguez
 */
#pragma once
#include "vector3d.h"
class Line {
	Vector3D p,v;
public:
	Line(Vector3D p0,Vector3D p1):p(p0),v((p1-p0)){v.normalize();}
    //Projection of point in line
	Vector3D project(Vector3D q){
		// v is normalized
		Vector3D qp=p-q;
		float proj=v*qp;
		Vector3D projection=proj*v;
		return projection;
	}
	//Vector perpendicular to projection
	Vector3D nproject(Vector3D q){
		Vector3D prj=this->project(q);
		Vector3D nprojection=q-prj;
		//The length of this vector is the distance from q to line
		return nprojection;
	}
	float distance(Vector3D q){
		float normv=v.length();
		Vector3D qp=p-q;
		Vector3D sp=qp.X(v);
		float normsp=sp.length();
		float dist=normsp/normv;
		return dist;
	}
	float distance(Line l){
		Vector3D ab=p-l.p;
		Vector3D uv=l.v.X(v);
		float normuv=uv.length();
		if(nearZero(normuv)){
			//parallel lines
			Vector3D abu=v.X(ab);
			float normabu=abu.length();
			float normv=v.length();
			float dist=normabu/normv;
			return dist;
		}
		float abuv=ab.mix(l.v,v);
		float dist=abuv/normuv;
		return dist;
	}
	inline bool intersect(Line l){return nearZero(distance(l));}
};
