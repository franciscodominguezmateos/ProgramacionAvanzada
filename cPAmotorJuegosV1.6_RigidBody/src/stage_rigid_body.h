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
		for(SolidRigidBody* &s:rigidBodies){
			s->limpiaFuerza();
			s->acumulaFuerza(Vector3D(0,-9.8/1.0,0));
			vector<Contact> contacts;
			for(unsigned int i=0;i<8;i++){
				Vector3D v=s->getCorner(i);
				if(v.getY()<0){
					Contact c(s,dummy);
					c.addContactPoint(Vector3D(v.getX(),0,v.getZ()));
					c.setPenetration(v.getY());
					c.setNormal(Vector3D(0,-1,0));
					contacts.push_back(c);
				}
			}
			// Initialize collision
			for(int i = 0; i < contacts.size( ); ++i){
				contacts[i].init( );
			    contacts[i].applyImpulse( );
			    contacts[i].positionalCorrection();
			}
			s->update(dt);
		}
		/*
		cout << "pcm="<<pcm<<endl;
		for(unsigned int i=0;i<corners.size();i++){
			Vector3D v=getCorner(i); //corner in global frame
			cout << "v="<<v<<endl;
			if(v.getY()<0){
				// here apply impulse
				Vector3D torque=torqueFromForce(Vector3D(0,500,0),v);
				aplicaTorque(torque);
				Vector3D v=getVel();
				v.setY(-v.getY()*0.9);
				setVel(v);
			}
		}
*/
	}
	void render(){
		for(SolidRigidBody* &s:rigidBodies)
			s->render();
	}

};

#endif /* STAGE_RIGID_BODY_H_ */
