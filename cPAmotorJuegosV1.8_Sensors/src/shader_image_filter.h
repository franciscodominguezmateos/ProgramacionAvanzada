/*
 * shader_image_filter.h
 *
 *  Created on: 29 Dec 2020
 *      Author: Francisco Dominguez
 *  This is a GPGPU program to implent different filter images
 */

#pragma once

#include "shader.h"
#include "texture.h"
#include "solid.h"

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
const string fragmentShaderSobelXXXX=R"glsl(
#version 330 core

in vec3 pixel;

out vec4 out_color;

uniform sampler2D tex;
uniform vec2 dim;

float lum(vec4 c){return (c.r+c.g+c.b)/3.0;}
vec2 tc(vec2 x){return x/dim;}
vec2 rc(vec2 x){return tc(gl_FragCoord.xy+x);}
float depth(vec2 x=vec2(0,0)){return texture(tex,rc(x)).w;}
vec3  get3D(vec2 x=vec2(0,0)){
 float cx=320;
 float cy=240;
 float fx=640;
 float fy=480;
 vec2 p=gl_FragCoord.xy+x;
 float z=depth();
 return vec3((p.x-cx)*z/fx,(p.y-cy)*z/fy,z);
}

void main(){
 vec2 v00=rc(vec2(0,0));
 vec2 v10=rc(vec2(1,0));
 vec2 v01=rc(vec2(0,1));
 float l00=lum(texture(tex,v00));
 float l10=lum(texture(tex,v10));
 float l01=lum(texture(tex,v01));
 float gx=l10-l00;
 float gy=l01-l00;
 float l=gx+gy;
 if(l<-0.1){ l=1;}
 else if(l> 0.1){ l=1;}
 else{ l+=0.5;}
 vec4 crgb=texture(tex,v00);
 vec4 c=vec4(abs(gx),abs(gy),0,crgb.w);
 if(c.w>4.5) c=vec4(c.rgb*0.25,0.25);
 if(c.w<0.1) c=vec4(0,0,10,1);
 out_color=vec4(gx,gx,gx,1);
}
)glsl";

class ShaderImageFilter : public Solid{
protected:
	static vector<GLfloat> vertices;
	static string vertexShader;
	string fragmentShader;
	GLSLShaderProgram spProg;
	GLSLVAO* pVao;
	GLSLFBO* pFbo;//Output 32FC4 fourth channel not used
	Texture* pTex;//Input  32FC4 four channels same value
public:
	ShaderImageFilter(int w=640,int h=480):pFbo(new GLSLFBO(w,h)),pTex(new Texture()){
		pVao=new GLSLVAO();
		pVao->init();
		pVao->createAttribute(0,vertices,3);
		pTex->init();
		pFbo->init();
	}
	virtual void init()=0;
	void setFragmentShader(string &fs){
		fragmentShader=fs;
		spProg.compileFromStrings(vertexShader,fragmentShader);
	}
	Texture* getTexture(){return pTex;}
	void setImage(Mat img){
		pTex->setImage(img);
		//Vec2 dim={float(img.cols),float(img.rows)};
		Vec2 dim={float(img.cols),float(img.rows)};
		setDim(dim);
	}
	void setDim(Vec2 d){spProg.start();spProg["dim"]=d;spProg.stop();}
	Mat filter(Mat m){
		setImage(m);
		render();
		Mat r=pFbo->toOpenCV32FC4();
		return r;
	}
	void render(){
		glDepthMask(GL_FALSE);
		pFbo->bind();
		spProg.start();
		pTex->bind();
		pVao->bindAll();
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
		pVao->unbindAll();
		pTex->unbind();
		spProg.stop();
		pFbo->unbind();
		glDepthMask(GL_TRUE);
	}
};
//This vertex does nothing
string ShaderImageFilter::vertexShader=R"glsl(
#version 330 core

in vec3 in_vertex;
//Must have same name that in fragmentShader
out vec3 pixel;

void main(){
    pixel=in_vertex/2+0.5;
    gl_Position=vec4(in_vertex,1.0);
}
)glsl";
//Just a square
vector<GLfloat> ShaderImageFilter::vertices = {
    // Left bottom triangle
    -1.0f,  1.0f, 0.0f,
    -1.0f, -1.0f, 0.0f,
     1.0f, -1.0f, 0.0f,
	 // Right top triangle
     1.0f, -1.0f, 0.0f,
     1.0f,  1.0f, 0.0f,
    -1.0f,  1.0f, 0.0f,
};




