/*
 * textura.h
 *
 *  Created on: Oct 15, 2017
 *      Author: Francisco Dominguez
 * 11/04/2020 this Textures can be: images, render buffer or deep buffer in a frame buffer object
 * 28/04/2020 add TextureCube
 * 27/12/2020 starting with opencv float images
 *  values in float textures are NOT clamped to [0,1]
 */
#pragma once
#include <string>
#include "opencv2/opencv.hpp"

// In order to use GLSL
#define GL_GLEXT_PROTOTYPES
#include <GL/glut.h>
// In order to use GLSL
#include <GL/glext.h>

using namespace std;
using namespace cv;

class Texture {
	GLuint idTexture;
	Mat img;
public:
	Texture():idTexture(0){}
	virtual ~Texture(){}
	bool isReady(){
		return idTexture!=0 && !img.empty();
	}
	void init(){
		glGenTextures(1,&idTexture);
		glBindTexture(GL_TEXTURE_2D, idTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	Mat getImage(){return img;}
	void setImage(Mat i){
		flip(i,img,0);
		//reflexion
		//flip(img,img,1);
		upload();
	}
	void setImageFlipped(Mat i){
		img=i;
		upload();
	}
	void bind(){
		if(!isReady())
			return;
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, idTexture);
	}
	void unbind(){
		if(!isReady())
			return;
		glDisable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	void upload(){
		glBindTexture(GL_TEXTURE_2D, idTexture);
		if (img.channels()==3){
			if(img.type()==CV_8UC3)
				glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,img.cols,img.rows,0,GL_BGR,GL_UNSIGNED_BYTE,img.ptr());
			if(img.type()==CV_32FC3)
				glTexImage2D(GL_TEXTURE_2D,0,GL_RGB32F,img.cols,img.rows,0,GL_BGR,GL_FLOAT,img.ptr());
		}
		if (img.channels()==4){
			if(img.type()==CV_8UC4)
				glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,img.cols,img.rows,0,GL_BGRA,GL_UNSIGNED_BYTE,img.ptr());
			if(img.type()==CV_32FC4)
				glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA32F,img.cols,img.rows,0,GL_BGRA,GL_FLOAT,img.ptr());
		}

	}
	void asRenderTexture8UC3(int w,int h){
		glBindTexture(GL_TEXTURE_2D, idTexture);
		glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, w, h, 0,GL_RGB, GL_UNSIGNED_BYTE, 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, idTexture, 0);
	}
	void asRenderTexture32FC4(int w,int h){
		glBindTexture(GL_TEXTURE_2D, idTexture);
		glTexImage2D (GL_TEXTURE_2D, 0,GL_RGBA, w, h, 0,GL_RGBA, GL_FLOAT, 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, idTexture, 0);
	}
	void asRenderTexture(int w,int h){asRenderTexture32FC4(w,h);}
	void asDepthTexture(int w,int h){
		glBindTexture(GL_TEXTURE_2D, idTexture);
		glTexImage2D (GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT24, w, h, 0,GL_DEPTH_COMPONENT, GL_FLOAT, 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, idTexture, 0);
	}
};

