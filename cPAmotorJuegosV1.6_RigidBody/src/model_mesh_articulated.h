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
	// local initial/bind transfIormation with respect to parent
	Mat localBindTransform;
	// inverse of global initial/bind transformation with respect to model origin transform
	Mat inverseBindTransform;
public:
	Joint(){};
	Joint(GLuint idx,string name,Mat pLocalBindTransform):idx(idx),name(name){
		pLocalBindTransform.copyTo(localBindTransform);
	}
	void addChild(Joint child){
		children.push_back(child);
	}
	void calcInverseBindTransform(Mat parentBindTransform){
		Mat bindTransform=parentBindTransform*localBindTransform;
		cout<<"********" << name << "*********" <<endl;
		cout << "pbT"<<parentBindTransform<<endl;
		cout << "lbT"<<localBindTransform<<endl;
		cout << "bT"<<bindTransform<<endl;
		animatedTransform=bindTransform;
		inverseBindTransform=bindTransform.inv();
		for(Joint &j:children)
			j.calcInverseBindTransform(bindTransform);
	}
	inline vector<Joint> &getChildren(){return children;}
	inline GLuint getIdx(){return idx;}
	inline Mat getAnimatedTransform(){return animatedTransform;}
};
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

#endif /* MODEL_MESH_ARTICULATED_H_ */
