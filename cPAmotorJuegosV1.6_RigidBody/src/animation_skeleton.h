/*
 * animation_skeleton.h
 *
 *  Created on: 2 Apr 2020
 *      Author: Francisco Dominguez
 */

#pragma once

#include "model_mesh_articulated.h"
class KeyFrameJoint{
	float timeStamp;
	Mat localJointTransform;
public:
	float getTimeStamp(){return timeStamp;}
	Mat &getT(){return localJointTransform;}
};
class KeyFramesJoint{
	string jointName;
	int currentFrame;
	vector<KeyFrameJoint> keyFrames;
public:
	KeyFramesJoint(){}//needed for default containers constructor
	KeyFramesJoint(string name):jointName(name),currentFrame(0){}
	Mat getTransformation(KeyFrameJoint c,KeyFrameJoint n,float t){
		float di=n.getTimeStamp()-c.getTimeStamp();
		float dt=t-c.getTimeStamp();
		float tr=dt/di;
		return interpolate(n.getT(),c.getT(),tr);
	}
	Mat getTransformation(float t){
		//TODO: finetune this ethos it is to rough sure it fail
		int nextFrame=currentFrame+1;
		if(keyFrames[nextFrame].getTimeStamp()<t){
			if(nextFrame==(int)keyFrames.size()){
				currentFrame=0;
				nextFrame=currentFrame+1;
			}
			else{
				currentFrame=nextFrame;
				while(keyFrames[nextFrame].getTimeStamp()<t) nextFrame++;
			}
		}
		Mat r=getTransformation(keyFrames[currentFrame],keyFrames[nextFrame],t);
		return r;
	}
	string &getName(){return jointName;}
};
class KeyFramesSkeleton{
	vector<string> jointNames;
	map<string,KeyFramesJoint> keyFramesSkeleton;
public:
	void addKeyFramesJoint(KeyFramesJoint &kfj){keyFramesSkeleton[kfj.getName()]=kfj;}
	map<string,Mat> snapShotSkeleton(float t){
		map<string,Mat> mm;
		for(string &n:jointNames){
			mm[n]=keyFramesSkeleton[n].getTransformation(t);
		}
		return mm;
	}
};

class KeyFrameJoints{
	float timeStamp;
	vector<Mat> localJointTransforms;
public:
};
class AnimationJoints{
	vector<KeyFrameJoints> keyFrames;
public:

};

class AnimationSkeleton {
	KeyFramesSkeleton kfSkeleton;
	float duration;
};
class AnimatorArticulated{
	//SolidArticulated sa;
	AnimationJoints animation;
public:
void applyPose2Joints(map<string,Mat> &vm,Joint &joint,Mat &currentParentTransform){
	Mat &currentLocalTransform=vm[joint.getName()];
	 Mat currentTransform=currentParentTransform*currentLocalTransform;
	 for(Joint &j:joint.getChildren())
		 applyPose2Joints(vm,j,currentTransform);
	 Mat animationTransform=currentTransform*joint.getInverseBindTransform();
	 joint.setAnimatedTransform(animationTransform);
}
};
