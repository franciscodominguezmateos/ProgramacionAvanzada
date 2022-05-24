/*
 * shader_image_filter_bilateral.h
 *
 *  Created on: 1 Jan 2021
 *      Author: Francisco Dominguez
 *  Implementation of a bilateral filter from shadertoy.com
 */
#pragma once
#include "shader_image_filter.h"
const string fragmentShaderBilateralFilter=R"glsl(
//https://www.shadertoy.com/view/4dfGDH
//precomputing the kernel improves the performance 7/01/2021 ¡DONE!
#version 330 core

in vec3 pixel;

out vec4 out_color;

uniform sampler2D tex;
uniform vec2 dim;

#define SIGMA 10.0
#define BSIGMA 0.1
#define MSIZE 15
const int kSize = (MSIZE-1)/2;
const float kernel[MSIZE];//PRECOMPUTED BEFORE COMPILING

//relative to current point in pixels
vec3 getColor(vec2 x=vec2(0,0)){return texture(tex,(gl_FragCoord.xy+x)/dim.xy).rgb;}
float normpdf(in float x, in float sigma){
	return 0.39894*exp(-0.5*x*x/(sigma*sigma))/sigma;
}
float normpdf3(in vec3 v, in float sigma){
	return 0.39894*exp(-0.5*dot(v,v)/(sigma*sigma))/sigma;
}
void main()
{
	vec3 c = getColor();
		
	//declare stuff
	vec3 final_colour = vec3(0.0);
	float Z = 0.0;
	
	vec3 cc;
	float factor;
	float bZ = 1.0/normpdf(0.0, BSIGMA);
	//read out the texels
	for (int i=-kSize; i <= kSize; i++)	{
		for (int j=-kSize; j <= kSize; j++)	{
			cc = getColor(vec2(float(i),float(j)));
			factor = normpdf3(cc-c, BSIGMA)*bZ*kernel[kSize+j]*kernel[kSize+i];
			Z += factor;
			final_colour += cc*factor;
		}
	}
    out_color = vec4(final_colour/Z, 1.0);
}
)glsl";
class ShaderImageFilterBilateral:public ShaderImageFilter{
	float sigma;
	float bsigma;
	int msize;
public:
	ShaderImageFilterBilateral(Texture* ptex=nullptr,float s=10,float bs=0.1,int ms=15,int w=640,int h=480):
			ShaderImageFilter(ptex,w,h),
			sigma(s),
			bsigma(bs),
			msize(ms){
		init();
		workOutDim(ptex,w,h);
	}
	float normpdf(const float x, const float sigma){
		return 0.39894*exp(-0.5*x*x/(sigma*sigma))/sigma;
	}
	vector<float> precompute_kernel(){
		const int kSize = (msize-1)/2;
		vector<float> kernel(msize);
		//create the 1-D kernel
		for (int j = 0; j <= kSize; j++){
			kernel[kSize+j] = kernel[kSize-j] = normpdf(float(j), sigma);
		}
		return kernel;
	}
	void init(){
		fragmentShader=fragmentShaderBilateralFilter;
		string &fs=fragmentShader;
		fs=replaceLinesIfContains("#define SIGMA" ,fs,"#define SIGMA " +to_string(sigma));
		fs=replaceLinesIfContains("#define BSIGMA",fs,"#define BSIGMA "+to_string(bsigma));
		fs=replaceLinesIfContains("#define MSIZE" ,fs,"#define MSIZE " +to_string(msize));
		fs=replaceLinesIfContains("const float kernel[MSIZE]" ,fs,"const float kernel[MSIZE]=float[MSIZE](" +to_string(precompute_kernel())+");");
		spProg.compileFromStrings(vertexShader,fs);
	}
};



