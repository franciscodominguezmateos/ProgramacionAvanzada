#ifndef ESFERA_H_
#define ESFERA_H_
#include <GL/glut.h>
#include "vector3d.h"
#include "solido.h"

class Esfera: public Solido{
 double r;
public:
 Esfera():Solido(),r(1){}
 Esfera(const Esfera &e):Solido(e),r(e.r){}
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
	os << static_cast<const Solido&>( e );
	os <<",R="<< e.r;
    return os;
}
#endif
