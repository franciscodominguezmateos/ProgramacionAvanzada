#ifndef CUBO_H_
#define CUBO_H_
#include <GL/glut.h>

#include "solid.h"
#include "vector3d.h"
class Cubo: public Solid{
 float s;
public:
 Cubo():Solid(),s(1){}
 Cubo(const Cubo &c):Solid(c),s(c.s){}
 Cubo *clone(){
	 return new Cubo(*this);
 }
 inline float getS(){return s;}
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
 friend std::ostream &operator << (std::ostream &os, const Cubo &v);
};
inline std::ostream &operator<<(std::ostream &os, const Cubo &c){
	os << static_cast<const Solid&>( c );
	os <<",S="<< c.s;
    return os;
}
#endif
