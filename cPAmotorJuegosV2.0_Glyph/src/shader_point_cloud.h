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
	Mat T;
public:
	void setT(Mat t){T=t;}
	ShaderPointCloud(){
		hazFija();
		T=posEulerAnglesToTransformationMatrix<float>(getPos(),getRot());
	}
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
	void addVertices(vector<Vector3D> &vv){
		for(Vector3D &p:vv){
			addVertex(Vector3D(p.getX(),-p.getY(),-p.getZ()));
		}
	}
	void addColor(Vector3D v){
		colors.push_back(v.getX());
		colors.push_back(v.getY());
		colors.push_back(v.getZ());
	}
	void addColors(vector<Vector3D> &vv){
		for(Vector3D &c:vv)	addColor(c);
	}
	void render(){
		GLSLShaderProgram &spProg=*pSpProg;
		glPointSize(2);
		pSpProg->start();
		//Mat T=posEulerAnglesToTransformationMatrix<float>(getPos(),getRot());
		spProg["T"]=T;
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

