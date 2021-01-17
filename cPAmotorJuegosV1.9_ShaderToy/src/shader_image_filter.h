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

const string fragmentShaderTest=R"glsl(
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
 float cx= 319.5;
 float cy= 239.5;
 float fx= 480.2;
 float fy=-480.0;
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
 out_color=vec4(gx+0.5,gy+0.5,0,1);
}
)glsl";
//this may inherit from Shadertoy
class ShaderImageFilter : public Solid{
protected:
	static vector<GLfloat> vertices;
	static string vertexShader;
	string fragmentShader;
	GLSLShaderProgram spProg;
	GLSLVAOPtr pVao;
	GLSLFBOPtr pFbo;//Output 32FC4 fourth channel not used or not
	TexturePtr pTex;//Input  32FC4 four channels same value or not
public:
	ShaderImageFilter(Texture* ptex=nullptr,int w=640,int h=480):pFbo(new GLSLFBO(w,h)),pTex(ptex){
		pVao=new GLSLVAO();
		pVao->init();
		pVao->createAttribute(0,vertices,3);
		pFbo->init();
		//init();
		if(pTex==nullptr){
			pTex=new Texture();
			pTex->init();
		}
	}
	~ShaderImageFilter(){delete pVao;}
	virtual void init(){
		setFragmentShader(fragmentShaderTest);
	}
	virtual void initFromFileName(const string &fileName){
		ifstream ifs(fileName);
		string stCode;
		ifs>>stCode;
		setFragmentShader(stCode);
	}
	inline Texture &getOutTex(){return pFbo->getColorTex();}
	void setFragmentShader(const string &fs){
		fragmentShader=fs;
		spProg.compileFromStrings(vertexShader,fragmentShader);
	}
	TexturePtr getTexture(){return pTex;}
	void setTexture(TexturePtr pt){pTex=pt;workOutDim(pt);}
	void setFrameBuffer(GLSLFBOPtr fb){pFbo=fb;}
	GLSLFBOPtr getFrameBuffer(){return pFbo;}
	void setImage(Mat img){
		pTex->setImage(img);
		//Vec2 dim={float(img.cols),float(img.rows)};
		Vec2 dim={float(img.cols),float(img.rows)};
		setDim(dim);
	}
	void workOutDim(Texture* ptex,int w=640,int h=480){
		if(ptex==nullptr){setDim(w,h);}
		else             {setDim(ptex->getWidth(),ptex->getHeight());}
	}
	void setDim(Vec2 d){spProg.start();spProg["dim"]=d;spProg.stop();}
	void setDim(float w,float h){Vec2 dm={w,h};setDim(dm);}
	void setDim(int w,int h){setDim(float(w),float(h));}
	Mat filter(Mat m){
		setImage(m);
		render();
		return downloadResult();
	}
	Mat downloadResult(){
		Mat r=pFbo->toOpenCV32FC4();
		return r;
	}
	void render(){
		//cout << "pTex "<<pTex->id()<<":"<<pTex->getWidth()<<","<<pTex->getHeight()<<endl;
		//cout << "pFbo "<<pFbo->getColorTex().id()<<":"<<pFbo->getWidth()<<","<<pFbo->getHeight()<<endl;
		//Blend mix A channel with RGB
		glDisable(GL_BLEND);
		pFbo->bind();
		spProg.start();
		pTex->bind();
		pVao->bindAll();
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
		pVao->unbindAll();
		pTex->unbind();
		spProg.stop();
		pFbo->unbind();
		glEnable(GL_BLEND);
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




