/*
 * model_joint.h
 *
 *  Created on: 2 Apr 2020
 *      Author: Francisco Dominguez
 */
#pragma once
class Joint{
	// idx is the VAO index
	GLuint idx;
	string name;
	vector<Joint> children;
	// ****************** INITIAL/BIND TRANSFORMS
	// local initial/bind transfIormation with respect to parent
	Mat localBindTransform;
	// global initial/bind transformation with respect to model origin transform
	Mat bindTransform;
	// inverse of global initial/bind transformation with respect to model origin transform
	Mat inverseBindTransform;
	// ****************** FINAL TRANSFORMS
	// final increment global animation transform from bindTransform to transform
	// transform=animatedTransform*binTransform;
	Mat animatedTransform;
public:
	Joint(){};
	Joint(GLuint idx,string name,Mat pLocalBindTransform):
		idx(idx),
		name(name),
		localBindTransform(pLocalBindTransform.clone()){}
	void addChild(Joint child){children.push_back(child);}
	void calcInverseBindTransform(Mat parentBindTransform){
		bindTransform=parentBindTransform*localBindTransform;
		inverseBindTransform=bindTransform.inv();
		animatedTransform=Mat::eye(4,4,CV_32F);
		for(Joint &j:children)
			j.calcInverseBindTransform(bindTransform);
		/*cout<<"********" << name << "*********" <<endl;
		cout << "pbT"<<parentBindTransform<<endl;
		cout << "lbT"<<localBindTransform<<endl;
		cout << "bT"<<bindTransform<<endl;*/
	}
	vector<Mat> getJointTransforms(){
		vector<Mat> vm(16);
		addJoints(*this,vm);
		return vm;
	}
	void addJoints(Joint &joint,vector<Mat> &jm){
		jm[joint.getIdx()]=joint.getAnimatedTransform();
		for(Joint &j:joint.getChildren()){
			addJoints(j,jm);
		}
	}
	inline vector<Joint> &getChildren(){return children;}
	inline GLuint getIdx(){return idx;}
	inline string &getName(){return name;}
	inline Mat &getAnimatedTransform(){return animatedTransform;}
	inline Mat &getInverseBindTransform(){return inverseBindTransform;}
	inline Mat &getBindTransform(){return bindTransform;}
	inline void setAnimatedTransform(Mat &m){animatedTransform=m;}
};
