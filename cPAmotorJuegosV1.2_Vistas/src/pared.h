/*
 * Pared.h
 *
 *  Created on: Oct 12, 2017
 *      Author: francisco
 */

#ifndef PARED_H_
#define PARED_H_
#include "solido.h"

class Pared: public Solido {
	Vector3D dim;
	bool vertical;
public:
	//x=longitud, y=altura, z=profundidad
	Pared(double x=4,double y=3,double z=0.1):dim(Vector3D(x,y,z)),vertical(false){}
	Pared(const Pared &p):Solido(p),dim(p.dim),vertical(p.vertical){}
	virtual ~Pared();
	void setVertical(bool b){vertical=b;}
	bool getVertical(){return vertical;}
	Pared *clone(){
		return new Pared(*this);
	}
	void render(){
		glPushMatrix();
        glTranslatef(this->getPos().getX(),this->getPos().getY(),this->getPos().getZ());
        glTranslatef(0,dim.getY()/2.0,0);
        glRotatef(vertical?90:0,0,1,0);
 		glColor3f(this->getCol().getX(),this->getCol().getY(),this->getCol().getZ());
		glScalef(dim.getX(),dim.getY(),dim.getZ());
		glutSolidCube(1);
		glPopMatrix();
	}
};

#endif /* PARED_H_ */
