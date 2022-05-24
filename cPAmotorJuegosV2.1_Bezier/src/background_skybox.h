/*
 * sky_box.h
 *
 *  Created on: 29 Apr 2020
 *      Author: Francisco Dominguez
 */
#pragma once
#include "shader.h"
#include "texture_cube.h"

class SkyBox;
using SkyBoxPtr=SkyBox*;
class SkyBox {
	static vector<GLfloat> skyboxVertices;
	static string vertexShader;
	static string fragmentShader;
	GLSLShaderProgram spSkyBox;
	GLSLVAO* pskVao;
	TextureCube* tcube;
public:
	SkyBox(){
		tcube=new TextureCube();
		tcube->addImage(imread("modelos/skybox/right.jpg"));
		tcube->addImage(imread("modelos/skybox/left.jpg"));
		tcube->addImage(imread("modelos/skybox/top.jpg"));
		tcube->addImage(imread("modelos/skybox/bottom.jpg"));
		tcube->addImage(imread("modelos/skybox/front.jpg"));
		tcube->addImage(imread("modelos/skybox/back.jpg"));
/*		tcube->addImage(imread("modelos/skybox/Hogwarts_Sky_Rt.png"));
		tcube->addImage(imread("modelos/skybox/Hogwarts_Sky_Lf.png"));
		tcube->addImage(imread("modelos/skybox/Hogwarts_Sky_Up.png"));
		tcube->addImage(imread("modelos/skybox/Hogwarts_Sky_Dn.png"));
		tcube->addImage(imread("modelos/skybox/Hogwarts_Sky_Bk.png"));
		tcube->addImage(imread("modelos/skybox/Hogwarts_Sky_Fr.png"));*/
		 /* Test TextureCube with a colour at each face
		Mat mat0(16, 16, CV_8UC3, Scalar(255,0,0));
		tcube->addImage(mat0);
		Mat mat1(16, 16, CV_8UC3, Scalar(0,255,0));
		tcube->addImage(mat1);
		Mat mat2(16, 16, CV_8UC3, Scalar(255,255,0));
		tcube->addImage(mat2);
		Mat mat3(16, 16, CV_8UC3, Scalar(0,0,255));
		tcube->addImage(mat3);
		Mat mat4(16, 16, CV_8UC3, Scalar(255,0,255));
		tcube->addImage(mat4);
		Mat mat5(16, 16, CV_8UC3, Scalar(0,128,255));*/
		tcube->init();

		spSkyBox.compileFromStrings(vertexShader,fragmentShader);

		pskVao=new GLSLVAO();
		pskVao->init();
		pskVao->createAttribute(0,skyboxVertices,3);
	}
	void setProjection(Mat p){spSkyBox.start();spSkyBox["projection"]=p;spSkyBox.stop();}
	void setCameraView(Mat c){spSkyBox.start();spSkyBox["cameraView"]=c;spSkyBox.stop();}
	void render(){
		glDepthMask(GL_FALSE);
		spSkyBox.start();
		tcube->bind();
		pskVao->bindAll();
		glDrawArrays(GL_TRIANGLES, 0, 36);
		pskVao->unbindAll();
		tcube->unbind();
		spSkyBox.stop();
		glDepthMask(GL_TRUE);
	}
};
string SkyBox::vertexShader=R"glsl(
#version 330 core
in vec3 position;

out vec3 pass_textureCoords;

uniform mat4 cameraView;
uniform mat4 projection;

void main(){

 mat4 cv=cameraView;
 cv[0][3]=0;
 cv[1][3]=0;
 cv[2][3]=0;
 vec3 p=position;
 p*=1000;
 gl_Position=projection*cv*vec4(p,1);
 pass_textureCoords=position;
}
)glsl";

string SkyBox::fragmentShader=R"glsl(
#version 330 core

in vec3 pass_textureCoords;

out vec4 out_color;

uniform samplerCube cubeMap;

void main(){
out_color = texture(cubeMap,pass_textureCoords);
}
)glsl";

vector<GLfloat> SkyBox::skyboxVertices = {
    // positions
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};


