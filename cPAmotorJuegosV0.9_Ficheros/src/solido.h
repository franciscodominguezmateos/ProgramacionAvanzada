/*
 * solido.h
 *
 *  Created on: 25 Oct 2016
 *      Author: francisco
 */

#ifndef SOLIDO_H_
#define SOLIDO_H_
#include "vector3d.h"

class Solido {
	 Vector3D pos;
	 Vector3D vel;
	 Vector3D color;
public:
	 Solido();
	 virtual ~Solido();
	 Solido(const Solido &s):pos(s.pos),vel(s.vel),color(s.color){}
	 virtual Solido *clone()=0;
	 inline Vector3D getPos(){return pos;}
	 inline Vector3D getVel(){return vel;}
	 inline Vector3D getCol(){return color;}
	 inline void setPos(Vector3D p){pos=p;}
	 inline void setVel(Vector3D v){vel=v;}
	 inline void setCol(Vector3D c){color=c;}
	 inline void update(float dt){
		 Vector3D dp=vel*dt;
		 pos=pos+dp;
	 }
	 virtual void render()=0;
    friend std::ostream &operator << (std::ostream &os, const Solido &v);
};
inline std::ostream &operator<<(std::ostream &os, const Solido &s){
    os << "Pos=" << s.pos << ",Vel=" << s.vel << ",Col=" << s.color;
    return os;
}
#endif /* SOLIDO_H_ */
