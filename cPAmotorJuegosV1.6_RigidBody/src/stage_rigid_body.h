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
		//rigidBodies[0]->setRot(Vector3D(30,15,15));
		rigidBodies[1]->setPos(Vector3D(0,3.75,0));
		rigidBodies[1]->setRot(Vector3D(30,15,15));
		for(int i=0;i<100;i++){
			double L=25;
			SolidRigidBody* s=new SolidRigidBody(getRand(3,0.25),getRand(3,0.25),getRand(3,0.25));
			s->setPos(Vector3D(getRand(  L, -L),getRand( 10)    ,getRand(  L, -L)));
			s->setRot(Vector3D(getRand( 30,-30),getRand( 30,-30),getRand( 30,-30)));
			rigidBodies.push_back(s);
		}
	}
	void solveColisionNoSure(vector<Contact> &contactsNoSure){
		for(unsigned int i=0;i<rigidBodies.size();i++)
			for(unsigned int j=i+1;j<rigidBodies.size();j++)
				if(rigidBodies[i]->colisionEsphere(rigidBodies[j]))
					contactsNoSure.push_back(Contact(rigidBodies[i],rigidBodies[j]));
	}
	void solveColision(Contact &c){
		SolidRigidBody* sp=c.getA();
		SolidRigidBody* st=c.getB();
		vector<Vector3D> vp=sp->getCorners();
		for(Vector3D &p:vp){
			if(st->contain(p)){
				c.addContactPoint(p);
				Triangle t=st->nearestTriangle(p);
				Vector3D vn=t.getNormal();
				c.setNormal(vn);
				double dist=t.distance(p);
				c.setPenetration(max(c.getPenetration(),-dist));
			}
		}
	}
	void update(double dt){
		SolidRigidBody* dummy=new SolidRigidBody(1,1,1);
		dummy->hazFija();
		vector<Contact> contacts;
		vector<Contact> contactsNoSure;
		solveColisionNoSure(contactsNoSure);
		//check corners of A in triangles of B
		for(Contact &c:contactsNoSure){
			solveColision(c);
			if(c.hasContactPoints()) contacts.push_back(c);
		}
		//check corners of B in triangles of A
		for(Contact &c0:contactsNoSure){
			Contact c(c0.getB(),c0.getA());
			solveColision(c);
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
				double L=25;
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
				double L=25;
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
