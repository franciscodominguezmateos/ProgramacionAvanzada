/*
 * animation_skeleton.h
 *
 *  Created on: 2 Apr 2020
 *      Author: Francisco Dominguez
 */
#pragma once
#include "solid_articulated.h"
#include "animation.h"
class KeyFrameJoint{
	float timeStamp;
	Mat localJointTransform;
public:
	KeyFrameJoint(float t,Mat &m):timeStamp(t),localJointTransform(m.clone()){}
	inline float getTimeStamp(){return timeStamp;}
	inline Mat &getT(){return localJointTransform;}
};
class AnimationJoint{
	string jointName;
	int currentFrame;
	vector<KeyFrameJoint> keyFrames;
public:
	AnimationJoint(){}//needed for default containers constructor
	AnimationJoint(string name):jointName(name),currentFrame(0){}
	inline void addKeyFrameJoint(KeyFrameJoint kfj){keyFrames.push_back(kfj);}
	inline KeyFrameJoint &getKeyFrameJoint(int idx){return keyFrames[idx];}
	Mat getTransformation(KeyFrameJoint c,KeyFrameJoint n,float t){
		float di=n.getTimeStamp()-c.getTimeStamp();
		float dt=t-c.getTimeStamp();
		float tr=dt/di;
		return interpolate(c.getT().clone(),n.getT().clone(),tr);
	}
	Mat getTransformation(float t){
		//TODO: fine tune this method, it is to rough sure it fail
		int nextFrame=currentFrame+1;
		cout << "currentFrame=" << currentFrame<<","<<nextFrame<<endl;
		if(keyFrames[nextFrame].getTimeStamp()<t){
			if(nextFrame==(int)keyFrames.size()-1){
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
class AnimationSkeleton:public Animation{
	vector<string> jointNames;
	map<string,AnimationJoint> animationJoints;
public:
	void addKeyFramesJoint(AnimationJoint kfj){animationJoints[kfj.getName()]=kfj;}
	void setJointNames(vector<string> vs){jointNames=vs;}
	SkeletonPose getCurrentPose(float t){
		SkeletonPose mm;
		for(string &n:jointNames){
			AnimationJoint &aJointNth=animationJoints[n];
			mm[n]=aJointNth.getTransformation(t).clone();
		}
		return mm;
	}
	//Assuming all AnimationJoint have the same number of keyframes
	SkeletonPose getPose(int idx){
		SkeletonPose mm;
		for(string &n:jointNames){
			AnimationJoint &aJointNth=animationJoints[n];
			mm[n]=aJointNth.getKeyFrameJoint(idx).getT().clone();
		}
		return mm;
	}
};


