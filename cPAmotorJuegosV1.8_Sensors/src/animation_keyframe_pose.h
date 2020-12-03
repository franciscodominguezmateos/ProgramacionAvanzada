/*
 * animation_skeleton.h
 *
 *  Created on: 3 Apr 2020
 *      Author: Francisco Dominguez
 */
#pragma once
#include "animation.h"
class KeyFramePose{
	float timeStamp;
	SkeletonPose localJointTransforms;
public:
};
class AnimationKeyFramePoses:public Animation{
	vector<KeyFramePose> keyFrames;
public:
};

