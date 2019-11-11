//#pragma once
#ifndef VECTOR3D__
#define VECTOR3D__
#include <iostream>
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
	//Vector3Dx(S x, S y, S z) :x(x), y(y), z(z) {}
	Vector3Dx(const S &x, const S &y, const S &z) :x(x), y(y), z(z) {}
	Vector3Dx(const Vector3Dx<S> &v) :x(v.x), y(v.y), z(v.z) {}
	Vector3Dx<S> *clone() { return new Vector3Dx<S>(*this); }
	inline Vector3Dx<S> operator+(const Vector3Dx<S> &b) {return	Vector3Dx<S>(x + b.x, y + b.y, z + b.z);}
	inline Vector3Dx<S> operator+=(const Vector3Dx<S> &v){x+=v.x;y+=v.y;z+=v.z;return *this;}
	inline Vector3Dx<S> operator-() {return	Vector3Dx<S>(-x,-y,-z);}
	inline Vector3Dx<S> operator-(const Vector3Dx<S> &b) {return Vector3Dx<S>(x-b.x,y-b.y,z-b.z);}
	inline Vector3Dx<S> operator-=(const Vector3Dx<S> &v){x-=v.x;y-=v.y;z-=v.z;}
	inline Vector3Dx<S> operator*(const S &d) {return Vector3Dx<S>(x * d, y * d, z * d);}
	inline Vector3Dx<S> operator*=(const S &d){	x*=d; y*=d;	z*=d; return *this;	}
	inline Vector3Dx<S> operator/(S d) {return 	Vector3Dx<S>(x / d, y / d, z / d);}
	inline S getX() { return x; }
	inline S getY() { return y; }
	inline S getZ() { return z; }
	inline void setX(const double &px) { x = px; }
	inline void setY(const double &py) { y = py; }
	inline void setZ(const double &pz) { z = pz; }
	// read only operator
	inline S operator[](int i){
		if (i==0) return x;
		if (i==1) return y;
		if (i==2) return z;
		return 0;
	}
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
	inline S lengthSquared() {return x * x + y * y + z * z;}
	inline S length() {return sqrt(lengthSquared());}
	inline void normalize() {
		S magnitude = length();
		//assert(!equalsZero(magnitude));
		if (nearZero(magnitude))
			throw runtime_error("Normal muy pequeña en Vector3D::normalize()");
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
	inline S distancia(Vector3Dx<S> v){
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
		//project *thish on vn
		//It is suppose vn is unit legth vector
		Vector3Dx<S> &v=*this;
		S d=vn*v;
		return Vector3Dx<S>(vn*d);
	}
	friend ostream &operator<<(ostream &os, const Vector3Dx<S> &v);
};
/*
template <class T>
inline ostream &operator<<(ostream &os, const Vector3D<T> &v) {
os << v.x << "," << v.y << "," << v.z;
return os;
}
*/
//template ostream &operator<< (ostream &os, const Vector3Dx<double> &v);


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
inline Vector3D asVector3D(Mat m){return Vector3D(m.at<double>(0,0),m.at<double>(0,1),m.at<double>(0,2));}

// FROM: https://www.learnopencv.com/rotation-matrix-to-euler-angles/
// Calculates rotation matrix given euler angles.
inline Mat eulerAnglesToRotationMatrix(Vector3D &theta)
{
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

// Checks if a matrix is a valid rotation matrix.
inline bool isRotationMatrix(Mat &R)
{
    Mat Rt;
    transpose(R, Rt);
    Mat shouldBeIdentity = Rt * R;
    Mat I = Mat::eye(3,3, shouldBeIdentity.type());

    return  norm(I, shouldBeIdentity) < 1e-6;
}

// Calculates rotation matrix to euler angles
// The result is the same as MATLAB except the order
// of the euler angles ( x and z are swapped ).
inline Vector3D rotationMatrixToEulerAngles(Mat &R)
{
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
    return Vector3D(x, y, z);
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
