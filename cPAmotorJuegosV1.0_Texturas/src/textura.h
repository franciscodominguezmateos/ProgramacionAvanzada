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
	Textura();
	virtual ~Textura();
	Mat getImage(){return img;}
	void setImage(Mat i){
		img=i;//flip(i,img,0);
	}
	void init();
	void activar(){
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, idTextura);
	}
	void desactivar(){glDisable(GL_TEXTURE_2D);}
	void update(){
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,img.cols,img.rows,0,GL_BGR,GL_UNSIGNED_BYTE,img.ptr());
	}
};

#endif /* TEXTURA_H_ */
