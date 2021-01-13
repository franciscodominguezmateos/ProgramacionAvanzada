/*
 * shader_image_sobel_x.h
 *
 *  Created on: 31 Dec 2020
 *      Author: Francisco Dominguez
 */

#pragma once
#include "shader_image_filter.h"
const string fragmentShaderSobelY=R"glsl(
#version 330 core

in vec3 pixel;

out vec4 out_color;

uniform sampler2D tex;
uniform vec2 dim;

float lum(vec4 c){return (c.r+c.g+c.b)/3.0;}
vec2 tc(vec2 x){return x/dim;}
vec2 rc(vec2 x){return tc(gl_FragCoord.xy+x);}

void main(){
 vec2 v00=rc(vec2(0,0));
 vec2 v01=rc(vec2(0,1));
 float l00=lum(texture(tex,v00));
 float l01=lum(texture(tex,v01));
 float gy=l01-l00;
 vec3 c=vec3(gy,gy,gy);
 out_color=vec4(c.rgb+0.5,1);
}
)glsl";
class ShaderImageFilterSobelY:public ShaderImageFilter{
public:
	ShaderImageFilterSobelY(Texture* ptex=nullptr,int w=640,int h=480):
			ShaderImageFilter(ptex,w,h){
		init();
		workOutDim(ptex,w,h);
	}
	void init(){
		fragmentShader=fragmentShaderSobelY;
		spProg.compileFromStrings(vertexShader,fragmentShader);
	}
};


