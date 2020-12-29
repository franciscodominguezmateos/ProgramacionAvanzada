/*
 * shader_toy.h
 *
 *  Created on: 07 Dec 2020
 *      Author: Francisco Dominguez
 *  It is a fragment Shader it should seem like Iñigo Quilez Shader toy
 */
#pragma once
#include "shader.h"
#include "texture.h"
#include "solid.h"

class ShaderToy : public Solid{
	static vector<GLfloat> vertices;
	static string vertexShader;
	static string fragmentShader;
	GLSLShaderProgram spProg;
	GLSLVAO* pstVao;
	Texture* tex;
public:
	ShaderToy():tex(new Texture()){
		spProg.compileFromStrings(vertexShader,fragmentShader);
		pstVao=new GLSLVAO();
		pstVao->init();
		pstVao->createAttribute(0,vertices,3);
		tex->init();
	}
	Texture* getTexture(){return tex;}
	void setImage(Mat img){
		tex->setImage(img);
		//Vec2 dim={float(img.cols),float(img.rows)};
		Vec2 dim={img.cols,img.rows};
		setDim(dim);
	}
	void setDim(Vec2 d){spProg.start();spProg["dim"]=d;spProg.stop();}
	void render(){
		glDepthMask(GL_FALSE);
		spProg.start();
		tex->bind();
		pstVao->bindAll();
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
		pstVao->unbindAll();
		tex->unbind();
		spProg.stop();
		glDepthMask(GL_TRUE);
	}
};
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

string ShaderToy::fragmentShader=R"glsl(
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
 out_color=vec4(c.rgb*20,1);
}
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




