/*
 * sky_box.h
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
		Vec2 dim={640*2,480};
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

void main(){
    vec2 texcoord = gl_FragCoord.xy /dim;
    if(texcoord.x>0.5){
        out_color=vec4(pixel,1); 
    }
    else{
        out_color = texture(tex,texcoord);
    }
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




