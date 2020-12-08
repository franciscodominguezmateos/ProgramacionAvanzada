/*
 * shader_point_cloud.h
 *
 *  Created on: 07 Dec 2020
 *      Author: Francisco Dominguez
 *  To work with 3D points and color NOT normal for the moment
 */
#pragma once
#include "shader.h"
#include "texture.h"
#include "solid.h"

class ShaderPointCloud : public Solid{
	vector<GLfloat> vertices;
	vector<GLfloat> colors;
	static string vertexShader;
	static string fragmentShader;
	GLSLShaderProgram* pSpProg;
	GLSLVAO* pstVao;
public:
	ShaderPointCloud(){hazFija();}
	void init(){
		pSpProg=new GLSLShaderProgram();
		pSpProg->compileFromStrings(vertexShader,fragmentShader);
		pstVao=new GLSLVAO();
		pstVao->init();
		pstVao->createAttribute(0,vertices,3);
		pstVao->createAttribute(1,colors,3);
	}
	GLSLShaderProgram* getShaderProgram(){return pSpProg;}
	void addVertex(Vector3D v){
		vertices.push_back(v.getX());
		vertices.push_back(v.getY());
		vertices.push_back(v.getZ());
	}
	void addColor(Vector3D v){
		colors.push_back(v.getX());
		colors.push_back(v.getY());
		colors.push_back(v.getZ());
	}
	void render(){
		GLSLShaderProgram &spProg=*pSpProg;
		glPointSize(5);
		pSpProg->start();
		spProg["T"]=posEulerAnglesToTransformationMatrix<float>(getPos(),getRot());
		pstVao->bindAll();
		glDrawArrays(GL_POINTS, 0, vertices.size());
		pstVao->unbindAll();
		pSpProg->stop();
	}
};
//This vertex does nothing
string ShaderPointCloud::vertexShader=R"glsl(
 #version 330 core
 in vec3 position;
 in vec3 in_color;
 
 out vec3 pass_color;
 
 uniform mat4 T;
 uniform mat4 cameraView;
 uniform mat4 projection;
 
 void main(){
  vec4 worldPosition=T*vec4(position,1);
  gl_Position=projection*cameraView*worldPosition;
  pass_color=in_color;
}
)glsl";

string ShaderPointCloud::fragmentShader=R"glsl(
#version 330 core
in vec3 pass_color;

out vec4 out_color;

void main(){
  out_color=vec4(pass_color,1);
}
)glsl";

