#pragma once
#include "solid.h"
class Cube: public Solid{
 double s;
public:
 Cube(double d):Solid(),s(d){}
 Cube():Cube(1){}
 Cube(const Cube &c):Solid(c),s(c.s){}
 Cube *clone(){
	 return new Cube(*this);
 }
 inline double getS(){return s;}
 inline void setS(float sp){s=sp;}
 void render(){
	 glPushMatrix();
	  glColor3f(this->getCol().getX(),this->getCol().getY(),this->getCol().getZ());
		glTranslatef(getPos().getX(),getPos().getY(),getPos().getZ());
		glRotatef(getRot().getX(), 1, 0, 0);
		glRotatef(getRot().getY(), 0, 1, 0);
		glRotatef(getRot().getZ(), 0, 0, 1);
		glutSolidCube(s);
	 glPopMatrix();

 }
 friend std::ostream &operator << (std::ostream &os, const Cube &v);
};
inline std::ostream &operator<<(std::ostream &os, const Cube &c){
	os << static_cast<const Solid&>( c );
	os <<",S="<< c.s;
    return os;
}
