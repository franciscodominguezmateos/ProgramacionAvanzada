//#pragma once
#ifndef VECTOR3D__
#define VECTOR3D__
#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include "util.h"

using namespace std;
using namespace cv;


template <class S> class Vector3Dx;

typedef Vector3Dx<double> Vector3D;

typedef Vector3Dx<float>  Vector3Df;
typedef Vector3Dx<double> Vector3Dd;
typedef Vector3Dx<int>    Vector3Di;

template <class S>
class Vector3Dx {
	S x, y, z;
public:
	Vector3Dx() :Vector3Dx(0, 0, 0) {}
	Vector3Dx(S x, S y, S z=0) :x(x), y(y), z(z) {}
	//Vector3Dx(const S &x, const S &y, const S &z) :x(x), y(y), z(z) {}
	Vector3Dx(const vector<S> &v){x=v[0];y=v[1];if(v.size()==3)z=v[2];}
	Vector3Dx(const Mat &m){
		if(m.rows==1){
			x=m.at<S>(0,0);y=m.at<S>(0,1);z=m.at<S>(0,2);
		}
		else {
			if(m.cols==1){
				x=m.at<S>(0,0);y=m.at<S>(1,0);z=m.at<S>(2,0);
			}
			else{
				throw runtime_error("Mat has to have one row or one column this has "+to_string(m.cols)+" columns and "+to_string(m.rows)+" rows, at Vector3D constructor");
			}
		}
	}
	Vector3Dx(const Vector3Dx<S> &v) :x(v.x), y(v.y), z(v.z) {}
	Vector3Dx<S> *clone() { return new Vector3Dx<S>(*this); }
	inline Vector3Dx<S> operator=(Mat m) {
		Vector3Dx<S> &v=*this;
		if(m.cols==1 && m.rows>=3)	    for(int i=0;i<3;i++) v[i]=m.at<S>(i,0);
		else if(m.cols>=3 && m.rows==1) for(int i=0;i<3;i++) v[i]=m.at<S>(0,i);
		else throw runtime_error("Error in Vector3D::operator=(Mat) Vector from Mat");
		return v;
	}
	inline Vector3Dx<S> operator+(const Vector3Dx<S> &b) const {return	Vector3Dx<S>(x + b.x, y + b.y, z + b.z);}
	inline Vector3Dx<S> operator+=(const Vector3Dx<S> &v){x+=v.x;y+=v.y;z+=v.z;return *this;}
	inline Vector3Dx<S> operator-() {return	Vector3Dx<S>(-x,-y,-z);}
	inline Vector3Dx<S> operator-(const Vector3Dx<S> &b) {return Vector3Dx<S>(x-b.x,y-b.y,z-b.z);}
	inline Vector3Dx<S> operator-=(const Vector3Dx<S> &v){x-=v.x;y-=v.y;z-=v.z;}
	inline Vector3Dx<S> operator/=(const S &d){	x/=d; y/=d;	z/=d; return *this;	}
		inline Vector3Dx<S> operator/(S d) {return 	Vector3Dx<S>(x / d, y / d, z / d);}
	inline S getX() { return x; }
	inline S getY() { return y; }
	inline S getZ() { return z; }
	inline void setX(const double &px) { x = px; }
	inline void setY(const double &py) { y = py; }
	inline void setZ(const double &pz) { z = pz; }
	inline S &operator[](int i){
		if (i==0) return x;
		if (i==1) return y;
		if (i==2) return z;
		throw runtime_error("Error in Vector3Dx::operator[] index must be 0..2");
	}
	/*             P R O D U C T S            */
	inline Vector3Dx<S> operator*(const S &d) {return Vector3Dx<S>(x * d, y * d, z * d);}
	inline Vector3Dx<S> operator*=(const S &d){	x*=d; y*=d;	z*=d; return *this;	}
	// outer product
	inline Mat O(Vector3Dx<S> v){
		S &x0=x,&y0=y,&z0=z;
		S &x1=v.x,&y1=v.y,&z1=v.z;
	    Mat s = (Mat_<S>(3,3) <<
	               x0*x1,       x0*y1,     x0*z1,
	               y0*x1,       y0*y1,     y0*z1,
	               z0*x1,       z0*y1,     z0*z1 );
		return s;
	}
	// scalar product
	inline S operator * (const Vector3Dx<S> &v2) {
		Vector3Dx &v1 = *this;
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}
	inline Vector3Dx<S> crossProduct(const Vector3Dx<S> &v2) {
		Vector3Dx<S> &v1 = *this;
		return Vector3Dx<S>(
				v1.y*v2.z - v1.z*v2.y,
			    v1.z*v2.x - v1.x*v2.z,
			    v1.x*v2.y - v1.y*v2.x);
	}
	//Syntactic version of crossProduct
	inline Vector3Dx<S> X(const Vector3Dx<S> &v2) {
		Vector3Dx<S> &v1 = *this;
		return Vector3Dx<S>(
				v1.y*v2.z - v1.z*v2.y,
			    v1.z*v2.x - v1.x*v2.z,
			    v1.x*v2.y - v1.y*v2.x);
	}
	inline S mix(Vector3Dx<S> v2,Vector3Dx<S> v3){
		Vector3Dx<S> &v1=*this;
		return v1*v2.X(v3);
	}
	inline S lengthSquared() {return x * x + y * y + z * z;}
	inline S length() {return sqrt(lengthSquared());}
	inline void normalize() {
		S magnitude = length();
		//assert(!equalsZero(magnitude));
		if (nearZero(magnitude))
			throw runtime_error(" in Vector3D::normalize(): magnitude near to zero.");
		magnitude = 1.0f / magnitude;
		x *= magnitude;
		y *= magnitude;
		z *= magnitude;
	}
	//Change the length of this vector but not the direction
	inline void setLength(S d){
		Vector3Dx<S> &vn=*this;
		vn.normalize();
		x*=d;
		y*=d;
		z*=d;
	}
	inline S distance(Vector3Dx<S> v){
		Vector3Dx<S> &t=*this;
		Vector3Dx<S> dif=t-v;
		return dif.length();
	}
	inline Vector3D project(Vector3Dx<S> v){
		//project v on *this
		//It is suppose *this is unit length vector
		Vector3Dx<S> &vn=*this;
		S d=vn*v;
		return Vector3Dx<S>(vn*d);
	}
	inline Vector3D projectOn(Vector3Dx<S> vn){
		//project *this on vn
		//It is suppose vn is unit length vector
		Vector3Dx<S> &v=*this;
		S d=vn*v;
		return Vector3Dx<S>(vn*d);
	}
	// column vector
	inline Mat asMat(){	return (Mat_<S>(3,1)<< x,y,z);}
	inline Mat asH()  {	return (Mat_<S>(4,1)<< x,y,z,1);}
	inline Mat asH0() {	return (Mat_<S>(4,1)<< x,y,z,0);}
	//template <class S>  //this doesn't work
	friend ostream &operator<<(ostream &os, const Vector3Dx<S> &v);
};
Mat operator*(Mat m,Vector3D v){return m*v.asMat();}
Mat operator+(Mat m,Vector3D v){return m+v.asMat();}
Mat operator-(Mat m,Vector3D v){return m-v.asMat();}
/*
template <class T>
inline ostream &operator<<(ostream &os, const Vector3D<T> &v) {
os << v.x << "," << v.y << "," << v.z;
return os;
}
*/
inline ostream &operator<<(ostream &os, const Vector3Dd &v) {
	os << v.x << "," << v.y << "," << v.z;
	return os;
}
inline ostream &operator<<(ostream &os, const Vector3Df &v) {
	os << v.x << "," << v.y << "," << v.z;
	return os;
}
inline ostream &operator<<(ostream &os, const Vector3Di &v) {
	os << v.x << "," << v.y << "," << v.z;
	return os;
}
// column vector
inline Mat asMat(Vector3D v){
	return (Mat_<double>(3,1)<< v.getX(),v.getY(),v.getZ());
}
// Skew or hat operator
inline Mat S(Vector3D &v){
	double x=v.getX();
	double y=v.getY();
	double z=v.getZ();
    Mat s = (Mat_<double>(3,3) <<
               0,       -z,     y,
               z,        0,    -x,
              -y,        x,     0 );
	return s;
}
// Inverse skew or Vee operator
inline Vector3D v(Mat &S){
	Vector3D r(S.at<double>(2,1),S.at<double>(0,2),S.at<double>(1,0));
	return r;
}
inline Vector3D asVector3D(Mat m){
	if(m.rows==1)
		return Vector3D(m.at<double>(0,0),m.at<double>(0,1),m.at<double>(0,2));
	if(m.cols==1)
		return Vector3D(m.at<double>(0,0),m.at<double>(1,0),m.at<double>(2,0));
	throw runtime_error("Mat has to have one row or one column this has "+to_string(m.cols)+" columns and "+to_string(m.rows)+" rows, at Vector3D::asVector3D");
}

