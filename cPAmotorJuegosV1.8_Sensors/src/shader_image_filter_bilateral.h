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
//precomputing the kernel improves the performance
//sigma 10.0, MSIZE 15
//const float kernel[MSIZE] = float[MSIZE](
//0.031225216, 0.033322271, 0.035206333, 0.036826804, 0.038138565, 0.039104044, 0.039695028, 0.039894000, 0.039695028, 0.039104044, 0.038138565, 0.036826804, 0.035206333, 0.033322271, 0.031225216);
#ifdef GL_ES
precision mediump float;
#endif

#define SIGMA 10.0
#define BSIGMA 0.1
#define MSIZE 15

float normpdf(in float x, in float sigma)
{
	return 0.39894*exp(-0.5*x*x/(sigma*sigma))/sigma;
}

float normpdf3(in vec3 v, in float sigma)
{
	return 0.39894*exp(-0.5*dot(v,v)/(sigma*sigma))/sigma;
}


void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
	vec3 c = texture(iChannel0, vec2(0.0, 1.0)-(fragCoord.xy / iResolution.xy)).rgb;
	if (fragCoord.x < iMouse.x)
	{
		fragColor = vec4(c, 1.0);
		
	} else {
		
		//declare stuff
		const int kSize = (MSIZE-1)/2;
		float kernel[MSIZE];
		vec3 final_colour = vec3(0.0);
		
		//create the 1-D kernel
		float Z = 0.0;
		for (int j = 0; j <= kSize; ++j)
		{
			kernel[kSize+j] = kernel[kSize-j] = normpdf(float(j), SIGMA);
		}
		
		vec3 cc;
		float factor;
		float bZ = 1.0/normpdf(0.0, BSIGMA);
		//read out the texels
		for (int i=-kSize; i <= kSize; ++i)
		{
			for (int j=-kSize; j <= kSize; ++j)
			{
				cc = texture(iChannel0, vec2(0.0, 1.0)-(fragCoord.xy+vec2(float(i),float(j))) / iResolution.xy).rgb;
				factor = normpdf3(cc-c, BSIGMA)*bZ*kernel[kSize+j]*kernel[kSize+i];
				Z += factor;
				final_colour += factor*cc;

			}
		}
		
		
		fragColor = vec4(final_colour/Z, 1.0);
	}
}
)glsl";
class ShaderImageFilterBilateral:public ShaderImageFilter{
public:
	ShaderImageFilterBilateral(int w=640,int h=480):ShaderImageFilter(w,h){
			init();
	}
	void init(){
		fragmentShader=fragmentShaderBilateralFilter;
		spProg.compileFromStrings(vertexShader,fragmentShader);
	}
};



