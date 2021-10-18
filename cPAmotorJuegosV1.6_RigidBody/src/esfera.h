#ifndef ESFERA_H_
#define ESFERA_H_
#include <GL/glut.h>
#include "vector3d.h"
#include "solido.h"

class Sphere: public Solido{
 double r;
public:
 Sphere():Solido(),r(1){}
 Sphere(const Sphere &e):Solido(e),r(e.r){}
 Sphere *clone(){
	 return new Sphere(*this);
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
 friend std::ostream &operator << (std::ostream &os, const Sphere &v);
};
inline std::ostream &operator<<(std::ostream &os, const Sphere &e){
	os << static_cast<const Solido&>( e );
	os <<",R="<< e.r;
    return os;
}
#endif
