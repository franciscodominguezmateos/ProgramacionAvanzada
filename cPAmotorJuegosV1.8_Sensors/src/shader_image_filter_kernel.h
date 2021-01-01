/*
 * shader_image_sobel_x.h
 *
 *  Created on: 31 Dec 2020
 *      Author: Francisco Dominguez
 */

#pragma once
#include "shader_image_filter.h"
const string fragmentShaderKernel=R"glsl(
//https://www.shadertoy.com/view/4dfGDH
//precomputing the kernel improves the performance
//sigma 10.0, MSIZE 15
//const float kernel[MSIZE] = float[MSIZE](
//0.031225216, 0.033322271, 0.035206333, 0.036826804, 0.038138565, 0.039104044, 0.039695028, 0.039894000, 0.039695028, 0.039104044, 0.038138565, 0.036826804, 0.035206333, 0.033322271, 0.031225216);

#ifdef GL_ES
precision mediump float;
#endif
#version 330 core

in vec3 pixel;

out vec4 out_color;

uniform sampler2D tex;
uniform vec2 dim;
uniform mat3 kernel;

float lum(vec4 c){return (c.r+c.g+c.b)/3.0;}
vec2 tc(vec2 x){return x/dim;}
vec2 rc(vec2 x=vec2(0,0)){return tc(gl_FragCoord.xy+x);}
vec4 getColor(vec2 x=vec2(0,0)){return texture(tex,rc(x));}

#define MSIZE 3

void main(){
	//declare stuff
	const int kSize = (MSIZE-1)/2;
	vec3 final_colour = vec3(0.0);

	float factor;
    float Z=0.0;
	//read out the texels
	for (int i=-kSize; i <= kSize; i++)	{
		for (int j=-kSize; j <= kSize; j++)	{
			vec3 c = getColor().rgb;
			factor = kernel[kSize+j[kSize+i];
			Z += factor;
			final_colour += factor*c;
		}
	}
	out_color = vec4(final_colour/Z, 1.0);
}
)glsl";
class ShaderImageFilterKernel:public ShaderImageFilter{
public:
	ShaderImageFilterKernel(int w=640,int h=480):ShaderImageFilter(w,h){
			init();
	}
	void init(){
		fragmentShader=fragmentShaderKernel;
		spProg.compileFromStrings(vertexShader,fragmentShader);
	}
};


