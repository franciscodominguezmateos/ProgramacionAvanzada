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
		tcube->init();
		spSkyBox.compileFromFileNames("src/SimpleSkyBoxVertex.glsl",
				                      "src/SimpleSkyBoxFragment.glsl");
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


