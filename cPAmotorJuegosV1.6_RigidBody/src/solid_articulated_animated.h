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
	AnimatorArticulated* aa;
public:
	SolidArticulatedAnimatedVAO():aa(nullptr){}
	SolidArticulatedAnimatedVAO(GLSLShaderProgram* p=nullptr,GLSLVAO *pvao=nullptr):
		SolidArticulatedVAO(p,pvao),aa(nullptr){}
    SolidArticulatedAnimatedVAO(const SolidArticulatedAnimatedVAO &s):SolidArticulatedVAO(s),aa(s.aa->clone()){aa->setSolidArticulated(this);}
    SolidArticulatedAnimatedVAO* clone(){return new SolidArticulatedAnimatedVAO(*this);}
	void init(ModelMeshArticulated &ma,AnimationSkeleton* pas,float scale=0.01){
	    ma.doUniformScale(scale);
	    SolidArticulatedVAO::init(ma);
	    pas->doUniformScale(scale);
	    aa=new AnimatorArticulated(this,pas);
	}
	void setAnimation(AnimationSkeleton* pas,bool b=true){
		aa->setAnimation(pas,b);
	}
	void update(double dt){
		SolidArticulatedVAO::update(dt);
		aa->update(dt);
	}
};
