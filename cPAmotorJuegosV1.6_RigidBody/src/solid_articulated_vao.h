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
	//increment transform from
	// global bind pose to current global pose
	vector<Mat> animatedTransforms;
	bool isMaster;
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
	void setShaderProgram(GLSLShaderProgram *p){shaderProgram=p;}
	void init(ModelMeshArticulated &pm){
		SolidArticulated::init(pm);
		animatedTransforms=vector<Mat>(getJointNames().size());
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
	void setPose(SkeletonPose &currentPose){
		SolidArticulated::setPose(currentPose);
		getJointsRoot().fillAnimatedTransforms(animatedTransforms);
	}
	void applyPose2Joints(SkeletonPose &pose,Joint &joint,Mat &currentParentTransform){
		//not all articulations have to be animated
		Mat currentLocalTransform;
		if(pose.count(joint.getName())>0)
			currentLocalTransform=pose[joint.getName()];
		//if not articulation on pose just take the default localBindTransform
		else
			currentLocalTransform=joint.getLocalBindTransform();
		Mat currentTransform  =currentParentTransform*currentLocalTransform;
		Mat animationTransform=currentTransform*joint.getInverseBindTransform();
		joint.setAnimatedTransform(animationTransform);
		for(Joint &j:joint.getChildren())
			applyPose2Joints(pose,j,currentTransform);
	}
	void render(){
		//SolidArticulated::render();
		GLSLShaderProgram &sp=*shaderProgram;
		sp.start();
	    vao->bindAll();
	    sp["jointT"]=animatedTransforms;
		sp["T"]=posEulerAnglesToTransformationMatrix<float>(getPos(),getRot());
		glDrawElements(GL_TRIANGLES, vao->getIndexCount(), GL_UNSIGNED_INT,0); // Empezar desde el vértice 0S; 3 vértices en total -> 1 triángulo
		vao->unbindAll();
		sp.stop();
	}
	GLSLVAO* getPtrVAO(){return vao;}
};

