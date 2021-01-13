/*
 * shader.h
 *
 *  Created on: 3 Jan 2020
 *      Author: Francisco Dominguez
 *  Inspired by: https://www.youtube.com/user/ThinMatrix
 */
#pragma once
#include "util.h"
#include "vector3d.h"

// In order to use GLSL
#define GL_GLEXT_PROTOTYPES
#include <GL/glut.h>
// In order to use GLSL
#include <GL/glext.h>
#include "texture.h"

struct Vec2{float x,y;};
struct Vec3{float x,y,z;};
struct Vec4{float x,y,z,w;};

#include "glsl_uniform.h"
#include "glsl_vbo.h"
#include "glsl_vao.h"
#include "glsl_fbo.h"
#include "glsl_shader.h"
#include "glsl_shader_program.h"

