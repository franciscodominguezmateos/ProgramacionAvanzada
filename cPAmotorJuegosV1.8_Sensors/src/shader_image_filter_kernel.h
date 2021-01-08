/*
 * shader_image_filter_kernel.h
 *
 *  Created on: 31 Dec 2020
 *      Author: Francisco Dominguez
 *  Kernel size must be 3x3
 */
#pragma once
#include "shader_image_filter.h"
const string fragmentShaderKernel=R"glsl(
#version 330 core

in vec3 pixel;

out vec4 out_color;

uniform sampler2D tex;
uniform vec2 dim;
uniform mat3 kernel;
#define MSIZE 3
const int kSize = (MSIZE-1)/2;

//relative to current point in pixels
vec3 getColor(vec2 x=vec2(0,0)){return texture(tex,(gl_FragCoord.xy+x)/dim.xy).rgb;}
void main(){
	//declare stuff
	vec3 final_colour = vec3(0.0);
	float factor=0.0;
    float Z=0.0;
	//read out the texels
	for (int i=-kSize; i <= kSize; i++)	{
		for (int j=-kSize; j <= kSize; j++)	{
			vec3 c = getColor(vec2(float(i),float(j)));
			factor = kernel[kSize+j][kSize+i];
			Z += factor;
			final_colour += c*factor;
		}
	}
	out_color = vec4(final_colour/Z, 1.0);
}
)glsl";
class ShaderImageFilterKernel:public ShaderImageFilter{
	Mat kernel;
public:
	ShaderImageFilterKernel(Texture* ptex=nullptr,Mat m=Mat::ones(3,3,CV_32F),int w=640,int h=480):
			ShaderImageFilter(ptex,w,h),
			kernel(m){
		init();
		setKernel(kernel);
		Vec2 dm={640,480};
		setDim(dm);
	}
	void setKernel(Mat &m){
		spProg.start();
		spProg["kernel"]=m;
		spProg.stop();
	}
	void init(){
		fragmentShader=fragmentShaderKernel;
		spProg.compileFromStrings(vertexShader,fragmentShader);
	}
};


