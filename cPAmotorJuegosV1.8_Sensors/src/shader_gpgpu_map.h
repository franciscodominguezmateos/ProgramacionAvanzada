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
vec4 opt(vec4 v){return v/2;}

void main(){
 out_color=opt(texture(tex,gl_FragCoord);
}
)glsl";
class ShaderMap:public ShaderImageFilter{
	string optCode;
public:
	ShaderMap(Texture* ptex=nullptr,string oc="vec4 opt(vec4 v){return v/2;}",int w=640,int h=480):
			ShaderImageFilter(ptex,w,h),
			optCode(oc){
		init();
		Vec2 dm={640,480};
		setDim(dm);
	}
	void init(){
		fragmentShader=fragmentShaderMap;
		string &fs=fragmentShader;
		fs=replaceLinesIfContains("vec4 opt" ,fs,optCode);
		spProg.compileFromStrings(vertexShader,fs);
	}
};

