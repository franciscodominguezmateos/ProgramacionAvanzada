/*
 * texture_cube.h
 *
 *  Created on: 26 Nov 2020
 *      Author: Francisco Dominguez
 *  Just extracted from texture.h
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

