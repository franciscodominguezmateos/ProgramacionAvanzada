/*
 * rectangulo.cpp
 *
 *  Created on: Oct 18, 2017
 *      Author: francisco
 */

#include "rectangulo.h"

Rectangulo::Rectangulo(Vector3D p0,Vector3D p1,Vector3D p2,Vector3D p3):Triangulo(p0,p1,p2),p3(p3),nu(1),nv(1) {

}

Rectangulo::~Rectangulo() {
}

