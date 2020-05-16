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
	//Plucker coordinates
	//from: http://web.cs.iastate.edu/~cs577/handouts/plucker-coordinates.pdf
	Vector3D l,m;
public:
	Line(Vector3D p0,Vector3D p1):p(p0),l((p1-p0)){
		v=l;v.normalize();
		//moment vector of the line
		//It is the moment of a unit force acting at p0 in the direction v with respect to the origin.
		//The norm‖m‖ gives the distance from the origin to the line.
		m=p0.X(v);
	}
    //Projection of point in line
	Vector3D project(Vector3D q){
		// v is normalized
		Vector3D qp=p-q;
		float proj=v*qp;
		Vector3D projection=v*proj;
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
		float normv=l.length();
		Vector3D qp=q-p;
		Vector3D sp=qp.X(l);
		float normsp=sp.length();
		float dist=normsp/normv;
		return dist;
	}
	float distance(Line l){
		Vector3D ab=l.p-p;
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
	inline Vector3D getPoint(float t){return p+v*t;}
	inline Vector3D intersectionPoint(Line l){
		float px0=p.getX();
		float py0=p.getY();
		//float pz0=p.getZ();
		float vx0=v.getX();
		float vy0=v.getY();
		//float vz0=v.getZ();
		float px1=l.p.getX();
		float py1=l.p.getY();
		//float pz1=l.p.getZ();
		float vx1=l.v.getX();
		float vy1=l.v.getY();
		//float vz1=l.v.getZ();
		float denk=vx0*vy1-vy0*vx1;
		if(denk==0)
			throw runtime_error("from Line::intersectionPoint() line don't intersect.");
		float k=(vx0*(py0-py1)+vy0*(px1-px0))/denk;
		float t=(px1+vx1*k-px0)/vx0;
		Vector3D p0=p+v*t;
		Vector3D p1=l.p+l.v*k;
		if(!nearZero((p0-p1).length()))
			throw runtime_error("from Line::intersectionPoint() line don't intersect p0<>p1.");
		return p0;
	}
};
