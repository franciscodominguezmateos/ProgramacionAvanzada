/*
 * model_joint.h
 *
 *  Created on: 2 Apr 2020
 *      Author: Francisco Dominguez
 */
#pragma once
#include <vector>
#include <map>
#include <string>
#include <opencv2/opencv.hpp>

// SkeletonPose is all local transformations for a hole skeleton, in this case map<string,Mat>
// since they are indexed by name
typedef map<string,Mat> SkeletonPose;

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
	void doUniformScale(float s){
		localBindTransform=uniformScaleTransform(localBindTransform,s);
		bindTransform=uniformScaleTransform(bindTransform,s);
		inverseBindTransform=bindTransform.inv();
		for(Joint &j:children)
			j.doUniformScale(s);
	}
	//localBindTransform are fine work out other transformations
	void calcInverseBindTransform(Mat parentBindTransform){
		bindTransform=parentBindTransform*localBindTransform;
		inverseBindTransform=bindTransform.inv();
		animatedTransform=Mat::eye(4,4,CV_32F);
		for(Joint &j:children)
			j.calcInverseBindTransform(bindTransform);
	}
	//given InverseBindTransforms (from DAEs files) work out all other transforms
	void loadInverseBindTransforms(vector<Mat> &ibts,Mat &parentBindTransform){
		Mat &ibt=ibts[getIdx()];
		setInverseBindTransform(ibt);
		Mat lbt=ibt*parentBindTransform;
		setLocalBindTransform(lbt);
		animatedTransform=Mat::eye(4,4,CV_32F);
		for(Joint &j:getChildren())
			j.loadInverseBindTransforms(ibts,getBindTransform());
	}
	void fillAnimatedTransforms(vector<Mat> &jm){
		Joint &joint=*this;
		jm[joint.getIdx()]=joint.getAnimatedTransform();
		for(Joint &j:joint.getChildren()){
			j.fillAnimatedTransforms(jm);
		}
	}
	void fillJointTransforms(vector<Mat> &jm){
		Joint &joint=*this;
		jm[joint.getIdx()]=joint.getAnimatedTransform()* joint.getBindTransform();
		for(Joint &j:joint.getChildren()){
			j.fillJointTransforms(jm);
		}
	}
	SkeletonPose getLocalBindPose(){
		SkeletonPose sp;
		addLocalJoint(sp);
		return sp;
	}
	void addLocalJoint(SkeletonPose &sp){
		sp[getName()]=getLocalBindTransform();
		for(Joint &j:getChildren()){
			j.addLocalJoint(sp);
		}
	}
	void applyPose(SkeletonPose &pose){
		Joint &thisJoint=*this;
		Mat I=Mat::eye(4,4,CV_32F);
		thisJoint.applyPose2Joints(pose,I);
	}
	void applyPose2Joints(SkeletonPose &pose,Mat &currentParentTransform){
		Joint &joint=*this;
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
			j.applyPose2Joints(pose,currentTransform);
	}

	inline vector<Joint> &getChildren()  {return children;}
	inline GLuint getIdx()               {return idx;}
	inline string &getName()             {return name;}
	inline Mat &getAnimatedTransform()   {return animatedTransform;}
	inline Mat &getInverseBindTransform(){return inverseBindTransform;}
	inline Mat &getBindTransform()       {return bindTransform;}
	inline Mat &getLocalBindTransform()  {return localBindTransform;}
	inline void setAnimatedTransform(Mat &m){animatedTransform=m;}
	inline void setInverseBindTransform(Mat &m){inverseBindTransform=m;bindTransform=m.inv();}
	inline void setBindTransform(Mat &m){bindTransform=m;inverseBindTransform=m.inv();}
	inline void setLocalBindTransform(Mat &m){localBindTransform=m;}
};
