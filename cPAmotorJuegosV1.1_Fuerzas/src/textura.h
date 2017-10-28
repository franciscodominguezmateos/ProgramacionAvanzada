/*
 * textura.h
 *
 *  Created on: Oct 15, 2017
 *      Author: francisco
 */

#ifndef TEXTURA_H_
#define TEXTURA_H_
#include <string>
#include <GL/glut.h>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

class Textura {
	GLuint idTextura;
	Mat img;
public:
	Textura():idTextura(0){}
	virtual ~Textura();
	bool isReady(){
		return idTextura!=0 && !img.empty();
	}
	void init(){
		glGenTextures(1,&idTextura);
		glBindTexture(GL_TEXTURE_2D, idTextura);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	Mat getImage(){return img;}
	void setImage(Mat i){
		flip(i,img,0);
		//reflexion
		flip(img,img,1);
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
		glBindTexture(GL_TEXTURE_2D, idTextura);
	}
	void desactivar(){
		if(!isReady())
			return;
		glDisable(GL_TEXTURE_2D);
	}
	void update(){
		glBindTexture(GL_TEXTURE_2D, idTextura);
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,img.cols,img.rows,0,GL_BGR,GL_UNSIGNED_BYTE,img.ptr());
	}
};

#endif /* TEXTURA_H_ */
