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
    uint width,height;
	TexturePtr ptex;
    ShaderBinOp* remap;
    Mat frame;
public:
	Undistor(Mat& K,Mat& d,Mat& R,Mat&P,uint width=640,uint height=480):K(K),d(d),R(R),P(P),width(width),height(height){
		Mat map1,map2;
		initUndistortRectifyMap(K,d,R,P,Size(width,height),CV_32FC2,map1,map2);
		ptex=new Texture(map1);
	}
	void init(){
		ptex->init();
	    remap=new ShaderBinOp(nullptr,width,height,"vec4 opt(vec4 v0,vec4 v1){return remap(v0,v1);}");
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
	const Mat& getD() const {return d;}
	void setD(const Mat &d) {this->d = d;}
	const Mat& getK() const {return K;}
	void setK(const Mat &k) {K = k;	}
	const Mat& getP() const {return P;}
	void setP(const Mat &p) {P = p;	}
	const Mat& getR() const {return R;}
	void setR(const Mat &r) {R = r;	}
	uint getWidth() const {	return width;}
	void setWidth(uint width) {	this->width = width;}
	uint getHeight() const {return height;	}
	void setHeight(uint height) {this->height = height;	}
};



