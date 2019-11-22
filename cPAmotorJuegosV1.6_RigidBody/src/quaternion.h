/*
 * quaternion.h
 *
 *  Created on: 15 Nov 2019
 *      Author: Francisco Dominguez
 */

#ifndef QUATERNION_H_
#define QUATERNION_H_
#include <cmath>
#include "vector3d.h"
class Quaternion {
	Vector3D v;
	double w;
public:
	Quaternion();
	// from angle axis (aka exp operator?)
	Quaternion(double angle,Vector3D axis){
		double c=cos(angle/2.0);
		double s=sin(angle/2.0);
		w=c;
		v=axis*s;
	}
	// from other vector and scalar
	Quaternion(Vector3D v,double w=0):v(v),w(w){}
	virtual ~Quaternion();
	Quaternion(const Quaternion &q):v(q.v),w(q.w){}
	inline static Quaternion exp(Vector3D v){
		double angle=v.length();
		v.normalize();
		return Quaternion(angle,v);
	}
	inline static Vector3D log(Quaternion q){
		return q.getAxis()*q.getAngle();
	}
	inline Vector3D log(){
		Quaternion &q=*this;
		return q.getAxis()*q.getAngle();
	}
	inline Vector3D getAxis(){
		Vector3D axis=v;
		axis.normalize();
		return axis;
	}
	inline double getAngle(){
		double angle=2.0*acos(w);
		return angle;
	}
	inline Vector3D getAngleAxisVector(){
		return getAxis()*getAngle();
	}
	// Conjugated
	inline Quaternion operator!(){
		return Quaternion(-v,w);
	}
	inline Quaternion operator*(Quaternion q2){
		double &w1=w, &w2=q2.w;
		Vector3D &v1=v, &v2=q2.v;
		double w3=w1*w2-v1*v2;
		Vector3D v3=v2*w1+v1*w2-v1.X(v2);
		return Quaternion(v3,w3);
	}
	inline Vector3D operator*(Vector3D v){
		Quaternion &q=*this;
		Quaternion qv(v);
		Quaternion r=q*qv*!q;
		return r.v;
	}
	inline Quaternion normalize(){
		double vl2=v.lengthSquared();
		double wl2=w*w;
		double l=sqrt(vl2+wl2);
		w/=l;
		v/=l;
		return *this;
	}
	// unit quaternions is only conjugate
	inline Quaternion inv(){
		Quaternion q=*this;
		// it should be norm squared?
		Quaternion r=!q.normalize();
	}
	inline Mat asMat(){
		Mat I=Mat::eye(3,3,CV_64F);
	    Mat A = I*(w*w-v*v)+v.O(v)*2+S(v)*w*2;
		return A;
	}
	// TODO: Jacobian
	inline Mat J(){
		Mat I=Mat::eye(3,3,CV_64F);
		return I;
	}
	// TODO: Aprox almost Rotation Matrix m as Quaternion
	inline static Quaternion Aprox(Mat m){
		return Quaternion();
	}
};

#endif /* QUATERNION_H_ */
