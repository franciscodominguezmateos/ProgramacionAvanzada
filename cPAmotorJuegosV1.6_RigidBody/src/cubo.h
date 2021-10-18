#ifndef CUBO_H_
#define CUBO_H_
#include <GL/glut.h>
#include "vector3d.h"
#include "solido.h"
class Cube: public Solido{
 float s;
public:
 Cube():Solido(),s(1){}
 Cube(const Cube &c):Solido(c),s(c.s){}
 Cube *clone(){
	 return new Cube(*this);
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
 friend std::ostream &operator << (std::ostream &os, const Cube &v);
};
inline std::ostream &operator<<(std::ostream &os, const Cube &c){
	os << static_cast<const Solido&>( c );
	os <<",S="<< c.s;
    return os;
}
#endif
