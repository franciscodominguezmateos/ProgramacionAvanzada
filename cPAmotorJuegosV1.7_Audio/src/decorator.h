/*
 * decorator.h
 *
 *  Created on: 28 May 2020
 *      Author: Francisco Dominguez
 */
#pragma once
#include "solido.h"
class Decorator: public Solido {
	Solido* decorated;
public:
	Decorator(Solido* s){decorated=s;}
	inline Solido* &getDecorated(){return decorated;}
	virtual Vector3D getPos(){
		return decorated->getPos();
	}
/*	 void setSuelo(double s){suelo=s;}
	 double getSuelo(){return suelo;}
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
	 */
	 virtual void render(){
		   /*glPushMatrix();
		   glColor3f(this->getCol().getX(),this->getCol().getY(),this->getCol().getZ());
		   glTranslatef(this->getPos().getX(),this->getPos().getY(),this->getPos().getZ());
		   glutSolidSphere(0.1,10,10);
		   glPopMatrix();*/
		 decorated->render();
	 }
   //friend std::ostream &operator << (std::ostream &os, const Solido &v);
};
class DecoratorSpin: public Decorator{
	float speed;
public:
	DecoratorSpin(Solido* s):Decorator(s),speed(50){}
	virtual void update(double dt){
		Solido &s=*getDecorated();
		Vector3D nRot=s.getRot()+Vector3D(0,speed*dt,0);
		s.setRot(nRot);
	}
	void setSpeed(float s){speed=s;}
	float getSpeed(){return speed;}
};
