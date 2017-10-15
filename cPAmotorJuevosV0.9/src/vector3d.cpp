/*
 * vector3d.cpp
 *
 *  Created on: 13 Oct 2016
 *      Author: Francisco Dominguez
 */
#include "vector3d.h"
Vector3D Vector3D::operator+(Vector3D b){
 Vector3D ret;
 ret.x=x+b.x;
 ret.y=y+b.y;
 ret.z=z+b.z;
 return ret;
}
Vector3D Vector3D::operator-(Vector3D b){
 Vector3D ret;
 ret.x=x-b.x;
 ret.y=y-b.y;
 ret.z=z-b.z;
 return ret;
}
Vector3D Vector3D::operator*(double f){
 Vector3D ret;
 ret.x=x*f;
 ret.y=y*f;
 ret.z=z*f;
 return ret;
}
Vector3D Vector3D::operator/(double f){
 Vector3D ret;
 ret.x=x/f;
 ret.y=y/f;
 ret.z=z/f;
 return ret;
}




