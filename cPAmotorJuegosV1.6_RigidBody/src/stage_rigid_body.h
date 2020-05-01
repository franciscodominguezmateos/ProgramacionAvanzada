/*
 * stage_rigid_body.h
 *
 *  Created on: 25 Apr 2020
 *      Author: Francisco Dominguez
 */

#ifndef STAGE_RIGID_BODY_H_
#define STAGE_RIGID_BODY_H_
#include "contact.h"
#include "composite.h"
class StageRigidBody:public Solido {
	vector<SolidRigidBody*> rigidBodies;
public:
	StageRigidBody(){
		rigidBodies.push_back(new SolidRigidBody(1,2,3));
		rigidBodies.push_back(new SolidRigidBody(1,2,3));
		rigidBodies[0]->setPos(Vector3D(0,1,0));
		rigidBodies[0]->setRot(Vector3D(30,15,15));
		rigidBodies[1]->setPos(Vector3D(0,3.5,0));
		rigidBodies[1]->setRot(Vector3D(30,15,15));
		for(int i=0;i<30;i++){
			double L=50;
			SolidRigidBody* s=new SolidRigidBody(1,2,3);
			s->setPos(Vector3D(getRand(  L, -L),getRand( 10)    ,getRand(  L, -L)));
			s->setRot(Vector3D(getRand( 30,-30),getRand( 30,-30),getRand( 30,-30)));
			rigidBodies.push_back(s);
		}
	}
	void solveColision(SolidRigidBody* sp,SolidRigidBody* st,Contact &c){
		vector<Vector3D> vp=sp->getCorners();
		c.setPenetration(0);
		for(Vector3D &p:vp){
			if(st->contain(p)){
				c.addContactPoint(p);
				Triangle t=st->nearestTriangle(p);
				c.setNormal(t.getNormal());
				c.setPenetration(max(c.getPenetration(),-t.distance(p)));
			}
		}
	}
	void update(double dt){
		SolidRigidBody* dummy=new SolidRigidBody(1,1,1);
		dummy->hazFija();
		vector<Contact> contacts;
		vector<Contact> contactsNoSure;
		for(unsigned int i=0;i<rigidBodies.size();i++)
			for(unsigned int j=i+1;j<rigidBodies.size();j++)
				if(rigidBodies[i]->colisionEsphere(rigidBodies[j]))
					contactsNoSure.push_back(Contact(rigidBodies[i],rigidBodies[j]));
		for(Contact &c:contactsNoSure){
			solveColision(c.getA(),c.getB(),c);
			if(c.hasContactPoints()) contacts.push_back(c);
		}
		for(Contact &c:contactsNoSure){
			solveColision(c.getB(),c.getA(),c);
			if(c.hasContactPoints()) contacts.push_back(c);
		}
		//Collision with ground
		for(SolidRigidBody* &s:rigidBodies){
			Contact c(s,dummy);
			c.setPenetration(0);
			Vector3D v;
			for(unsigned int i=0;i<8;i++){
				v=s->getCorner(i);
				if(v.getY()<0.0){
					c.addContactPoint(Vector3D(v.getX(),0,v.getZ()));
					c.setPenetration(max(c.getPenetration(),-v.getY()));
					c.setNormal(Vector3D(0,1,0));
				}
			}
			if(c.hasContactPoints()) contacts.push_back(c);
		}
		//Collision  X
		for(SolidRigidBody* &s:rigidBodies){
			Contact c(s,dummy);
			c.setPenetration(0);
			Vector3D v;
			for(unsigned int i=0;i<8;i++){
				double L=50;
				v=s->getCorner(i);
				if(abs(v.getX())>L){
					c.getA()->setShowSphere();
					c.addContactPoint(v);
					c.setPenetration(max(c.getPenetration(),abs(v.getX())-L));
					Vector3D n(-v.getX(),0,0);
					n.normalize();
					c.setNormal(n);
				}
			}
			if(c.hasContactPoints()) contacts.push_back(c);
		}
		//Collision Z
		for(SolidRigidBody* &s:rigidBodies){
			Contact c(s,dummy);
			c.setPenetration(0);
			Vector3D v;
			for(unsigned int i=0;i<8;i++){
				double L=50;
				v=s->getCorner(i);
				if(abs(v.getZ())>L){
					c.getA()->setShowSphere();
					c.addContactPoint(v);
					c.setPenetration(max(c.getPenetration(),abs(v.getZ())-L));
					Vector3D n(0,0,-v.getZ());
					n.normalize();
					c.setNormal(n);
				}
			}
			if(c.hasContactPoints()) contacts.push_back(c);
		}
		for(SolidRigidBody* &s:rigidBodies){
			s->limpiaFuerza();
			s->acumulaFuerza(Vector3D(0,-9.8*s->getM(),0));
		}
		for(SolidRigidBody* &s:rigidBodies)	s->workOutSpeeds(dt);
		// Initialize collision
		for(unsigned int i = 0; i < contacts.size( ); ++i){
			contacts[i].init( );
			for(int j=0;j<1;j++) contacts[i].applyImpulse( );
		}
		for(SolidRigidBody* &s:rigidBodies)	s->updatePose(dt);
		for(unsigned int i = 0; i < contacts.size( ); ++i){
			//for(unsigned int j=0;j<contacts[i].getContactPoints().size();j++)
		    contacts[i].positionalCorrection();
		}
	}
	void render(){
		for(SolidRigidBody* &s:rigidBodies)
			s->render();
	}
};

#endif /* STAGE_RIGID_BODY_H_ */
