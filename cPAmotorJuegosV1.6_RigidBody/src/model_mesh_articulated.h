/*
 * model_mesh_articulated.h
 *
 *  Created on: 30 Mar 2020
 *      Author: francisco
 */
#pragma once
#include "model_mesh.h"
#include "model_joint.h"
class ModelMeshArticulated:public ModelMesh {
	vector<GLuint>  joints;
	vector<GLfloat> weights;
	Joint jointsRoot;
	vector<string>  jointNames;
	map<string,int> jointIdx;
public:
	inline vector<GLuint>  &getJoints() {return joints;}
	inline vector<GLfloat> &getWeights(){return weights;}
	inline void setJointNames(vector<string> &jn){
		jointNames=jn;
		for(unsigned int i=0;i<jn.size();i++) jointIdx[jn[i]]=i;
	}
	inline vector<string> &getJointNames(){return jointNames;}
	inline int getJointIdx(string name){return jointIdx[name];}
	inline void addJoint (GLuint jIdx){joints.push_back(jIdx);}
	inline void addWeight(GLfloat w)  {weights.push_back(w);}
	inline void setJointsRoot(Joint j){jointsRoot=j;}
	inline Joint &getJointsRoot(){return jointsRoot;}
};

