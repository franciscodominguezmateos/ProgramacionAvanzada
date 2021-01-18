/*
 * shader_gpgpu_map.h
 *
 *  Created on: 1 Jan 2021
 *      Author: Francisco Dominguez
 *  Treat data as images of 32FC4
 */
#pragma once
#include "shader_image_filter.h"
const string fragmentShaderMap=R"glsl(
#version 330 core

in vec3 pixel;

out vec4 out_color;

uniform sampler2D tex;
uniform vec2 dim;

//reduce operation to apply
vec4 square(vec4 v) {return v*v;}
//Init for argmax and argmin reductions
vec4 initArg(vec4 v){return vec4(v.x,gl_FragCoord.x/dim.x,gl_FragCoord.y/dim.y,1.0);}
vec4 opt(vec4 v){return v/2;}

void main(){
 out_color=opt(texture(tex,gl_FragCoord.xy/dim));
}
)glsl";
const string opInttArg="vec4 opt(vec4 v){return intArg(v);}";
const string opSquare ="vec4 opt(vec4 v){return square(v);}";
class ShaderMap:public ShaderImageFilter{
protected:
	string optCode;
public:
	ShaderMap(Texture* ptex=nullptr,int w=640,int h=480,string oc="vec4 opt(vec4 v){return v/2;}"):
			ShaderImageFilter(ptex,w,h),
			optCode(oc){
		init();
		workOutDim(ptex,w,h);
	}
	void init(){
		fragmentShader=fragmentShaderMap;
		string &fs=fragmentShader;
		fs=replaceLinesIfContains("vec4 opt" ,fs,optCode);
		spProg.compileFromStrings(vertexShader,fs);
	}
};

