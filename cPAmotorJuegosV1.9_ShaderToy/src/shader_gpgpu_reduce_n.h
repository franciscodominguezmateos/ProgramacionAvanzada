/*
 * shader_gpgpu_reduce_n.h
 *
 *  Created on: 17 Jan 2021
 *      Author: Francisco Dominguez
 *  Make a reduce n times reducing 2^n
 *  Both dimensions raws and cols have to be divisible by 2^n
 *
 */

#pragma once

#include "shader_gpgpu_reduce_half.h"

class ShaderReduceN: public ShaderReduceHalf {
	int n;
	vector<GLSLFBOPtr> vFbo;
public:
	ShaderReduceN(int n,Texture* ptex=nullptr,int w=640,int h=480,string oc="vec4 opt(vec4 v00,vec4 v10,vec4 v01,vec4 v11){return (v00+v10+v01+v11)/4.0;}"):
		ShaderReduceHalf(ptex,w,h,oc),n(n){
		if(ptex!=nullptr){w=getTexture()->getWidth();h=getTexture()->getHeight();}
		vFbo.push_back(getFrameBuffer());
		for(int i=1;i<n;i++){
			GLSLFBOPtr pfbo=new GLSLFBO(w>>(i+1),h>>(i+1));
			vFbo.push_back(pfbo);
			pfbo->init();
		}
	}
	~ShaderReduceN(){
		for(int i=1;i<n;i++) delete vFbo[i];
	}
	void render(){
		ShaderReduceHalf::render();
		Mat fboImg=downloadResult();
		for(int i=1;i<n;i++){
			setTexture(&vFbo[i-1]->getColorTex());
			setFrameBuffer(vFbo[i]);
			ShaderReduceHalf::render();
			fboImg=downloadResult();
		}
	}
};