// FROM: https://www.learnopencv.com/rotation-matrix-to-euler-angles/
// Calculates rotation matrix given euler angles.
inline Mat eulerAnglesToRotationMatrix(Vector3D theta){
	theta=theta*DEG2RAD;
	// Calculate rotation about x axis
    Mat R_x = (Mat_<double>(3,3) <<
               1,       0,              0,
               0,       cos(theta[0]),   -sin(theta[0]),
               0,       sin(theta[0]),   cos(theta[0])
               );
    // Calculate rotation about y axis
    Mat R_y = (Mat_<double>(3,3) <<
               cos(theta[1]),    0,      sin(theta[1]),
               0,               1,      0,
               -sin(theta[1]),   0,      cos(theta[1])
               );
    // Calculate rotation about z axis
    Mat R_z = (Mat_<double>(3,3) <<
               cos(theta[2]),    -sin(theta[2]),      0,
               sin(theta[2]),    cos(theta[2]),       0,
               0,               0,                  1);
    // Combined rotation matrix
    Mat R = R_z * R_y * R_x;
    return R;
}
template<class S=double>
inline Mat posEulerAnglesToTransformationMatrix(Vector3D pos,Vector3D theta){
	theta=theta*DEG2RAD;
    // Calculate rotation about x axis
    Mat R_x = (Mat_<S>(4,4) <<
               1,       0,                            0, 0,
               0,       cos(theta[0]),   -sin(theta[0]), 0,
               0,       sin(theta[0]),    cos(theta[0]), 0,
               0,       0,                            0, 1);
    // Calculate rotation about y axis
    Mat R_y = (Mat_<S>(4,4) <<
               cos(theta[1]),    0,      sin(theta[1]), 0,
                0,               1,                  0, 0,
               -sin(theta[1]),   0,      cos(theta[1]), 0,
               0,       0,                           0, 1);
    // Calculate rotation about z axis
    Mat R_z = (Mat_<S>(4,4) <<
               cos(theta[2]),    -sin(theta[2]),      0, 0,
               sin(theta[2]),     cos(theta[2]),      0, 0,
               0,                 0,                  1, 0,
               0,                 0,                  0, 1);
    // Calculate translation
    Mat T = (Mat_<S>(4,4) <<
               1,    0,      0, pos[0],
               0,    1,      0, pos[1],
               0,    0,      1, pos[2],
               0,    0,      0, 1);
    // Combined rotation matrix
    Mat_<S> R (T*R_x * R_y * R_z);
    return R;
}
Mat getRotation(Mat &t){
	Mat r=t(Rect(0,0,3,3));
	return r;
}
Mat getTranslation(Mat &t){
	Mat r=t(Rect(3,0,1,3));
	return r;
}
Mat buildTransformation(Mat R,Mat t){
	Mat r;
	hconcat(R,t,r);
	//m as to be same type as r
	Mat m=Mat::zeros(1,4,r.type());
	//I don't know the real type I guess float
	// TO FIX
	m.at<float>(0,3)=1;
	r.push_back(m);
	return r;
}
Mat uniformScaleTransform(Mat t,float s){
	Mat R=getRotation(t);
	Mat p=getTranslation(t);
	p=p*s;
	Mat r= buildTransformation(R,p);
	cout << "r="<<r<<endl;
	return r;
}
// this interpolated by right-> dm in on the right and update is r=T0*Tt
// m1=m0*dm->dm=m0.inv()*m1;
// This interpolation is not done in the SE(3) group.
// it makes rotation and translation independently from each other.
Mat interpolate(Mat T0,Mat T1,float t){
	//Rotation
	Mat m0=getRotation(T0);
	Mat m1=getRotation(T1);
	Mat dm,vt,mt;
	dm=m0.inv()*m1;
	//get vector axis representatiom
	Rodrigues(dm,vt);
	//scale vector by t
	vt=vt*t;
	//get final matrix increment mt
	Rodrigues(vt,mt);
	Mat mr=m0*mt;
	//Translation
	Mat t0=getTranslation(T0);
	Mat t1=getTranslation(T1);
	Mat mdif=t1-t0;
	Mat tt=mdif*t;
	Mat tr=t0+tt;
	//Build increment Transformation
	Mat Tr=buildTransformation(mr,tr);
	return Tr;
}
// Checks if a matrix is a valid rotation matrix.
inline bool isRotationMatrix(Mat &R){
    Mat Rt;
    transpose(R, Rt);
    Mat shouldBeIdentity = Rt * R;
    Mat I = Mat::eye(3,3, shouldBeIdentity.type());
    return  norm(I, shouldBeIdentity) < 1e-6;
}
//TODO
//Nearest rotation matrix
inline Mat nearestRotation(Mat &R0){
	return R0;
}
// Calculates rotation matrix to euler angles
// The result is the same as MATLAB except the order
// of the euler angles ( x and z are swapped ).
inline Vector3D rotationMatrixToEulerAngles(Mat &R){
    assert(isRotationMatrix(R));

    float sy = sqrt(R.at<double>(0,0) * R.at<double>(0,0) +  R.at<double>(1,0) * R.at<double>(1,0) );

    bool singular = sy < 1e-6; // If

    double x, y, z;
    if (!singular)
    {
        x = atan2(R.at<double>(2,1) , R.at<double>(2,2));
        y = atan2(-R.at<double>(2,0), sy);
        z = atan2(R.at<double>(1,0), R.at<double>(0,0));
    }
    else
    {
        x = atan2(-R.at<double>(1,2), R.at<double>(1,1));
        y = atan2(-R.at<double>(2,0), sy);
        z = 0;
    }
    return Vector3D(rad2deg(x), rad2deg(y), rad2deg(z));
}

#endif // !VECTOR3D__
/*
//Devolver constante de un objeto mutable v
class Vector3D {
	const double &getX() { return x; }
	double &getY() { return y; }
	.......
};
//Objeto mutable ya que no se declara con const
Vector3D v(1, 2, 3);

const double &d = v.getX();
d = 4;///ERROR d es constante
double &c = v.getY();
c = 7;


//Devolver valores a objetos const v
class Vector3D {
	double getX() { return x; }
	double getY() const { return y; }
	.......
};
//Objeto const v
const Vector3D v(1, 2, 3);
double d = v.getX();//No se pude hacer v es constante
double c = v.getY();//se puede hacer ya que getY() est permitido en const
*/
