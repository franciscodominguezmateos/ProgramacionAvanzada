/*
 * animation.h
 *
 *  Created on: 3 Apr 2020
 *      Author: Francisco Dominguez
 */
#pragma once

// SkeletonPose is all local transformations for a hole skeleton, in this case map<string,Mat>
// since they are indexed by name
typedef map<string,Mat> SkeletonPose;

class Animation{
	float duration;
public:
	float getDuration() const {return duration;}
	void setDuration(float duration) {this->duration = duration;}
	virtual SkeletonPose getCurrentPose(float currentTime)=0;

};

class AnimatorArticulated{
	SolidArticulated* sa;
	Animation* animation;
	float currentTime;
public:
	AnimatorArticulated(SolidArticulated* sa,Animation* animation):sa(sa),animation(animation),currentTime(0){}
	void setAnimation(Animation* a){currentTime=0;animation=a;}
	void update(float dt){
		incrementTime(dt);
		SkeletonPose currentPose=animation->getCurrentPose(currentTime);
		Mat I=Mat::eye(4,4,CV_32F);
	    Mat up32f=Mat::zeros(4,4,CV_32F);
	    up32f.at<float>(0,0)= 1.0;
	    up32f.at<float>(3,3)= 1.0;
	    up32f.at<float>(1,2)= 1.0;
	    up32f.at<float>(2,1)=-1.0;
		applyPose2Joints(currentPose,sa->getJointsRoot(),up32f);
	}
	void incrementTime(float dt){
		currentTime+=dt;
		float animDuration=animation->getDuration();
		if(currentTime>animDuration)
			// aproximation to remaind/mod/% with two floats
			while(currentTime>animDuration) currentTime-=animDuration;
	}
	void applyPose2Joints(SkeletonPose &vm,Joint &joint,Mat &currentParentTransform){
		Mat &currentLocalTransform=vm[joint.getName()];
		Mat currentTransform=currentParentTransform*currentLocalTransform;
		for(Joint &j:joint.getChildren())
			applyPose2Joints(vm,j,currentTransform);
		Mat animationTransform=currentTransform*joint.getInverseBindTransform();
		joint.setAnimatedTransform(animationTransform);
	}
};
