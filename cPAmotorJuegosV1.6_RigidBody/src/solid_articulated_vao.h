/*
 * model_vao.h
 *
 *  Created on: 30 Mar 2020
 *      Author: Francisco Dominguez
 */
#pragma once
#include "shader.h"
#include "model_mesh_articulated.h"
#include "solid_articulated.h"
class SolidArticulatedVAO: SolidArticulated{
	GLSLShaderProgram* shaderProgram;
	GLSLVAO *vao;
	Uniform jointTransforms;
	bool isMaster;
	SolidArticulatedVAO(GLSLShaderProgram* p,GLSLVAO *pvao=nullptr):shaderProgram(p),vao(pvao),jointTransforms("jointTransforms"),isMaster(false){
		jointTransforms.setLocation(shaderProgram->id());
		if(vao==nullptr){
			//this is the master instance the owner of vao
			vao=new GLSLVAO();
			isMaster=true;
		}
	}
	void init(ModelMeshArticulated &pm){
		ModelMeshArticulated m;//=pm.buildShaderReadyMeshModel();
		vao->init();
		vao->createIndexBuffer(m.getIvertices());
		vector<GLfloat> vf=m.getFloatFromVertices();
		vao->createAttribute(0,vf,3);
		vector<GLfloat> vt=m.getFloatFromTextures();
		vao->createAttribute(1,vt,2);
		vector<GLfloat> vn=m.getFloatFromNormals();
		vao->createAttribute(2,vn,3);
		vector<GLuint> vj=m.getJoints();
		vao->createAttribute(3,vj,3);
		vector<GLfloat> vw=m.getWeights();
		vao->createAttribute(4,vw,3);
	}
	void render(){
		shaderProgram->start();
	    vao->bindAll();
		//T=posEulerAnglesToTransformationMatrix<float>(getPos(),getRot());
		glDrawElements(GL_TRIANGLES, vao->getIndexCount(), GL_UNSIGNED_INT,0); // Empezar desde el vértice 0S; 3 vértices en total -> 1 triángulo
		vao->unbindAll();
		shaderProgram->stop();
	}
	GLSLVAO* getPtrVAO(){return vao;}
};

