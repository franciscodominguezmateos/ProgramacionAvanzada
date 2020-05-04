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
	/*KeyFrameJoint(const KeyFrameJoint &c):
		timeStamp(c.timeStamp)//,
		//localJointTransform(c.localJointTransform.clone())
	{c.localJointTransform.copyTo(localJointTransform);}*/
	inline double getTimeStamp(){return timeStamp;}
	inline Mat getT(){return localJointTransform.clone();}
	inline void doUniformScale(float s){localJointTransform=uniformScaleTransform(localJointTransform,s);}
};
class AnimationJoint{
	string jointName;
	int currentFrame;
	vector<KeyFrameJoint> keyFrames;
	double duration;
public:
	AnimationJoint():duration(0){}//needed for default containers constructor
	AnimationJoint(string name):jointName(name),currentFrame(0),duration(0){}
	inline void addKeyFrameJoint(KeyFrameJoint kfj){keyFrames.push_back(kfj);duration=max(duration,kfj.getTimeStamp());}
	inline KeyFrameJoint &getKeyFrameJoint(int idx){return keyFrames[idx];}
	Mat getTransformation(KeyFrameJoint c,KeyFrameJoint n,float t){
		float di=n.getTimeStamp()-c.getTimeStamp();
		float dt=t-c.getTimeStamp();
		float tr=dt/di;
		return interpolate(c.getT(),n.getT(),tr);
	}
	void resetCurrentFrame(){currentFrame=0;}
	Mat getTransformation(float t){
		int nextFrame=currentFrame+1;
		if(t>keyFrames[nextFrame].getTimeStamp()){
			if(nextFrame!=(int)keyFrames.size()-1){
				currentFrame=nextFrame;
				nextFrame+=1;
			}
		}
		Mat r=getTransformation(keyFrames[currentFrame],keyFrames[nextFrame],t);
		return r;
	}
	inline void doUniformScale(float s){
		for(KeyFrameJoint &kfj:keyFrames) kfj.doUniformScale(s);
	}
	inline string &getName(){return jointName;}
	inline double getDuration(){return duration;}
};
class AnimationSkeleton:public Animation{
	vector<string> jointNames;
	map<string,AnimationJoint> animationJoints;
public:
	inline void doUniformScale(float s){
		for(string &n:jointNames){
			//some articulation could not be animated
			if(animationJoints.count(n)>0)
				animationJoints[n].doUniformScale(s);
		}
	}
	void resetCurrentFrame(){
		for(string &n:jointNames){
			//some articulation could not be animated
			if(animationJoints.count(n)>0)
				animationJoints[n].resetCurrentFrame();
		}
	}
	void addKeyFramesJoint(AnimationJoint kfj){animationJoints[kfj.getName()]=kfj;setDuration(max(getDuration(),kfj.getDuration()));}
	void setJointNames(vector<string> vs){jointNames=vs;}
	SkeletonPose getCurrentPose(float t){
		SkeletonPose mm;
		for(string &n:jointNames){
			//some articulation could not be animated
			if(animationJoints.count(n)>0){
				AnimationJoint &aJointNth=animationJoints[n];
				mm[n]=aJointNth.getTransformation(t);
			}
		}
		return mm;
	}
	//Assuming all AnimationJoint have the same number of keyframes
	//and all start at same time.
	SkeletonPose getPose(int idx){
		SkeletonPose mm;
/*		for(string &n:jointNames){
			//some articulation could not be animated
			if(animationJoints.count(n)>0){
				AnimationJoint &aJointNth=animationJoints[n];
				mm[n]=aJointNth.getKeyFrameJoint(idx).getT();
			}
		}
*/
		int i=0;
		for(pair<string,AnimationJoint> p:animationJoints){
			//some articulation could not be animated
			string &n=p.first;
			AnimationJoint &aJointNth=p.second;
			mm[n]=aJointNth.getKeyFrameJoint(idx).getT();
			cout << i++ << "="<<n << "=="<<aJointNth.getName()<<endl;
		}
		cout <<"jointNames="<< jointNames.size()<<endl;
		for(unsigned int i=0;i<jointNames.size();i++){
			cout << i<<"="<<jointNames[i]<<endl;
		}
		return mm;
	}
};


