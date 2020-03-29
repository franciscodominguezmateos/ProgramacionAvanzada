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
	Quaternion(){}
	// from angle axis (aka exp operator?)
	Quaternion(double angle,Vector3D axis){
		double c=cos(angle/2.0);
		double s=sin(angle/2.0);
		w=c;
		axis.normalize();
		v=axis*s;
	}
	// from other vector and scalar
	Quaternion(Vector3D v,double w=0):v(v),w(w){}
	// from rotation matrix m, get vector representation vr and exp to get quaternion.
	Quaternion(Mat &m){Mat vr;Rodrigues(m,vr);*this=exp(asVector3D(vr));}
	Quaternion(const Quaternion &q):v(q.v),w(q.w){}
	// better implementation from Kaes icra15 Grassia ref
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
	// Conjugate
	inline Quaternion operator!(){
		return Quaternion(-v,w);
	}
	// quaternion multiplication
	inline Quaternion operator*(Quaternion q2){
		double   &w1=w, &w2=q2.w;
		Vector3D &v1=v, &v2=q2.v;
		double w3=w1*w2-v1*v2;
		Vector3D v3=v2*w1+v1*w2+v1.X(v2);
		return Quaternion(v3,w3);
	}
	// quaternion vector application
	inline Vector3D operator*(Vector3D v){
		Quaternion &q=*this;
		// put in quaternion form it is not a unit quaternion
		Quaternion qv(v);
		Quaternion r=q*qv*!q;
		return r.v;
	}
	// doesn't have to be a unit quaternion
	// scale quaternion needed in tiem derivative function
	inline Quaternion operator*(double d){
		Quaternion &q=*this;
		Quaternion r(q.v*d,q.w*d);
		return r;
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
		return q;
	}
	inline Mat asMat(){
		Mat I=Mat::eye(3,3,CV_64F);
	    Mat A = I*(w*w-v*v)+v.O(v)*2+S(v)*w*2;
		return A;
	}
	// Time derivative of this quaternion given w as angular velocity
	Quaternion d(Vector3D w){
		Quaternion &q=*this;
		// put in quaternion form it is not a unit quaternion
		Quaternion qw(w);
		Quaternion r=q*qw*0.5;
		return r;
	}
	// Time integration of a angular velocity w a time dt considering dw=0<->no angular acceleration
	Quaternion I(Vector3D w,double dt){
		Quaternion &q=*this;
		Quaternion dq(exp(w*dt));
		Quaternion r=q*dq;
		return r;
	}
	// Parameter derivative aka Jacobian
	inline Mat J(){
		Mat I=Mat::eye(3,3,CV_64F);
		return I;
	}
	// Parameter derivateve of quaterion action on p aka Jacobian of action/application
	Mat Ja(Vector3D a){
		Mat I=Mat::eye(3,3,CV_64F);
		Vector3D v0=a*w+v.X(a);
		Mat m=a*v*I+v.O(a)-a.O(v)-w*S(a);
		Mat r=Mat::zeros(3,4,CV_64F);
	}
	// TODO: Aprox almost Rotation Matrix m as Quaternion
	inline static Quaternion aprox(Mat m){
		return Quaternion();
	}
	// TODO: Interpolation
	// https://en.wikipedia.org/wiki/Slerp
	// q1=d*q0 => q1*!q0=d
	inline Quaternion slerp(Quaternion q1,double t){
		return q1;
	}
};
#endif /* QUATERNION_H_ */
