#ifndef CUBO_H_
#define CUBO_H_
#include <GL/glut.h>
#include "vector3d.h"
#include "solido.h"
class Cubo: public Solido{
 float s;
public:
 inline float getS(){return s;}
 inline void setS(float sp){s=sp;}
 void render();
};
#endif
