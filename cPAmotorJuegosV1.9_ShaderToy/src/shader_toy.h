/*
 * shader_toy.h
 *
 *  Created on: 07 Dec 2020
 *      Author: Francisco Dominguez
 *  It is a fragment Shader it should seem like Iñigo Quilez Shader toy
 *  06/01/2021 Parse shader toy code to define uniform parameters properly.
 *  Parameters allowed: "iTime","iTimeDelta","iFrame","iFrameRate","iMouse","iResolution"
 *  Trick: in order to make originals shadertoy.com shaders to work just add a space after one iTime and one iFrame
 */
#pragma once
#include "shader.h"
#include "texture.h"
#include "solid.h"
#define STRINGIFY(A) #A
//STRINGIFY doesn't work with / operator division¿?¿?¿?
const string hello=STRINGIFY(
		float f(vec3 p) {
			p.z+=iTime ;return length(.05*cos(9.*p.y*p.x)+cos(p)-.1*cos(9.*(p.z+.3*p.x-p.y)))-1.;
		}
void mainImage( out vec4 c, vec2 p ){
	vec3 d=.5-vec3(p,1)//iResolution.x,o=d;
    for(int i=0;i<128;i++)o+=f(o)*d;
    c = vec4(abs(f(o-d)*vec3(0,1,2)+f(o-.6)*vec3(2,1,0))*(1.-.1*o.z),1.0);
}
);
// Created by inigo quilez - iq/2013 https://www.shadertoy.com/view/MsfGzM
// License Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License.
// Name: Two Tweets
const string shaderToyCode=R"glsl(

float f(vec3 p) { 
	p.z+=iTime ;return length(.05*cos(9.*p.y*p.x)+cos(p)-.1*cos(9.*(p.z+.3*p.x-p.y)))-1.; 
}
void mainImage( out vec4 c, vec2 p ){
    vec3 d=.5-vec3(p,1)/iResolution.x,o=d;for(int i=0;i<128;i++)o+=f(o)*d;
    c = vec4(abs(f(o-d)*vec3(0,1,2)+f(o-.6)*vec3(2,1,0))*(1.-.1*o.z),1.0);
}
)glsl";
class ShaderToy;
using ShaderToyPtr=ShaderToy*;

