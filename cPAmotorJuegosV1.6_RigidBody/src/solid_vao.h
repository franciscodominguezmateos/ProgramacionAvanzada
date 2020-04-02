/*
 * solid_vao.h
 *
 *  Created on: 2 Apr 2020
 *      Author: Francisco Dominguez
 */
#pragma once
#include "solido.h"
#include "shader.h"
#include "model_mesh.h"
/*
 * The point of this class is to use one instance as master, the owner of the pointer vao
 * and other clones are used to share the vao attribute of the master instance.
 * Then we can render lots of different objects with the same vao.
 */
class SolidVAO: public Solido {
	GLSLShaderProgram* shaderProgram;
	GLSLVAO* vao;
	Uniform T;
	bool isMaster;
public:
	SolidVAO(GLSLShaderProgram* p,GLSLVAO *pvao=nullptr):shaderProgram(p),vao(pvao),T("T"),isMaster(false){
		T.setLocation(shaderProgram->id());
		if(vao==nullptr){
			//this is the master instance the owner of vao
			vao=new GLSLVAO();
			isMaster=true;
		}
	}
	virtual ~SolidVAO(){
		if(isMaster) delete vao;
	}
	SolidVAO *clone(){return new SolidVAO(*this);}
	// This only have to call on the master instance
	void init(ModelMesh &pm){
	    ModelMesh m=pm.buildShaderReadyMeshModel();
		vao->init();
		vao->createIndexBuffer(m.getIvertices());
		vector<GLfloat> vf=m.getFloatFromVertices();
		vao->createAttribute(0,vf,3);
		vector<GLfloat> vt=m.getFloatFromTextures();
		vao->createAttribute(1,vt,2);
		vector<GLfloat> vn=m.getFloatFromNormals();
		vao->createAttribute(2,vn,3);
	}
	void render(){
		shaderProgram->start();
	    vao->bindAll();
		T=posEulerAnglesToTransformationMatrix(getPos(),getRot());
		glDrawElements(GL_TRIANGLES, vao->getIndexCount(), GL_UNSIGNED_INT,0); // Empezar desde el vértice 0S; 3 vértices en total -> 1 triángulo
		vao->unbindAll();
		shaderProgram->stop();
	}
	GLSLVAO* getPtrVAO(){return vao;}
};

