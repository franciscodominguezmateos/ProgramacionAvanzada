/*
 * model_vao.h
 *
 *  Created on: 30 Mar 2020
 *      Author: francisco
 */

#ifndef MODEL_VAO_H_
#define MODEL_VAO_H_
#include "solido.h"
#include "shader.h"

class ModelVAO: public Solido {
	GLSLVAO vao;
	GLSLShaderProgram* shaderProgram;
	Uniform T;
public:
	ModelVAO(GLSLShaderProgram* p=nullptr):shaderProgram(p),T("T"){
		T.setLocation(shaderProgram->id());
	}
	void init(ModelMesh &pm){
	    ModelMesh m=pm.buildShaderReadyMeshModel();
		vao.init();
		vao.createIndexBuffer(m.getIvertices());
		vector<GLfloat> vf=m.getFloatFromVertices();
		vao.createAttribute(0,vf,3);
		vector<GLfloat> vt=m.getFloatFromTextures();
		vao.createAttribute(1,vt,2);
		vector<GLfloat> vn=m.getFloatFromNormals();
		vao.createAttribute(2,vn,3);
	}
	void render(){
		shaderProgram->start();
	    vao.bindAll();
		T=posEulerAnglesToTransformationMatrix(getPos(),getRot());
		glDrawElements(GL_TRIANGLES, vao.getIndexCount(), GL_UNSIGNED_INT,0); // Empezar desde el vértice 0S; 3 vértices en total -> 1 triángulo
		vao.unbindAll();
		shaderProgram->stop();
	}
	friend class ModelVAOref;
};
class ModelVAOref: public Solido{
	ModelVAO* mvao;
public:
	ModelVAOref(ModelVAO *v):mvao(v){}
	void render(){
		mvao->shaderProgram->start();
	    mvao->vao.bindAll();
		mvao->T=posEulerAnglesToTransformationMatrix(getPos(),getRot());
		glDrawElements(GL_TRIANGLES, mvao->vao.getIndexCount(), GL_UNSIGNED_INT,0); // Empezar desde el vértice 0S; 3 vértices en total -> 1 triángulo
		mvao->vao.unbindAll();
		mvao->shaderProgram->stop();
	}
};

#endif /* MODEL_VAO_H_ */
