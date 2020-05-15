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
public:
	Segment(Vector3D p0,Vector3D p1):Line(p0,p1),p1(p1){}
};
