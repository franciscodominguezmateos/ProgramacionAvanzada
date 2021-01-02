/*
 * shader_image_depth_warp.h
 *
 *  Created on: 2 Jan 2021
 *      Author: Francisco Dominguez
 *  It should make a image warp from depth di0 to di1 given a pose T
 *  text is a 32FC4 color depth texture but:
 *  - .xyz is color from di1
 *  - .w   is depth from di0
 *
 */
#pragma once
#include "shader_image_filter.h"
const string fragmentShaderWarp=R"glsl(
#version 330 core

in vec3 pixel;

out vec4 out_color;

uniform sampler2D tex;
uniform vec2 dim;//image dimensions
uniform mat4 T;  //camera pose

//camera intrinsic parameters
const float cx=320;
const float cy=240;
const float fx=640;
const float fy=480;

float lum(vec4 c){return (c.r+c.g+c.b)/3.0;}
vec2     tc(vec2 x=vec2(0,0)){return x/dim;}
vec2     rc(vec2 x=vec2(0,0)){return tc(gl_FragCoord.xy+x);}
float depth(vec2 x=vec2(0,0)){return texture(tex,rc(x)).w;}  //Relative
vec3  color(vec2 x=vec2(0,0)){return texture(tex,tc(x)).rgb;}//Absolute
vec3  get3D(vec2 x=vec2(0,0)){
 vec2 p=gl_FragCoord.xy+x;
 float z=depth();
 return vec3((p.x-cx)*z/fx,(p.y-cy)*z/fy,z);
}
vec2 project(vec3 p){ return vec2(p.x*fx/p.z+cx,py*fy/p.z+cy); }
bool is2DPointInImage(vec2 p){
	if (p.x<0     ) return false;
	if (p.y<0     ) return false;
	if (p.x>=dim.x)	return false;
	if (p.y>=dim.y)	return false;
	return true;
}
//warping function
vec2 w(vec2 x){
  vec3 p=get3D();
  vec3 Tp=T*p;
  return project(Tp);
}
void main(){
 //if good depth point
 if(depth()>0.0){//0 values of depth are bad values
  vec2 x=w();
  if(is2DPointInImage(x)){
   vec3 c=color(x);
   out_color=vec4(c,1);
  }
  else{
   out_color=vec4(0,0,1,0);
  }
 }
 else{ 
  out_color=vec4(1,0,0,0);
 }
)glsl";
class ShaderImageDepthWarp:public ShaderImageFilter{
public:
	ShaderImageDepthWarp(int w=640,int h=480):ShaderImageFilter(w,h){
			init();
	}
	void init(){
		fragmentShader=fragmentShaderWarp;
		spProg.compileFromStrings(vertexShader,fragmentShader);
	}
};




