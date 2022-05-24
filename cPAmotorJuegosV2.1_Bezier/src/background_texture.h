/*
 * fondo_textura.h
 *
 *  Created on: Oct 28, 2017
 *      Author: francisco
 */
#pragma once
#include "background.h"
#include "rectangle.h"

class BackgroundTexture;
using BackgroundTexturePtr=BackgroundTexture*;
class BackgroundTexture: public Background {
	Rectangle recBackground;
public:
	BackgroundTexture():BackgroundTexture(640,480){}
	BackgroundTexture(unsigned int ancho,unsigned int alto):recBackground(
		Vector3D(  0,  0,0),
		Vector3D(1,  0,0),
		Vector3D(1,1,0),
		Vector3D(  0,1,0)){}
	virtual ~BackgroundTexture(){}
	inline void setTextura(Texture t){recBackground.setTextura(t);}
	inline void init(){recBackground.getTex().init();}
	inline void setImage(Mat& img){recBackground.getTex().setImage(img);}
	void render(){
		//int w=recBackground.getTex().getImage().cols;
		//int h=recBackground.getTex().getImage().rows;
		//Projection matrix mode
	    glMatrixMode(GL_PROJECTION);
	    //save perspective projection
	    glPushMatrix();
	    glLoadIdentity();
	    //set orthogonal projection
	    //gluOrtho2D(-w/2, w/2, -h/2, h/2);
	    gluOrtho2D(-1.0/2.0, 1.0/2.0, -1.0/2.0, 1.0/2.0);

	    //Render  B A C K G R O U N D
	    glDisable(GL_LIGHTING);
	    glMatrixMode(GL_MODELVIEW);
	    glPushMatrix();
	     glLoadIdentity();
	     //glTranslatef(-w/2,-h/2,0);
	     //glTranslatef(-1.0/2.0,-1.0/2.0,0);
	     //fondo.setPos(Vector3D(-w/2,-h/2,0));
	     recBackground.setPos(Vector3D(-1.0/2.0,-1.0/2.0,0));
	     recBackground.render();
	    glPopMatrix();
	    glEnable(GL_LIGHTING);

		//Projection matrix mode
	    glMatrixMode(GL_PROJECTION);
	    //restore perspective projection
	    glPopMatrix();

	    //R E N D ER
	    glMatrixMode(GL_MODELVIEW);
	    glClear(GL_DEPTH_BUFFER_BIT);
	}
};
