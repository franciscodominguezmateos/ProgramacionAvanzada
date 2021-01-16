/*
 * shader_image_filter_gauss.h
 *
 *  Created on: 1 Jan 2021
 *      Author: Francisco Dominguez
 *  Implement a gaussian filter
 *  It is jusst a bilateral filter without color filter only location filter
 */
#pragma once
#include "shader_image_filter.h"
const string fragmentShaderGauss=R"glsl(
#version 330 core

in vec3 pixel;

out vec4 out_color;

uniform sampler2D tex;
uniform vec2 dim;

#define MSIZE 15
const int kSize = (MSIZE-1)/2;
const float kernel[MSIZE];//PRECOMPUTED BEFORE COMPILING

//relative to current point in pixels
vec3 getColor(vec2 x=vec2(0,0)){return texture(tex,(gl_FragCoord.xy+x)/dim.xy).rgb;}
void main(){
    //declare stuff
	vec3 final_colour = vec3(0.0);
	float Z = 0.0;
	float factor=0;
	//read out the texels
	for (int i=-kSize; i <= kSize; i++)	{
		for (int j=-kSize; j <= kSize; j++)	{
			vec3 c = getColor(vec2(float(i),float(j)));
			factor = kernel[kSize+j]*kernel[kSize+i];
			Z += factor;
			final_colour += c*factor;
		}
	}
    out_color = vec4(final_colour/Z, 1.0);
}
)glsl";
class ShaderImageFilterGauss:public ShaderImageFilter{
	float sigma;
	int msize;
public:
	ShaderImageFilterGauss(Texture* ptex=nullptr,float s=1.5,int ms=15,int w=640,int h=480):
			ShaderImageFilter(ptex,w,h),
			sigma(s),
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
		fragmentShader=fragmentShaderGauss;
		string &fs=fragmentShader;
		fs=replaceLinesIfContains("#define MSIZE" ,fs,"#define MSIZE " +to_string(msize));
		fs=replaceLinesIfContains("const float kernel[MSIZE]" ,fs,"const float kernel[MSIZE]=float[MSIZE](" +to_string(precompute_kernel())+");");
		//cout <<"ShaderImageFilterGauss:"<<endl<<fs<<endl;
		spProg.compileFromStrings(vertexShader,fs);
	}
};






