/*
 * solido.h
 *
 *  Created on: 25 Oct 2016
 *      Author: francisco
 */

#ifndef SOLIDO_H_
#define SOLIDO_H_
#include <GL/glut.h>
#include "vector3d.h"

class Solido {
	 Vector3D pos;
	 Vector3D vel;
	 Vector3D color;
	 Vector3D f;
	 double m;
public:
	 Solido();
	 virtual ~Solido();
	 Solido(const Solido &s):pos(s.pos),vel(s.vel),color(s.color),f(s.f),m(s.m){}
	 virtual Solido *clone()=0;
	 inline Vector3D getPos(){return pos;}
	 inline Vector3D getVel(){return vel;}
	 inline Vector3D getCol(){return color;}
	 inline Vector3D getF(){return f;}
	 inline double getM(){return m;}
	 inline void setPos(Vector3D p){pos=p;}
	 inline void setVel(Vector3D v){vel=v;}
	 inline void setCol(Vector3D c){color=c;}
	 inline void setF(Vector3D v){f=v;}
	 inline void setM(double mp){m=mp;}
	 inline void update(float dt){
		 vel=vel+f/m*dt;
		 pos=pos+vel*dt;
		 if(abs(pos.getX())>50.0){
			 vel.setX(vel.getX()*-1);
		 }
		 if(abs(pos.getZ())>50){
		 	 vel.setZ(vel.getZ()*-1);
		 }
		 if(pos.getY()<0){
			 vel.setY(vel.getY()*-1);
			 pos.setY(0);
		 }
	 }
	 virtual void render()=0;
    friend std::ostream &operator << (std::ostream &os, const Solido &v);
};
inline std::ostream &operator<<(std::ostream &os, const Solido &s){
    os << "Pos=" << s.pos << ",Vel=" << s.vel << ",Col=" << s.color;
    return os;
}
#endif /* SOLIDO_H_ */
