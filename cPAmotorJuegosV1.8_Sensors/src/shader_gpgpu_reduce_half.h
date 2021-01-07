/*
 * shader_gpgpu_reduce_half.h
 *
 *  Created on: 1 Jan 2021
 *      Author: Francisco Dominguez
 *  Treat data as images of 32FC4
 *  Input is a image of w,h size and output is w/2,h/2
 */
#pragma once
#include "shader_image_filter.h"
const string fragmentShaderReduceHalf=R"glsl(
#version 330 core

in vec3 pixel;

out vec4 out_color;

uniform sampler2D tex;
uniform vec2 dim;

vec2 rc(vec2 x){return tc(gl_FragCoord.xy*2+x);}
vec4 getValues(vec2 x){return texture(tex,rc(x));}

//reduce operation to apply
vec4 opt(vec4 v00,vec4 v10,vec4 v01,vec4 v11){return (v00+v10+v01+v11)/4.0;}

void main(){
 vec4 v00=getValues(vec2(0,0));
 vec4 v10=getValues(vec2(1,0));
 vec4 v01=getValues(vec2(0,1));
 vec4 v11=getValues(vec2(1,1));
 out_color=opt(v00,v10,v01,v11);
}
)glsl";
class ShaderReduceHalf:public ShaderImageFilter{
	string optCode;
public:
	ShaderReduceHalf(int w=640,int h=480,string oc="vec4 opt(vec4 v00,vec4 v10,vec4 v01,vec4 v11){return (v00+v10+v01+v11)/4.0;}"):
			ShaderImageFilter(w>>1,h>>1),
			optCode(oc){
			init();
	}
	void init(){
		fragmentShader=fragmentShaderReduceHalf;
		string &fs=fragmentShader;
		fs=replaceLinesIfContains("vec4 opt" ,fs,optCode);
		spProg.compileFromStrings(vertexShader,fs);
		spProg.compileFromStrings(vertexShader,fragmentShader);
	}
};

