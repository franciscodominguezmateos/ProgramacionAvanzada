/*
 * animator.h
 *
 *  Created on: 27 Freb 2020
 *      Author: Francisco Dominguez
 */
#pragma once
#include <map>
#include <string>
#include <opencv2/opencv.hpp>
#include "model_joint.h"
#include "solid_articulated.h"
#includd "animation.h"

// ABSTRACT CLASS
class Animator;
using AnimatorPtr=Animator*;
class Animator{
	AnimationPtr animation;
	float currentTime;
	bool isLoop;
public:
	Animator(AnimationPtr animation,bool l=true):animation(animation),currentTime(0),isLoop(l){}
	Animator(const Animator &aa):animation(aa.animation->clone()),currentTime(0),isLoop(aa.isLoop){}
	//Animator* clone(){return new Animator(*this);}
	AnimationPtr getAnimationPtr(){return animation;}
	void setAnimation(AnimationPtr a,bool l=true){
		currentTime=0;
		a->resetCurrentFrame();
		animation=a;
		isLoop=l;
	}
	virtual void update(float dt)=0;
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
class AnimatorArticulated{
	SolidArticulated* sa;
	Animation* animation;
	float currentTime;
	bool isLoop;
public:
	AnimatorArticulated(SolidArticulated* sa,Animation* animation,bool l=true):sa(sa),animation(animation),currentTime(0),isLoop(l){}
	AnimatorArticulated(const AnimatorArticulated &aa):sa(aa.sa),animation(aa.animation->clone()),currentTime(0),isLoop(aa.isLoop){}
	AnimatorArticulated* clone(){return new AnimatorArticulated(*this);}
	void setSolidArticulated(SolidArticulated* s){sa=s;}
	void setAnimation(Animation* a,bool l=true){
		currentTime=0;
		a->resetCurrentFrame();
		animation=a;
		isLoop=l;
	}
	void update(float dt){
		incrementTime(dt);
		SkeletonPose currentPose=animation->getCurrentPose(currentTime);
		//Mat I=Mat::eye(4,4,CV_32F);
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
