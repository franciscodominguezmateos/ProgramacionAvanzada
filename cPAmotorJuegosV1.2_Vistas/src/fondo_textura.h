/*
 * fondo_textura.h
 *
 *  Created on: Oct 28, 2017
 *      Author: francisco
 */

#ifndef FONDO_TEXTURA_H_
#define FONDO_TEXTURA_H_
#include "rectangulo.h"
#include "fondo.h"

class FondoTextura: public Fondo {
	Rectangulo fondo;
public:
	FondoTextura():FondoTextura(640,480){}
	FondoTextura(unsigned int ancho,unsigned int alto):fondo(
		Vector3D(  0,  0,0),
		Vector3D(ancho,  0,0),
		Vector3D(ancho,alto,0),
		Vector3D(  0,alto,0)){}
	virtual ~FondoTextura();
	inline void setTextura(Textura t){fondo.setTextura(t);}
	void render(){
		int w=fondo.getTex().getImage().cols;
		int h=fondo.getTex().getImage().rows;
		//Projection matrix mode
	    glMatrixMode(GL_PROJECTION);
	    //save perspective projection
	    glPushMatrix();
	    glLoadIdentity();
	    //set orthogonal projection
	    gluOrtho2D(-w/2, w/2, -h/2, h/2);

	    //Render  B A C K G R O U N D
	    glDisable(GL_LIGHTING);
	    glMatrixMode(GL_MODELVIEW);
	    glPushMatrix();
	     glLoadIdentity();
	     glTranslatef(-w/2,-h/2,0);
	     fondo.render();
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

#endif /* FONDO_TEXTURA_H_ */
