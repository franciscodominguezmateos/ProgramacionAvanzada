/*
 * rosco.h
 *
 *  Created on: 27 Oct 2016
 *      Author: francisco
 */

#ifndef ROSCO_H_
#define ROSCO_H_
#include <GL/glut.h>
#include "vector3d.h"
#include "solido.h"

class Rosco: public Solido {
	float r1;
	float r2;
public:
	Rosco(float pr1=0.05,float pr2=0.27);
	Rosco(const Rosco &r):Solido(r),r1(r.r1),r2(r.r2){}
	virtual ~Rosco();
	Rosco *clone(){
		return new Rosco(*this);
	}
	void render();
	float getR1() const {return r1;}
	void setR1(float r1) {this->r1 = r1;}
	float getR2() const {return r2;}
	void setR2(float r2) {this->r2 = r2;}
	friend std::ostream &operator << (std::ostream &os, const Rosco &r);
};
	inline std::ostream &operator<<(std::ostream &os, const Rosco &r){
		os << static_cast<const Solido&>( r );
		os <<",R1="<< r.r1 << ",R2="<<r.r2;
	    return os;
	}
#endif /* ROSCO_H_ */
