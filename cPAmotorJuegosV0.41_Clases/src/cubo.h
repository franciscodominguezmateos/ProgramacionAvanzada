#ifndef CUBO_H_
#define CUBO_H_
#include <GL/glut.h>
#include "vector3d.h"
class Cubo{
 Vector3D pos;
 Vector3D vel;
 Vector3D color;
 Vector3D f;
 double m;
 double s;
public:
 inline Vector3D getPos(){return pos;}
 inline Vector3D getVel(){return vel;}
 inline Vector3D getCol(){return color;}
 inline Vector3D getF(){return f;}
 inline double getM(){return m;}
 inline double getS(){return s;}
 inline void setPos(Vector3D p){pos=p;}
 inline void setVel(Vector3D v){vel=v;}
 inline void setCol(Vector3D c){color=c;}
 inline void setF(Vector3D v){f=v;}
 inline void setM(double mp){m=mp;}
 inline void setS(double sp){s=sp;}
 void render();
 void update(double dt){
	 vel=vel+f/m*dt;
	 pos=pos+vel*dt;
	 if(abs(pos.getX())>2){
		 vel.setX(vel.getX()*-1);
	 }
	 if(abs(pos.getZ())>2){
		 vel.setZ(vel.getZ()*-1);
	 }
	 if(pos.getY()<0){
		 vel.setY(vel.getY()*-1);
		 pos.setY(0);
	 }
 }
};
#endif
