/*
 * solid_raymarching.h
 *
 *  Created on: 15 Jan 2021
 *      Author: Francisco Dominguez
 *  The point is return depth values in shader.
 */
#pragma once
#include "shader_toy.h"

class SolidRayMarching;
using SolidRayMarchingPtr=SolidRayMarching*;

class SolidRayMarching: public ShaderToy {
public:
	SolidRayMarching():ShaderToy(){setDepthTest(true);}
};

