#pragma once
#include "solid.h"
#include "vector3d.h"
class Sphere;
using SpherePtr=Sphere*;
class Sphere: public Solid{
 double r;
public:
 Sphere(double r=1):Solid(),r(r){}
 Sphere(const Sphere &e):Solid(e),r(e.r){}
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
	os << static_cast<const Solid&>( e );
	os <<",R="<< e.r;
    return os;
}

