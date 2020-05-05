/*
 * solid_articulated_animated.h
 *
 *  Created on: 4 May 2020
 *      Author: Francisco Dominguez
 */
#pragma once
#include "animation_skeleton.h"
#include "solid_articulated_vao.h"

class SolidArticulatedAnimatedVAO:public SolidArticulatedVAO {
	Animation* as;
	AnimatorArticulated* aa;
public:
	SolidArticulatedAnimatedVAO():as(nullptr),aa(nullptr){}
	SolidArticulatedAnimatedVAO(GLSLShaderProgram* p=nullptr,GLSLVAO *pvao=nullptr):
		SolidArticulatedVAO(p,pvao){}
	void init(ModelMeshArticulated &ma,AnimationSkeleton* pas,float scale=0.01){
	    ma.doUniformScale(scale);
	    SolidArticulatedVAO::init(ma);
	    pas->doUniformScale(scale);
	    as=pas;
	    aa=new AnimatorArticulated(this,as);
	}
	void update(double dt){
		SolidArticulatedVAO::update(dt);
		aa->update(dt);
	}
};
