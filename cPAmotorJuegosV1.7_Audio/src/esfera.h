#ifndef ESFERA_H_
#define ESFERA_H_
#include <GL/glut.h>

#include "solid.h"
#include "vector3d.h"

class Esfera: public Solid{
 double r;
public:
 Esfera():Solid(),r(1){}
 Esfera(const Esfera &e):Solid(e),r(e.r){}
 Esfera *clone(){
	 return new Esfera(*this);
 }
 inline float getR(){return r;}
 inline void setR(float rp){r=rp;}
 void render(){
	 glPushMatrix();
	   glColor3f(this->getCol().getX(),this->getCol().getY(),this->getCol().getZ());
	   glTranslatef(this->getPos().getX(),this->getPos().getY(),this->getPos().getZ());
	   glutSolidSphere(r,10,10);
	 glPopMatrix();
 }
 friend std::ostream &operator << (std::ostream &os, const Esfera &v);
};
inline std::ostream &operator<<(std::ostream &os, const Esfera &e){
	os << static_cast<const Solid&>( e );
	os <<",R="<< e.r;
    return os;
}
#endif
