/*
 * model_vao.h
 *
 *  Created on: 30 Mar 2020
 *      Author: Francisco Dominguez
 */
#pragma once
#include "solido.h"
#include "shader.h"
class SolidArticulatedVAO: SolidArticulated{
	GLSLVAO vao;
	GLSLShaderProgram* shaderProgram;
	Uniform jointTransforms;
	SolidArticulatedVAO(GLSLShaderProgram* p=nullptr):shaderProgram(p),jointTransforms("jointTransforms"){
		jointTransforms.setLocation(shaderProgram->id());
	}
	void init(ModelMeshArticulated &pm){
		ModelMeshArticulated m;//=pm.buildShaderReadyMeshModel();
		vao.init();
		vao.createIndexBuffer(m.getIvertices());
		vector<GLfloat> vf=m.getFloatFromVertices();
		vao.createAttribute(0,vf,3);
		vector<GLfloat> vt=m.getFloatFromTextures();
		vao.createAttribute(1,vt,2);
		vector<GLfloat> vn=m.getFloatFromNormals();
		vao.createAttribute(2,vn,3);
		vector<GLuint> vj=m.getJoints();
		vao.createAttribute(3,vj,3);//?????????????????????????????????????????????????????????????????
		vector<GLfloat> vw=m.getWeights();
		vao.createAttribute(4,vn,3);
	}
};

