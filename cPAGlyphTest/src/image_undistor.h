/*
 * image_undistor.h
 *
 *  Created on: 13 Mar 2022
 *      Author: Francisco Dominguez
 *  Efficient undistor class usin GLSL Shader
 */
#pragma once
#include "shader_gpgpu_binop.h"

class Undistor{
public:
	Mat K,d,R,P;
	TexturePtr ptex;
    ShaderBinOp* remap;
    Mat frame;
public:
	Undistor(){
		Mat K=(Mat_<double>(3,3)<<951.4741259832514, 0, 293.3416170604175, 0, 950.3903009136468, 272.6355355349708, 0, 0, 1);
		Mat d=(Mat_<double>(1,5)<<-0.4953024672257269, 0.3330644888148587, -0.004006958790423586, 0.008548218245607087, 0);
		Mat R=(Mat_<double>(3,3)<<0.9960354394631838, 0.007417679552695122, -0.0886475118851934, -0.008281288486258502, 0.9999217313542504, -0.009378242187402474, 0.08857100876917536, 0.01007517719803191, 0.996018909062493);
		Mat P=(Mat_<double>(3,4)<<953.0537522907438, 0, 384.4876556396484, 0, 0, 953.0537522907438, 276.9204998016357, 0, 0, 0, 1, 0);
		Mat map1,map2;
		initUndistortRectifyMap(K,d,R,P,Size(640,480),CV_32FC2,map1,map2);
		ptex=new Texture(map1);
	}
	void init(){
		ptex->init();
	    remap=new ShaderBinOp(nullptr,640,480,"vec4 opt(vec4 v0,vec4 v1){return remap(v0,v1);}");
		remap->setTex1(ptex);
	}
	Mat process(Mat& image){
	    image.convertTo(frame,CV_32FC3,1.0/255.0);
	    remap->setImage(frame);
		remap->render();
		Mat imgRect=remap->downloadResult();
		Mat ret;
		cvtColor(imgRect, ret, CV_BGRA2BGR);
		//imgRect.convertTo(ret,CV_32FC3);
		return ret;
	}
};



