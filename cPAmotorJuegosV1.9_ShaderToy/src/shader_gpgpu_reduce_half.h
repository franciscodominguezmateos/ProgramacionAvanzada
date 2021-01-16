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

vec2 tc(vec2 x){return x/dim;}
vec2 rc(vec2 x){return tc(gl_FragCoord.xy*2+x);}
vec4 getValues(vec2 x){return texture(tex,rc(x));}

//reduce operation to apply
vec4 mean(vec4 v00,vec4 v10,vec4 v01,vec4 v11){return (v00+v10+v01+v11)/4.0;}
// x value, arg should be in one of the other coordinates
vec4 argmax2(vec4 v0,vec4 v1){
 if(v0.x>v1.x)return v0;
 return v1;
}
vec4 argmin2(vec4 v0,vec4 v1){
 if(v0.x<v1.x)return v0;
 return v1;
}
vec4 argmax (vec4 v00,vec4 v10,vec4 v01,vec4 v11){
  vec4 vmax=argmax2(v00,v10);
       vmax=argmax2(vmax,v01);
       vmax=argmax2(vmax,v11);
  return vmax;
}
vec4 argmin (vec4 v00,vec4 v10,vec4 v01,vec4 v11){
  vec4 vmax=argmin2(v00,v10);
       vmax=argmin2(vmax,v01);
       vmax=argmin2(vmax,v11);
  return vmax;
}
vec4 max4(vec4 v00,vec4 v10,vec4 v01,vec4 v11){return max(v00,max(v10,max(v01,v11)));}
vec4 min4(vec4 v00,vec4 v10,vec4 v01,vec4 v11){return min(v00,min(v10,min(v01,v11)));}
vec4 opt (vec4 v00,vec4 v10,vec4 v01,vec4 v11){return mean(v00,v10,v01,v11);}

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
	ShaderReduceHalf(Texture* ptex=nullptr,int w=640,int h=480,string oc="vec4 opt(vec4 v00,vec4 v10,vec4 v01,vec4 v11){return (v00+v10+v01+v11)/4.0;}"):
			ShaderImageFilter(ptex,w>>1,h>>1),
			optCode(oc){
			init();
			workOutDim(ptex,w,h);
	}
	void init(){
		fragmentShader=fragmentShaderReduceHalf;
		string &fs=fragmentShader;
		fs=replaceLinesIfContains("vec4 opt" ,fs,optCode);
		spProg.compileFromStrings(vertexShader,fs);
	}
};

