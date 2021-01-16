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

//reduce operation to apply
vec4 add(vec4 v0,vec4 v1){return v0+v1;}
vec4 sub(vec4 v0,vec4 v1){return v0-v1;}
vec4 mul(vec4 v0,vec4 v1){return v0*v1;}
vec4 div(vec4 v0,vec4 v1){return v0/v1;}
vec4 mix(vec4 v0,vec4 v1){const float s=0.5;return v0*s+(1.0-s)*v1;}
vec4 opt(vec4 v0,vec4 v1){return v0+v1;}

void main(){
 vec2 p=gl_FragCoord/dim;
 out_color=opt(texture(tex,p),texture(tex1,p));
}
)glsl";
class ShaderBinOp:public ShaderMap {
	TexturePtr pTex1;
public:
	ShaderBinOp(Texture* ptex=nullptr,int w=640,int h=480,string oc="vec4 opt(vec4 v){return mix(v0,v1);}"):
		ShaderMap(ptex,w,h,oc){}
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
		pTex->bind();
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

