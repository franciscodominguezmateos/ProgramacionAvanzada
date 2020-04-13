/*
 * textura.h
 *
 *  Created on: Oct 15, 2017
 *      Author: Francisco Dominguez
 * 11/04/2020 this Textures can be: images, render buffer or deep buffer in a frame buffer object
 */

#ifndef TEXTURA_H_
#define TEXTURA_H_
#include <string>
#include <GL/glut.h>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

//this prototype is to avoid #include loops
extern int glFramebufferTexture();

class Texture {
	GLuint idTexture;
	Mat img;
public:
	Texture():idTexture(0){}
	virtual ~Texture(){}
	bool isReady(){
		return idTexture!=0;// && !img.empty();
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
		update();
	}
	void setImageFlipped(Mat i){
		img=i;
		update();
	}
	void activar(){
		if(!isReady())
			return;
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, idTexture);
	}
	void desactivar(){
		if(!isReady())
			return;
		glDisable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	void update(){
		glBindTexture(GL_TEXTURE_2D, idTexture);
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,img.cols,img.rows,0,GL_BGR,GL_UNSIGNED_BYTE,img.ptr());
	}
	void asRenderTexture(int w,int h){
		glBindTexture(GL_TEXTURE_2D, idTexture);
		glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, w, h, 0,GL_RGB, GL_UNSIGNED_BYTE, 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, idTexture, 0);
	}
	void asDepthTexture(int w,int h){
		glBindTexture(GL_TEXTURE_2D, idTexture);
		glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT24, w, h, 0,GL_DEPTH_COMPONENT, GL_FLOAT, 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, idTexture, 0);
	}
};

#endif /* TEXTURA_H_ */
