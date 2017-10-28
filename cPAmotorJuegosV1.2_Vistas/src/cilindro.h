/*
 * cilindro.h
 *
 *  Created on: 25 Oct 2016
 *      Author: francisco
 */

#ifndef CILINDRO_H_
#define CILINDRO_H_
#include <GL/glut.h>
#include "vector3d.h"
#include "solido.h"
class Cilindro: public Solido {
	float b;
	float h;
	GLUquadricObj *quadratic;
public:
	Cilindro(float bp=1,float hp=1);
	Cilindro(const Cilindro &c):Solido(c),b(c.b),h(c.h){
		quadratic=gluNewQuadric();
	}
	virtual ~Cilindro();
	Cilindro *clone(){
		return new Cilindro(*this);
	}
	void render();
	 friend std::ostream &operator << (std::ostream &os, const Cilindro &v);
};
inline std::ostream &operator<<(std::ostream &os, const Cilindro &c){
	os << static_cast<const Solido&>( c );
	os <<",B="<< c.b <<",H="<<c.h;
    return os;
}
#endif /* CILINDRO_H_ */
