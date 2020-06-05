/*
 * animation.h
 *
 *  Created on: 3 Apr 2020
 *      Author: Francisco Dominguez
 */
#pragma once
#include <map>
#include <string>
#include <opencv2/opencv.hpp>
#include "model_joint.h"
#include "solid_articulated.h"

class Animation{
	float duration;
public:
	double getDuration() const {return duration;}
	void setDuration(float duration) {this->duration = duration;}
	virtual void resetCurrentFrame()=0;
	virtual SkeletonPose getCurrentPose(float currentTime)=0;
};
class AnimatorArticulated{
	SolidArticulated* sa;
	Animation* animation;
	float currentTime;
	bool isLoop;
public:
	AnimatorArticulated(SolidArticulated* sa,Animation* animation,bool l=true):sa(sa),animation(animation),currentTime(0),isLoop(l){}
	void setAnimation(Animation* a,bool l=true){
		currentTime=0;
		a->resetCurrentFrame();
		animation=a;
		isLoop=l;
	}
	void update(float dt){
		incrementTime(dt);
		SkeletonPose currentPose=animation->getCurrentPose(currentTime);
		Mat I=Mat::eye(4,4,CV_32F);
	    //Mat up32f=posEulerAnglesToTransformationMatrix<float>(Vector3D(),Vector3D(-90,0,0));
		sa->setPose(currentPose);
	}
	void incrementTime(float dt){
		//TODO: if dt=0.1 only it doesn't work
		dt*=0.1;
		currentTime+=dt;
		float animDuration=animation->getDuration();
		if(currentTime>animDuration){
			if(isLoop){
				// aproximation to remaind/mod/% with two floats
				//while(currentTime>animDuration) currentTime-=animDuration;
				currentTime=0;
				animation->resetCurrentFrame();
			}
			else currentTime=animDuration;
		}
	}
};
