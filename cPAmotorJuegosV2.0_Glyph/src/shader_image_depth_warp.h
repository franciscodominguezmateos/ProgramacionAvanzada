/*
 * shader_image_depth_warp.h
 *
 *  Created on: 2 Jan 2021
 *      Author: Francisco Dominguez
 *  It should make a image warp from depth di0 to di1 given a pose T
 *  text is a 32FC4 color depth texture but:
 *  - .xyz is color from di1
 *  - .w   is depth from di0
 *  +08/01/2021 get intrinsic parameter from DepthImage di0 and set as const float inside shader
 */
#pragma once
#include "depth_image.h"
#include "shader_image_filter.h"
const string fragmentShaderWarp=R"glsl(
#version 330 core

in vec3 pixel;

out vec4 out_color;

//RGBD image
// r= gray scale from img1
// g= x gradient of img0 or 1? Not used
// b= y gradient of img0 or 1? Not used
// w= is depth divided by depthScale
uniform sampler2D tex;
//image dimensions
uniform vec2 dim;
//camera pose from 0 to 1 T10=T01.inv()
uniform mat4 T;  

//camera intrinsic parameters
const float level=1;
const float cx= 319.5/level;
const float cy= 239.5/level;
const float fx= 480.2/level;
const float fy= 480.0/level;
//this is done to normalize
//since, for the moment, I can't ouput unclamped color values
const float depthScale=10;

float lum(vec4 c){return (c.r+c.g+c.b)/3.0;}
vec2     tc(vec2 x=vec2(0,0)){return x/dim;}
vec2     rc(vec2 x=vec2(0,0)){return tc(gl_FragCoord.xy+x);}
float depth(vec2 x=vec2(0,0)){return texture(tex,rc(x)).w*depthScale;}  //Relative
vec3  color(vec2 x=vec2(0,0)){return texture(tex,tc(x)).rgb;}//Absolute
vec3  get3D(vec2 x=vec2(0,0)){
 vec2 p=gl_FragCoord.xy+x;
 float z=depth(x);
 return vec3((p.x-cx)*z/fx,(p.y-cy)*z/fy,z);
}
vec2 project(vec3 p){ return vec2(p.x*fx/p.z+cx,p.y*fy/p.z+cy); }
bool is2DPointInImage(vec2 p){
	if (p.x<0     ) return false;
	if (p.y<0     ) return false;
	if (p.x>=dim.x)	return false;
	if (p.y>=dim.y)	return false;
	return true;
}
//warping function
vec2 w(){
  vec3 p=get3D();
  vec4 Tp=T*vec4(p,1);
  return project(Tp.xyz);
}
void main(){
 //if good depth point
 float d=depth();
 if(d>0.0){//0 values of depth are bad values
  vec2 x=w();
  if(is2DPointInImage(x)){
   vec3 c=color(x);
   out_color=vec4(c,d/depthScale);
  }
  else{
   out_color=vec4(0,0,0,0);
  }
 }
 else{ 
  out_color=vec4(0,0,0,0);
 }
}
)glsl";
class ShaderImageDepthWarp:public ShaderImageFilter{
	DepthImage di0,di1;
	Mat T;//pose
	//vallue to normalize/divide in order to get shader values from 0 to 1
	//this valuee is max range in metres
	float depthScale;
public:
	ShaderImageDepthWarp(DepthImage &d0,DepthImage &d1,const Mat &t):
		ShaderImageFilter(nullptr,d0.cols(),d0.rows()),di0(d0),di1(d1),T(t),depthScale(10){
			init();
			Mat i1D0=getImg1Depth0();
			setImage(i1D0);
			spProg.start();
			 spProg["T"]=T;
			spProg.stop();
	}
	void init(){
		fragmentShader=fragmentShaderWarp;
		string &fs=fragmentShader;
		fs=replaceLinesIfContains("const float level"     ,fs,"const float level=" +to_string(di0.getLevel())+";");
		fs=replaceLinesIfContains("const float cx"        ,fs,"const float cx="    +to_string(di0.getCx())+";");
		fs=replaceLinesIfContains("const float cy"        ,fs,"const float cy="    +to_string(di0.getCy())+";");
		fs=replaceLinesIfContains("const float fx"        ,fs,"const float fx="    +to_string(di0.getFx())+";");
		fs=replaceLinesIfContains("const float fy"        ,fs,"const float fy="    +to_string(di0.getFx())+";");
		fs=replaceLinesIfContains("const float depthScale",fs,"const float depthScale="    +to_string(depthScale)+";");
		//cout <<"ShaderImageDepthWarp:"<<endl<<fs<<endl;
		spProg.compileFromStrings(vertexShader,fs);
	}
	//return colors from di1 and depth from di0
	//better gray scale, grad x and grad y from di1 and depth from di0
	Mat getImg1Depth0(){
		DepthImage &di=di1;
		/*Mat cimg=di.getImg();
		cimg.convertTo(cimg,CV_32FC3);
		cimg/=255;
		vector<Mat> channels;
		cv::split(cimg, channels);
		*/
		Mat dimg=di0.getDepth()/depthScale;
		Mat img;
		img.convertTo(img,CV_32FC4);
		//vector<Mat> vd = { channels[0], channels[1],channels[2], dimg };
		vector<Mat> vd = { di.getGray(), di.getGradXImg(),di.getGradYImg(), dimg };
		merge(vd, img);
		return img;
	}

};




