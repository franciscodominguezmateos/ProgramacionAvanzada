#ifndef ESFERA_H_
#define ESFERA_H_
#include <GL/glut.h>
#include "vector3d.h"
#include "solido.h"

class Esfera: public Solido{
 float r;
public:
 inline float getR(){return r;}
 inline void setR(float rp){r=rp;}
 void render();
};
#endif
