/*
 * model_joint.h
 *
 *  Created on: 2 Apr 2020
 *      Author: Francisco Dominguez
 */
#pragma once

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
	inline string &getName(){return name;}
	inline Mat &getAnimatedTransform(){return animatedTransform;}
	inline Mat &getInverseBindTransform(){return inverseBindTransform;}
	inline void setAnimatedTransform(Mat &m){animatedTransform=m.clone();}
};
