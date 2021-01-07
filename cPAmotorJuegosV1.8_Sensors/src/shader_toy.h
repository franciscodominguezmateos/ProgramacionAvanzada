/*
 * shader_toy.h
 *
 *  Created on: 07 Dec 2020
 *      Author: Francisco Dominguez
 *  It is a fragment Shader it should seem like Iñigo Quilez Shader toy
 *  06/01/2021 Parse shader toy code to define uniform parameters properly.
 *  Parameters allowed: "iTime","iTimeDelta","iFrame","iFrameRate","iMouse","iResolution"
 *  Trick: in order to make originals shaders to work just ad a space after one iTime and one iFrame
 */
#pragma once
#include "shader.h"
#include "texture.h"
#include "solid.h"
const string shaderToyCode=R"glsl(
// Created by inigo quilez - iq/2013
// License Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License.

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
	vec2 p = (2.0*fragCoord-iResolution.xy)/min(iResolution.y,iResolution.x);
	
    // background color
    vec3 bcol = vec3(1.0,0.8,0.7-0.07*p.y)*(1.0-0.25*length(p));

    // animate
    float tt = mod(iTime ,1.5)/1.5;
    float ss = pow(tt,.2)*0.5 + 0.5;
    ss = 1.0 + ss*0.5*sin(tt*6.2831*3.0 + p.y*0.5)*exp(-tt*4.0);
    p *= vec2(0.5,1.5) + ss*vec2(0.5,-0.5);

    // shape
#if 0
    p *= 0.8;
    p.y = -0.1 - p.y*1.2 + abs(p.x)*(1.0-abs(p.x));
    float r = length(p);
	float d = 0.5;
#else
	p.y -= 0.25;
    float a = atan(p.x,p.y)/3.141593;
    float r = length(p);
    float h = abs(a);
    float d = (13.0*h - 22.0*h*h + 10.0*h*h*h)/(6.0-5.0*h);
#endif
    
	// color
	float s = 0.75 + 0.75*p.x;
	s *= 1.0-0.4*r;
	s = 0.3 + 0.7*s;
	s *= 0.5+0.5*pow( 1.0-clamp(r/d, 0.0, 1.0 ), 0.1 );
	vec3 hcol = vec3(1.0,0.5*r,0.3)*s;
	
    vec3 col = mix( bcol, hcol, smoothstep( -0.01, 0.01, d-r) );

    fragColor = vec4(col,1.0);
})glsl";


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
public:
	ShaderToy():Solid(),
				sTime(chrono::steady_clock::now()),
				iTime(0),
				iTimeDelta(0.33),
				iFrame(0),
				iFrameRate(30),
				iChannel0(new Texture()){
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
		if(contains(stCode,"iTime "))     {sout<<"uniform float iTime;"     <<endl;declared["iTime"]      =true;}
		if(contains(stCode,"iTimeDelta")) {sout<<"uniform float iTimeDelta;"<<endl;declared["iTimeDelta"] =true;}
		//stCode has to have a space after iFrame to differenciate from iFrameRate
		if(contains(stCode,"iFrame "))    {sout<<"uniform int iFrame;"      <<endl;declared["iFrame"]     =true;}
		if(contains(stCode,"iFrameRate")) {sout<<"uniform float iFrameRate;"<<endl;declared["iFrameRate"] =true;}
		if(contains(stCode,"iMouse"))     {sout<<"uniform vec4 iMouse;"     <<endl;declared["iMouse"]     =true;}
		if(contains(stCode,"iResolution")){sout<<"uniform vec3 iResolution;"<<endl;declared["iResolution"]=true;}
		string sr=sout.str();
		cout <<"Uniforms infered from shader toy code:"<<endl<<sr<<endl;
		return sr;
	}
	void setiMouse(const Vec4 v){iMouse=v;}
	void setFragmentShader(const string &fs){
		string fragmentShader=fragmentShaderBaseBegin+inferUniformDeclarations(fs)+fs+fragmentShaderBaseEnd;
		spProg.compileFromStrings(vertexShader,fragmentShader);
	}
	Texture* getiChannel0(){return iChannel0;}
	void setImage(Mat img){
		iChannel0->setImage(img);
		//Vec2 dim={float(img.cols),float(img.rows)};
		Vec3 dim={(float)img.cols,(float)img.rows,1.0};
		setiResolution(dim);
	}
	void setiResolution(Vec3 d){iResolution=d;spProg.start();spProg["iResolution"]=d;spProg.stop();}
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
		glDepthMask(GL_FALSE);
		spProg.start();
		setDynamicParams();
		iChannel0->bind();
		pstVao->bindAll();
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
		pstVao->unbindAll();
		iChannel0->unbind();
		spProg.stop();
		glDepthMask(GL_TRUE);
		iTimeDelta=iTime-getTimeFromStartInSeconds();
		iFrame+=1;
		iFrameRate=1.0/iTimeDelta;
		//cout << "iTimeDelta="<< iTimeDelta<<"iFrameRate="<<iFrameRate<<endl;
	}
};
vector<string> ShaderToy::parameters={"iTime","iTimeDelta","iFrame","iFrameRate","iMouse","iResolution"};
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




