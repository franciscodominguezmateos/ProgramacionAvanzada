/*
 * shader_gpgpu_binop.h
 *
 *  Created on: 16 Jan 2021
 *      Author: Francisco Dominguez
 *  Abstraction of a pixelwise binary operation
 */
#pragma once
#include "shader_gpgpu_map.h"
const string fragmentShaderBinOp=R"glsl(
#version 330 core

in vec3 pixel;

out vec4 out_color;

uniform sampler2D tex;
uniform sampler2D tex1;
uniform vec2 dim;

//binary operations to apply
vec4 add4(vec4 v0,vec4 v1){return v0+v1;}
vec4 sub4(vec4 v0,vec4 v1){return v0-v1;}
vec4 mul4(vec4 v0,vec4 v1){return v0*v1;}
vec4 div4(vec4 v0,vec4 v1){return v0/v1;}
vec4 mix4(vec4 v0,vec4 v1){const float s=0.5;return v0*s+v1*(1.0-s);}
vec4 greenScreen(vec4 v0,vec4 v1){
 if(v1==vec4(0,0,0,1)) return v0;
 return v1;
}
//texture tex is image to remap, tex1 coordinates in x and y
//v0 is not used and uv=v1.xy/dim is the normalized coord to take the color from tex
vec4 remap(vec4 v0,vec4 v1){
  vec2 uv=v1.xy/dim;
  return texture(tex,vec2(uv.x,1.0-uv.y));
}
vec4 opt (vec4 v0,vec4 v1){return (v0+v1)/2.0;}

void main(){
 vec2 p=gl_FragCoord.xy/dim;
 vec4 c0=texture(tex ,p);
 vec4 c1=texture(tex1,p);
 out_color=opt(c0,c1);
}
)glsl";
class ShaderBinOp:public ShaderMap {
	TexturePtr pTex1;
public:
	ShaderBinOp(Texture* ptex=nullptr,int w=640,int h=480,string oc="vec4 opt(vec4 v0,vec4 v1){return greenScreen(v0,v1);}"):
		ShaderMap(ptex,w,h){
		optCode=oc;
		init();
		workOutDim(ptex,w,h);
	}
	void init(){
		fragmentShader=fragmentShaderBinOp;
		string &fs=fragmentShader;
		fs=replaceLinesIfContains("vec4 opt" ,fs,optCode);
		cout <<fs<<endl;
		spProg.compileFromStrings(vertexShader,fs);
		spProg.start();
		spProg["tex" ]=0;//GL_TEXTURE0
		spProg["tex1"]=1;//GL_TEXTURE1
		spProg.stop();
	}
	void setTex0(TexturePtr tex){pTex =tex;}
	void setTex1(TexturePtr tex){pTex1=tex;}
	void render(){
		//glDepthMask(GL_FALSE);
		//Blend mix A channel with RGB
		glDisable(GL_BLEND);
		pFbo->bind();
		spProg.start();
		glActiveTexture(GL_TEXTURE0);
		pTex->bind();
		glActiveTexture(GL_TEXTURE1);
		pTex1->bind();
		pVao->bindAll();
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
		pVao->unbindAll();
		pTex->unbind();
		pTex->unbind();
		spProg.stop();
		pFbo->unbind();
		glEnable(GL_BLEND);
		//glDepthMask(GL_TRUE);
		glActiveTexture(GL_TEXTURE0);
	}
};

