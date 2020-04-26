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
		rigidBodies[0]->setPos(Vector3D(0,2,0));
		rigidBodies[0]->setRot(Vector3D(30,15,15));

	}
	void update(double dt){
		SolidRigidBody* dummy=new SolidRigidBody(1,1,1);
		dummy->hazFija();
		vector<Contact> contacts;
		for(SolidRigidBody* &s:rigidBodies){
			s->limpiaFuerza();
			s->acumulaFuerza(Vector3D(0,-9.8/1.0,0));
			Contact c(s,dummy);
			for(unsigned int i=0;i<8;i++){
				Vector3D v=s->getCorner(i);
				if(v.getY()<0){
					c.addContactPoint(Vector3D(v.getX(),0,v.getZ()));
					c.setPenetration(max(c.getPenetration(),-v.getY()));
					c.setNormal(Vector3D(0,-1,0));
				}
			}
			if(c.hasContactPoints()) contacts.push_back(c);
		}
		// Initialize collision
		for(unsigned int i = 0; i < contacts.size( ); ++i){
			contacts[i].init( );
		    contacts[i].applyImpulse( );
		}
		for(SolidRigidBody* &s:rigidBodies)	s->update(dt);
		for(unsigned int i = 0; i < contacts.size( ); ++i){
		    contacts[i].positionalCorrection();
		}
	}
	void render(){
		for(SolidRigidBody* &s:rigidBodies)
			s->render();
	}

};

#endif /* STAGE_RIGID_BODY_H_ */
