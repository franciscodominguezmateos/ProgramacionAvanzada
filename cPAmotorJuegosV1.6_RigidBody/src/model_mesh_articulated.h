/*
 * model_mesh_articulated.h
 *
 *  Created on: 30 Mar 2020
 *      Author: francisco
 */

#ifndef MODEL_MESH_ARTICULATED_H_
#define MODEL_MESH_ARTICULATED_H_
#include "model_mesh.h"

class Joint{
	GLuint idx;
	string name;
	vector<Joint> children;
	// final glonal animation transform
	Mat animatedTransform;
	// local initial/bind transformation with respect to parent
	Mat localBindTransform;
	// inverse of global initial/bind transformation with respect to model origin transform
	Mat inverseBindTransform;
public:
	Joint(){};
	Joint(GLuint idx,string name,Mat localBindTransform):idx(idx),name(name),localBindTransform(localBindTransform){}
	void addChild(Joint child){
		children.push_back(child);
	}
	void calcInverseBindTransform(Mat parentBindTransform){
		Mat bindTransform=localBindTransform*parentBindTransform;
		inverseBindTransform=bindTransform.inv();
		for(Joint &j:children)
			j.calcInverseBindTransform(bindTransform);
	}
};
class ModelMeshArticulated:public ModelMesh {
	vector<GLuint>  joints;
	vector<GLfloat> weights;
	Joint jointsRoot;
public:
	inline void addJoint (GLuint jIdx){joints.push_back(jIdx);}
	inline void addWeight(GLfloat w)  {weights.push_back(w);}
	inline void setJointsRoot(Joint j){jointsRoot=j;}
	inline Joint &getJointsRoot(){return jointsRoot;}
};

#endif /* MODEL_MESH_ARTICULATED_H_ */
