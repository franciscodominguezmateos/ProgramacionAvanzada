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

// SkeletonPose is all local transformations for a whole skeleton, in this case map<string,Mat>
// since they are indexed by name
typedef map<string,Mat> SkeletonPose;
inline ostream &operator<<(ostream &os, const SkeletonPose &pose) {
	for(pair<string,Mat> psm:pose){
		cout << psm.first << endl;
		cout << psm.second << endl;
	}
	return os;
}
class Joint{
	// idx is the VAO index
	int idx;
	string name;
	vector<Joint> children;
	// ****************** INITIAL/BIND TRANSFORMS
	// local initial/bind transfIormation with respect to parent
	// Mparent,local
	Mat localBindTransform;
	// global initial/bind transformation with respect to model origin transform
	// Mglobal,local
	Mat bindTransform;
	// inverse of global initial/bind transformation with respect to model origin transform
	// Mlocal,global
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
		//if this pose is not articulated then
		if(getIdx()==-1){
			//a localBindTransform must have been set in some way
			bindTransform=parentBindTransform*localBindTransform;
			inverseBindTransform=bindTransform.inv();
			animatedTransform=Mat::eye(4,4,CV_32F);
			for(Joint &j:getChildren())
				j.loadInverseBindTransforms(ibts,getBindTransform());
		}
		else{
			Mat &ibt=ibts[getIdx()];
			setInverseBindTransform(ibt);
			//Mat lbt=ibt*parentBindTransform;
			Mat lbt=parentBindTransform.inv()*getBindTransform();
			setLocalBindTransform(lbt);
			animatedTransform=Mat::eye(4,4,CV_32F);
			for(Joint &j:getChildren())
				j.loadInverseBindTransforms(ibts,getBindTransform());
		}
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
		jm[joint.getIdx()]=joint.getAnimatedTransform()*joint.getBindTransform();
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
	inline void applyLocalBinPose(){SkeletonPose pose=getLocalBindPose();applyPose(pose);}
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
	//TO CHECK: 18/10/2021
	Joint& getJointByName(string name){
		Joint& self=*this;
		if(getName()==name)
			return self;
		else{
			for(Joint& j:getChildren()) try{
				return j.getJointByName(name);
			}
			catch(runtime_error &e){}
		}
		throw runtime_error("Joint name="+name+" not found in joint="+getName());
	}
	inline vector<Joint> &getChildren()  {return children;}
	inline int getIdx()               {return idx;}
	inline string &getName()             {return name;}
	inline Mat &getAnimatedTransform()   {return animatedTransform;}
	inline Mat &getInverseBindTransform(){return inverseBindTransform;}
	inline Mat &getBindTransform()       {return bindTransform;}
	inline Mat &getLocalBindTransform()  {return localBindTransform;}
	inline void setAnimatedTransform(Mat &m){animatedTransform=m;}
	inline void setInverseBindTransform(Mat &m){inverseBindTransform=m;bindTransform=m.inv();}
	inline void setBindTransform(Mat &m){bindTransform=m;inverseBindTransform=m.inv();}
	inline void setLocalBindTransform(Mat &m){localBindTransform=m;}
	friend ostream &operator<<(ostream &os, const Joint &v);
	friend void printJoint(ostream &os, const Joint &j,int indent);
};
void printJoint(ostream &os, const Joint &j,int indent){
	for(int i=0;i<indent;i++) os<<" ";
	os<<indent<<".-Name="<<j.name<<"("<<j.idx<<")"<<endl;
	for(const Joint &jc:j.children)
		printJoint(os,jc,indent+2);
}
ostream &operator<<(ostream &os, const Joint &j){
	printJoint(os,j,0);
	return os;
}
