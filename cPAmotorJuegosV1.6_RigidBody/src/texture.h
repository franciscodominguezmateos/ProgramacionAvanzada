/*
 * textura.h
 *
 *  Created on: Oct 15, 2017
 *      Author: Francisco Dominguez
 * 11/04/2020 this Textures can be: images, render buffer or deep buffer in a frame buffer object
 * 28/04/2020 add TextureCube
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

class TextureCube{
	GLuint idTexture;
	//image order matter
	//0= GL_TEXTURE_CUBE_MAP_POSITIVE_X = Right  Face
	//1= GL_TEXTURE_CUBE_MAP_NEGATIVE_X = Left   Face
	//2= GL_TEXTURE_CUBE_MAP_POSITIVE_Y = Top    Face
	//3= GL_TEXTURE_CUBE_MAP_NEGATIVE_Y = Bottom Face
	//4= GL_TEXTURE_CUBE_MAP_POSITIVE_Z = Back   Face
	//5= GL_TEXTURE_CUBE_MAP_NEGATIVE_Z = Front  Face
	vector<Mat> images;
public:
	TextureCube():idTexture(0){}
	void init(){
		if(images.size()==6){
			glGenTextures(1,&idTexture);
			glBindTexture(GL_TEXTURE_CUBE_MAP, idTexture);
			for(unsigned int i=0;i<images.size();i++){
				Mat &img=images[i];
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,0,GL_RGB,img.cols,img.rows,0,GL_BGR,GL_UNSIGNED_BYTE,img.ptr());
			}
			glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
			/*glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
			*/
		    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		}
		else
			runtime_error(" from TextureCube::init() 6 images needed before calling  init");
	}
	void addImage(Mat img){images.push_back(img);}
	void bind(){
		glEnable(GL_TEXTURE_CUBE_MAP);
		glBindTexture(GL_TEXTURE_CUBE_MAP, idTexture);
	}
	void unbind(){
		glDisable(GL_TEXTURE_CUBE_MAP);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}
};
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
		if (img.channels()==3)
			glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,img.cols,img.rows,0,GL_BGR,GL_UNSIGNED_BYTE,img.ptr());
		if (img.channels()==4)
			glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,img.cols,img.rows,0,GL_BGRA,GL_UNSIGNED_BYTE,img.ptr());
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

