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

class Texture;
using TexturePtr=Texture*;

class Texture {
	GLuint idTexture;
	int width,height;
	Mat img;
public:
	Texture(int w=0,int h=0):idTexture(0),width(w),height(h){}
	Texture(Mat img):idTexture(0),width(img.cols),height(img.rows),img(img){}
	Texture(string fileName):Texture(imread(fileName)){}
	virtual ~Texture(){}//glDeleteTextures(1,&idTexture);}
	void init(){
		glGenTextures(1,&idTexture);
		glBindTexture(GL_TEXTURE_2D, idTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		if(!img.empty()) setImage(img);
	}
	GLuint getId(){return idTexture;}
	Mat getImage(){return img;}
	inline void setSize(int w,int h){width=w;height=h;}
	inline int getWidth() {return width;}
	inline int getHeight(){return height;}
	inline GLuint id(){return idTexture;}
	void setImage(Mat i){
		setSize(i.cols,i.rows);
		flip(i,img,0);
		//reflexion
		//flip(img,img,1);
		upload();
	}
	void setImageFlipped(Mat i){
		setSize(i.cols,i.rows);
		img=i;
		upload();
	}
	bool isReady(){
		return idTexture!=0;//&& !img.empty();
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
		if (img.channels()==1){
			if(img.type()==CV_8UC1)
				glTexImage2D(GL_TEXTURE_2D,0,GL_R,img.cols,img.rows,0,GL_R,GL_UNSIGNED_BYTE,img.ptr());
			if(img.type()==CV_32FC1)
				glTexImage2D(GL_TEXTURE_2D,0,GL_R32F,img.cols,img.rows,0,GL_R,GL_FLOAT,img.ptr());
		}
		if (img.channels()==2){
			if(img.type()==CV_8UC2)
				glTexImage2D(GL_TEXTURE_2D,0,GL_RG,img.cols,img.rows,0,GL_RG,GL_UNSIGNED_BYTE,img.ptr());
			if(img.type()==CV_32FC2)
				glTexImage2D(GL_TEXTURE_2D,0,GL_RG32F,img.cols,img.rows,0,GL_RG,GL_FLOAT,img.ptr());
		}
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
		bind();
		glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, w, h, 0,GL_RGB, GL_UNSIGNED_BYTE, 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, idTexture, 0);
		unbind();
	}
	void asRenderTexture32FC4(int w,int h){
		bind();
		glTexImage2D (GL_TEXTURE_2D, 0,GL_RGBA32F, w, h, 0,GL_RGBA, GL_FLOAT, 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, idTexture, 0);
		unbind();
	}
	void asRenderTexture(int w,int h){asRenderTexture32FC4(w,h);}
	void asDepthTexture(int w,int h){
		bind();
		//glBindTexture(GL_TEXTURE_2D, idTexture);
		glTexImage2D (GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT24, w, h, 0,GL_DEPTH_COMPONENT, GL_FLOAT, 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, idTexture, 0);
		unbind();
	}
};

