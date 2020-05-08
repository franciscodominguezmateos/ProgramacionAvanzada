/*
 * solid_articulated_vao.h
 *
 *  Created on: 8 Apr 2020
 *      Author: Francisco Dominguez
 */
#pragma once
#include "shader.h"
#include "model_mesh_articulated.h"
#include "solid_articulated.h"
class SolidArticulatedVAO:public SolidArticulated{
	GLSLShaderProgram* shaderProgram;
	GLSLVAO *vao;
	bool isMaster;
	Material material;
public:
	SolidArticulatedVAO(GLSLShaderProgram* p=nullptr,GLSLVAO *pvao=nullptr):shaderProgram(p),vao(pvao),isMaster(false){
		if(vao==nullptr){
			//this is the master instance the owner of vao
			vao=new GLSLVAO();
			isMaster=true;
		}
	}
	virtual ~SolidArticulatedVAO(){
		if(isMaster) delete vao;
	}
	SolidArticulatedVAO *clone(){
		SolidArticulatedVAO *r=new SolidArticulatedVAO(*this);
		r->isMaster=false;
		return r;
	}
	void setMaterial(Material &m){material=m;}
	Material &getMaterial(){return material;}
	void setShaderProgram(GLSLShaderProgram *p){shaderProgram=p;}
	void init(ModelMeshArticulated &pm){
		SolidArticulated::init(pm);
		//getJointsRoot().fillAnimatedTransforms(animatedTransforms);
		ModelMesh m=pm.buildShaderReadyMeshModel();
		vao->init();
		vao->createIndexBuffer(m.getIvertices());
		vector<GLfloat> vf=m.getFloatFromVertices();
		vao->createAttribute(0,vf,3);
		vector<GLfloat> vt=m.getFloatFromTextures();
		vao->createAttribute(1,vt,2);
		vector<GLfloat> vn=m.getFloatFromNormals();
		vao->createAttribute(2,vn,3);
		vector<GLuint> vj=pm.getJoints();
		vao->createAttribute(3,vj,3);
		vector<GLfloat> vw=pm.getWeights();
		vao->createAttribute(4,vw,3);
	}
	void render(){
		SolidArticulated::render();
		GLSLShaderProgram &sp=*shaderProgram;
		material.bind();
		sp.start();
	    vao->bindAll();
	    sp["jointT"]=getAnimatedTransforms();
		sp["T"]=posEulerAnglesToTransformationMatrix<float>(getPos(),getRot());
		glDrawElements(GL_TRIANGLES, vao->getIndexCount(), GL_UNSIGNED_INT,0); // Empezar desde el vértice 0S; 3 vértices en total -> 1 triángulo
		vao->unbindAll();
		sp.stop();
		material.unbind();
	}
	GLSLVAO* getPtrVAO(){return vao;}
};

