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
	TexturePtr ptex;
    ShaderBinOp* remap;
public:
	Undistor(){
		Mat K=(Mat_<double>(3,3)<<958.8710177402639, 0, 310.8323417297909, 0, 954.3393084573635, 282.4460052293588, 0, 0, 1);
		Mat d=(Mat_<double>(1,5)<<-0.5177046435417091, 0.4190207743279942, -0.01204088578226774, -0.002396026919730114, 0);
		Mat R=(Mat_<double>(3,3)<<0.9968596736669864, 0.008162426114800682, -0.07876652725917205, -0.007394536192554946, 0.9999222997971208, 0.01003569643492776, 0.07884232271468458, -0.009421739136563212, 0.9968425748234252);
		Mat P=(Mat_<double>(3,4)<<953.0537522907438, 0, 384.4876556396484, -57.95554829812738, 0, 953.0537522907438, 276.9204998016357, 0, 0, 0, 1, 0);
		Mat map1,map2;
		initUndistortRectifyMap(K,d,R,P,Size(640,480),CV_32FC2,map1,map2);
		ptex=new Texture(map1);
	}
	void init(){
		ptex->init();
	    remap=new ShaderBinOp(nullptr,640,480,"vec4 opt(vec4 v0,vec4 v1){return remap(v0,v1);}");
		remap->setTex1(ptex);
	}
	Mat process(Mat& img){
	    remap->setImage(img);
		remap->render();
		Mat imgRect=remap->downloadResult();
		return imgRect;
	}
};



