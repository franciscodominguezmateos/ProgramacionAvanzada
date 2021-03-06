/*
 * solido.h
 *
 *  Created on: 25 Oct 2016
 *      Author: francisco
 */
#pragma once
#include <GL/glut.h>
#include "vector3d.h"

class Solido {
	 Vector3D pos;
	 Vector3D rot;
	 Vector3D vel;
	 Vector3D color;
	 Vector3D f;
	 double m;
	 bool fija;
	 double edad;
	 static double floorAll;
	 double suelo;
public:
	 Solido(Vector3D p):pos(p),rot(0,0,0),vel(0,0,0),color(1,1,1),f(0,0,0),m(1.0),fija(false),edad(0){suelo=floorAll;}
	 Solido(double x,double y,double z):Solido(Vector3D(x,y,z)){suelo=floorAll;}
	 Solido():Solido(0,0,0) {suelo=floorAll;}
	 Solido(const Solido &s):pos(s.pos),vel(s.vel),color(s.color),f(s.f),m(s.m),fija(s.fija){}
	 void setSuelo(double s){suelo=s;}
	 double getSuelo(){return suelo;}
	 static void setFloorAll(double s){floorAll=s;}
	 Solido& operator=(const Solido &s){
		 pos=s.pos;
		 vel=s.vel;
		 color=s.color;
		 f=s.f;
		 m=s.m;
		 fija=s.fija;
		 edad=s.edad;
		 return *this;
	 }
	 virtual ~Solido(){}
	 virtual Solido *clone (){return new Solido(*this);}
	 inline Vector3D getPos(){return pos;}
	 inline Vector3D getVel(){return vel;}
	 inline Vector3D getCol(){return color;}
	 inline Vector3D getF(){return f;}
	 inline double getM(){return m;}
	 inline bool getFija(){return fija;}
     virtual Vector3D getRot() {return rot;	}
	 inline void setRot(Vector3D rot) {this->rot = rot;}
	 inline void setPos(Vector3D p){pos=p;}
	 inline void setVel(Vector3D v){vel=v;}
	 inline void setCol(Vector3D c){color=c;}
	 inline virtual void setF(Vector3D v){f=v;}
	 inline void setM(double mp){m=mp;}
	 inline void setFija(bool b){fija=b;}
	 inline void hazFija (){fija=true;}
	 inline void hazMovil(){fija=false;}
	 inline bool esFija(){return fija;}
	 virtual void limpiaFuerza(){f=Vector3D();}
	 virtual void acumulaFuerza(Vector3D fv){f=f+fv;}
	 virtual void aplicaFuerza (Vector3D fv){f=f+fv;}
	 inline virtual void update(double dt){
		 if(fija)
			 return;
		 vel=vel+f/m*dt;
		 pos=pos+vel*dt;
		 if(abs(pos.getX())>50.0){
			 vel.setX(vel.getX()*-0.99);
		 }
		 if(abs(pos.getZ())>50){
		 	 vel.setZ(vel.getZ()*-0.99);
		 }
		 if(pos.getY()<suelo){
			 vel.setY(vel.getY()*-0.99);
			 pos.setY(suelo+0.0001);
		 }
	 }
	 virtual void render(){
		   glPushMatrix();
		   glColor3f(this->getCol().getX(),this->getCol().getY(),this->getCol().getZ());
		   glTranslatef(this->getPos().getX(),this->getPos().getY(),this->getPos().getZ());
		   glutSolidSphere(0.1,10,10);
		   glPopMatrix();
	 }
    friend std::ostream &operator << (std::ostream &os, const Solido &v);
};
inline std::ostream &operator<<(std::ostream &os, const Solido &s){
    os << "Pos=" << s.pos << ",Vel=" << s.vel << ",Col=" << s.color;
    return os;
}
double Solido::floorAll=0;