class ShaderToy : public Solid{
	static vector<GLfloat> vertices;
	static string vertexShader;
	static string fragmentShaderBaseBegin;
	static string fragmentShaderUniformsDec;//not needed for the moment
	static string fragmentShaderBaseEnd;
	//start time
	chrono::steady_clock::time_point sTime;
	//time from start
	float iTime;
	float iTimeDelta;
	int   iFrame;
	float iFrameRate;
	//xy = current pixel coords (if LMB is down). zw = click pixel
	Vec4  iMouse;
	GLSLShaderProgram spProg;
	GLSLVAO* pstVao;
	Texture* iChannel0;
	//z is aspect ratio, usually 1.0
	Vec3  iResolution;
	static vector<string> parameters;
	map<string,bool> declared;
	bool depthTest;
	GLSLFBOPtr pFbo;
public:
	ShaderToy():Solid(),
				sTime(chrono::steady_clock::now()),
				iTime(0),
				iTimeDelta(0.33),
				iFrame(0),
				iFrameRate(30),
				iChannel0(new Texture()),depthTest(false),pFbo(nullptr){
		pstVao=new GLSLVAO();
		pstVao->init();
		pstVao->createAttribute(0,vertices,3);
		iChannel0->init();
		for(string &p:parameters) declared[p]=false;
	}
	virtual void init(){
		setFragmentShader(shaderToyCode);
		Vec3 dim={640.0f*2,480.0f,1.0f};
		setiResolution(dim);
	}
	virtual void initFromFileName(const string &fileName){
		ifstream ifs(fileName);
		string stCode;
		ifs>>stCode;
		setFragmentShader(stCode);
		Vec3 dim={640.0f*3,480.0f*2,1.0f};
		setiResolution(dim);
	}
	//This doesn't avoid declaration compiler error because compiler is smarter but helps
	string inferUniformDeclarations(const string &stCode){
		ostringstream sout;
		//stCode has to have a space after iTime to differenciate from iTimeDelta
		if(contains(stCode,"iTime "))     {sout<<"uniform float iTime;"        <<endl;declared["iTime"]      =true;}
		if(contains(stCode,"iTimeDelta")) {sout<<"uniform float iTimeDelta;"   <<endl;declared["iTimeDelta"] =true;}
		//stCode has to have a space after iFrame to differenciate from iFrameRate
		if(contains(stCode,"iFrame "))    {sout<<"uniform int iFrame;"         <<endl;declared["iFrame"]     =true;}
		if(contains(stCode,"iFrameRate")) {sout<<"uniform float iFrameRate;"   <<endl;declared["iFrameRate"] =true;}
		if(contains(stCode,"iMouse"))     {sout<<"uniform vec4 iMouse;"        <<endl;declared["iMouse"]     =true;}
		if(contains(stCode,"iResolution")){sout<<"uniform vec3 iResolution;"   <<endl;declared["iResolution"]=true;}
		if(contains(stCode,"iChannel0"))  {sout<<"uniform sampler2D iChannel0;"<<endl;declared["iChannel0"]=true;}
		string sr=sout.str();
		cout <<"Uniforms infered from shader toy code:"<<endl<<sr<<endl;
		return sr;
	}
	inline void setFbo(GLSLFBOPtr fbo){pFbo=fbo;}
	inline GLSLFBOPtr getFbo(){return pFbo;}
	inline void setDepthTest(bool b){depthTest=b;	}
	inline void setiMouse(const Vec4 v){iMouse=v;}
	void setFragmentShader(const string &fs){
		string fragmentShader=fragmentShaderBaseBegin+inferUniformDeclarations(fs)+fs+fragmentShaderBaseEnd;
		spProg.compileFromStrings(vertexShader,fragmentShader);
	}
	GLSLShaderProgram &getShaderProgram(){return spProg;}
	Texture* getiChannel0(){return iChannel0;}
	void setImage(Mat img){
		iChannel0->setImage(img);
		setiResolution(img.cols,img.rows);
	}
	void setiResolution(Vec3 d){iResolution=d;spProg.start();spProg["iResolution"]=d;spProg.stop();}
	void setiResolution(int w,int h,float d=1){
		Vec3 dim={float(w),float(h),d};
		setiResolution(dim);
	}
	void setDynamicParams(){
		//this function is called in render() then not needed to start and stop
		//spProg.start();
		if(declared["iTime"])     spProg["iTime"]     =iTime;
		if(declared["iTimeDelta"])spProg["iTimeDelta"]=iTimeDelta;
		if(declared["iFrame"])    spProg["iFrame"]    =iFrame;
		if(declared["iFrameRate"])spProg["iFrameRate"]=iFrameRate;
		if(declared["iMouse"])    spProg["iMouse"]    =iMouse;
		//spProg.stop();
	}
	inline float getTimeFromStartInSeconds(){return getDurationInSeconds<float>(sTime);}
	void render(){
		iTime=getTimeFromStartInSeconds();
		if(!depthTest)glDepthMask(GL_FALSE);
		//Blend mix A channel with RGB
		glDisable(GL_BLEND);
		if(pFbo!=nullptr) pFbo->bind();
		spProg.start();
		setDynamicParams();
		iChannel0->bind();
		pstVao->bindAll();
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
		pstVao->unbindAll();
		iChannel0->unbind();
		spProg.stop();
		iTimeDelta=iTime-getTimeFromStartInSeconds();
		iFrame+=1;
		iFrameRate=1.0/iTimeDelta;
		if(pFbo!=nullptr) pFbo->unbind();
		//cout << "iTimeDelta="<< iTimeDelta<<"iFrameRate="<<iFrameRate<<endl;
		glEnable(GL_BLEND);
		if(!depthTest)glDepthMask(GL_TRUE);
	}
};
vector<string> ShaderToy::parameters={"iTime","iTimeDelta","iFrame","iFrameRate","iMouse","iResolution","iChannel0"};
//This vertex does nothing
string ShaderToy::vertexShader=R"glsl(
#version 330 core
in vec3 in_vertex;
//Must have same name that in fragmentShader
out vec3 pixel;
void main(){
    pixel=in_vertex/2+0.5;
    gl_Position=vec4(in_vertex,1.0);
}
)glsl";
string ShaderToy::fragmentShaderBaseBegin=R"glsl(
#version 330 core
in vec3 pixel;
out vec4 out_color;
)glsl";
string ShaderToy::fragmentShaderUniformsDec=R"glsl(
uniform float iTime;
uniform float iTimeDelta;
uniform int iFrame;
uniform float iFrameRate;
uniform vec4 iMouse;
uniform vec3 iResolution;
)glsl";
string ShaderToy::fragmentShaderBaseEnd=R"glsl(
void main(){ mainImage(out_color,gl_FragCoord.xy);}
)glsl";
//Just a square
vector<GLfloat> ShaderToy::vertices = {
    // Left bottom triangle
    -1.0f,  1.0f, 0.0f,
    -1.0f, -1.0f, 0.0f,
     1.0f, -1.0f, 0.0f,
	 // Right top triangle
     1.0f, -1.0f, 0.0f,
     1.0f,  1.0f, 0.0f,
    -1.0f,  1.0f, 0.0f,
};




