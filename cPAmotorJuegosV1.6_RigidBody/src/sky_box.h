/*
 * sky_box.h
 *
 *  Created on: 29 Apr 2020
 *      Author: Francisco Dominguez
 */
#pragma once
#include "shader.h"

class SkyBox {
	static vector<GLfloat> skyboxVertices;
	static string vertexShader;
	static string fragmentShader;
	GLSLShaderProgram spSkyBox;
	Uniform spsCameraView;
	Uniform spsProjection;
	GLSLVAO* pskVao;
	TextureCube* tcube;
public:
	SkyBox():spsCameraView("cameraView"),spsProjection("projection"){
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
		//spSkyBox.compileFromFileNames("src/SimpleSkyBoxVertex.glsl",
		//		                      "src/SimpleSkyBoxFragment.glsl");
		spSkyBox.compileFromStrings(vertexShader,fragmentShader);
		spSkyBox.setUniforms({"cameraView","projection"});
		spSkyBox.start();
		//spsProjection=projectionMat;
		spSkyBox.stop();
		pskVao=new GLSLVAO();
		pskVao->init();
		pskVao->createAttribute(0,skyboxVertices,3);
	}
	void setProjection(Mat p){spSkyBox.start();spSkyBox["projection"]=p;spSkyBox.stop();}
	void setCameraView(Mat c){spSkyBox.start();spSkyBox["cameraView"]=c;spSkyBox.stop();}
	void render(){
		glDepthMask(GL_FALSE);
		spSkyBox.start();
	    //spsCameraView=cameraViewMat;
		tcube->bind();
		pskVao->bindAll();
		glDrawArrays(GL_TRIANGLES, 0, 36);
		pskVao->unbindAll();
		tcube->unbind();
		spSkyBox.stop();
		glDepthMask(GL_TRUE);
	}
};
string SkyBox::vertexShader=
"#version 330 core\n"
"in vec3 position;\n"
"\n"
"out vec3 pass_textureCoords;\n"
"\n"
"uniform mat4 cameraView;\n"
"uniform mat4 projection;\n"
"\n"
"void main(){\n"
"\n"
" mat4 cv=cameraView;\n"
" cv[0][3]=0;\n"
" cv[1][3]=0;\n"
" cv[2][3]=0;\n"
" vec3 p=position;\n"
" p*=1000;\n"
" gl_Position=projection*cv*vec4(p,1);\n"
" pass_textureCoords=position;\n"
""
"}";
string SkyBox::fragmentShader=
"#version 330 core\n"
"in vec3 pass_textureCoords;\n"
"\n"
"out vec4 out_color;\n"
"\n"
"uniform samplerCube cubeMap;\n"
"\n"
"void main(){\n"
"out_color = texture(cubeMap,pass_textureCoords);\n"
"}\n";
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


