/*
 * cilindro.h
 *
 *  Created on: 25 Oct 2016
 *      Author: francisco
 */

#ifndef CYLINDER_H_
#define CYLINDER_H_
#include <GL/glut.h>

#include "solid.h"
#include "vector3d.h"
class Cone: public Solid {
	float b;
	float h;
	GLUquadricObj *quadratic;
public:
	Cone(float bp=1,float hp=1){
		 quadratic=gluNewQuadric();
		 b=bp;
		 h=hp;
	}
	Cone(const Cone &c):Solid(c),b(c.b),h(c.h){
		quadratic=gluNewQuadric();
	}
	virtual ~Cone(){
		if(quadratic!=nullptr){
			delete quadratic;
			quadratic=nullptr;
		}
	}
	Cone *clone(){
		return new Cone(*this);
	}
	void render(){
		glPushMatrix();
		glColor3f(this->getCol().getX(),this->getCol().getY(),this->getCol().getZ());
		glTranslatef(this->getPos().getX(),this->getPos().getY(),this->getPos().getZ());
		glRotatef(90,1,0,0);
		gluCylinder(quadratic,b,0,h,16,16);
		glPopMatrix();
	}
	 friend std::ostream &operator << (std::ostream &os, const Cone &v);
};
inline std::ostream &operator<<(std::ostream &os, const Cone &c){
	os << static_cast<const Solid&>( c );
	os <<",B="<< c.b <<",H="<<c.h;
    return os;
}
#endif /* CYLINDER_H_ */
