/*
 * shader_image_depth_direct_j.h
 *
 *  Created on: 8 Jan 2021
 *      Author: Francisco Dominguez
 *  It should make a image warp from depth di0 to di1 given a pose T
 *  text is a 32FC4 color depth texture but:
 *  - .xyz is x=gradientX, y=gradientY and z=residual of di1-di0
 *  - .w   is depth from di0
 *  +08/01/2021 get intrinsic parameter from DepthImage di0 and set as const float inside shader
 */
#pragma once
#include "depth_image.h"
#include "shader_image_filter.h"
const string fragmentShaderDirectJ=R"glsl(
#version 330 core

in vec3 pixel;

out vec4 out_color;

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

float lum(vec4 c){return (c.r+c.g+c.b)/3.0;}
vec2     tc(vec2 x=vec2(0,0)){return x/dim;}
vec2     rc(vec2 x=vec2(0,0)){return tc(gl_FragCoord.xy+x);}
float depth(vec2 x=vec2(0,0)){return texture(tex,rc(x)).w;}  //Relative
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
//gradient X is in x and gradient Y is in y
vec2 grad(){
  return color().xy;
}
void main(){
  //3D point at di0 frame
  vec3 p0=get3D();
  //3D point at di1 frame
  vec4 p =T*vec4(p0,1);
  vec2 g=grad();
  float x2=p.x*p.x;
  float y2=p.y*p.y;
  float z2=p.z*p.z;

#if 1
  float Jw00=-fx*p.x*p.y/z2;
  float Jw01= fx*(1+x2/z2);
  float Jw02=-fx*p.y/p.z;
  
  float Jw10=-fy*(1+y2/z2);
  float Jw11= fy*p.x*p.y/z2;
  float Jw12= fy*p.x/p.z;
  
  vec2 c0=vec2(Jw00,Jw10);
  vec2 c1=vec2(Jw01,Jw11);
  vec2 c2=vec2(Jw02,Jw12);
  float j0=dot(g,c0);
  float j1=dot(g,c1);
  float j2=dot(g,c2);
  out_color=vec4(j0,j1,j2,depth());

#else
  float Jw03= fx/p.z;
  float Jw04= 0;
  float Jw05=-fx*p.x/z2;
  
  float Jw13= 0;
  float Jw14= fy/p.z;
  float Jw15=-fy*p.y/z2;
  vec2 c3=vec2(Jw03,Jw13);
  vec2 c4=vec2(Jw04,Jw14);
  vec2 c5=vec2(Jw05,Jw15);

  float j3=dot(g,c3);
  float j4=dot(g,c4);
  float j5=dot(g,c5);
  out_color=vec4(j3,j4,j5,depth());
#endif
}
)glsl";
class ShaderImageDepthDirectJ:public ShaderImageFilter{
	DepthImage di0,di1;
	Mat T;//pose
public:
	ShaderImageDepthDirectJ(DepthImage &d0,DepthImage &d1,const Mat &t):
		ShaderImageFilter(nullptr,d0.cols(),d0.rows()),di0(d0),di1(d1),T(t){
			init();
			Mat i1D0=getImg1Depth0();
			setImage(i1D0);
			spProg.start();
			 spProg["T"]=T;
			spProg.stop();
	}
	void init(){
		fragmentShader=fragmentShaderDirectJ;
		string &fs=fragmentShader;
		fs=replaceLinesIfContains("const float level" ,fs,"const float level=" +to_string(di0.getLevel())+";");
		fs=replaceLinesIfContains("const float cx"    ,fs,"const float cx="    +to_string(di0.getCx())+";");
		fs=replaceLinesIfContains("const float cy"    ,fs,"const float cy="    +to_string(di0.getCy())+";");
		fs=replaceLinesIfContains("const float fx"    ,fs,"const float fx="    +to_string(di0.getFx())+";");
		fs=replaceLinesIfContains("const float fy"    ,fs,"const float fy="    +to_string(di0.getFx())+";");
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
		Mat dimg=di0.getDepth();
		Mat img;
		img.convertTo(img,CV_32FC4);
		//vector<Mat> vd = { channels[0], channels[1],channels[2], dimg };
		vector<Mat> vd = { di.getGray(), di.getGradXImg(),di.getGradYImg(), dimg };
		merge(vd, img);
		return img;
	}

};




