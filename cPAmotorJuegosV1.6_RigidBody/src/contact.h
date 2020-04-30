/*
 * contact.h
 *
 *  Created on: 30 Oct 2019
 *              25 Apr 2020
 *      Author: Francisco Dominguez
 * From: http://gamedev.tutsplus.com/tutorials/implementation/create-custom-2d-physics-engine-aabb-circle-impulse-resolution/
 *       https://gamedevelopment.tutsplus.com/tutorials/how-to-create-a-custom-2d-physics-engine-oriented-rigid-bodies--gamedev-8032
 */
#pragma once
#include "solid_rigid_body.h"
class Contact {
	SolidRigidBody* a;
	SolidRigidBody* b;
	double penetration;
	Vector3D normal;
	vector<Vector3D> contactPoints;
	double me; //Mixed restitution;
	double mdf;//Mixed dynamic friction
	double msf;//Mixed static friction
public:
	bool hasContactPoints(){return contactPoints.size()>0;}
	Contact(SolidRigidBody* a,SolidRigidBody* b):a(a),b(b){}
	void setNormal(Vector3D n){normal=n;}
	void setPenetration(double p){penetration=p;}
	double getPenetration(){return penetration;}
	void addContactPoint(Vector3D cp){contactPoints.push_back(cp);}
	vector<Vector3D> &getContactPoints(){return contactPoints;}
	void solve(){
		//TODO: detectCollision(a,b);
	}
	void init(){
		int contactCount=contactPoints.size();
		// Calculate mix restitution
		me=min(a->getRestitution(),b->getRestitution());
		// Calculate static and dynamic friction
		msf=sqrt(a->getStaticFriction() *b->getStaticFriction() );
		mdf=sqrt(a->getDynamicFriction()*b->getDynamicFriction());
/*		for(int i=0;i<contactCount;i++){
			//Calculate radii from COM to contact
			Vector3D ra=contactPoints[i]-a->getPos();
			Vector3D rb=contactPoints[i]-b->getPos();
			Vector3D va=a->getVel()+a->getW().X(ra);
			Vector3D vb=b->getVel()-b->getW().X(rb);
			Vector3D rv=vb-va;
			// Determine if we should perform a resting collision or not
			// The idea is if the only thing moving this object is gravity,
			// then the collision should be performed without any restitution
			double dt=0.1;
			Vector3D gravity(0,-9.8,0);
			double EPSILON=0.1;
			if(rv.lengthSquared()<(gravity*dt).lengthSquared()+EPSILON)
				me=0.0;
		}
		*/
	}
	void applyImpulse(){
		//Early out and positional correction if both objects have infinite mass
		if(nearZero(a->getInvM()+b->getInvM())){
			infiniteMassCorrection();
			return;
		}
		int contactCount=contactPoints.size();
		vector<Vector3D> in,ran,rbn;
		vector<Vector3D> it,rat,rbt;
		for(int i=0;i<contactCount;i++){
			//Calculate radii from COM to contact
			Vector3D ra=contactPoints[i]-a->getPos();
			Vector3D rb=contactPoints[i]-b->getPos();
			Vector3D va=a->getVel()+a->getW().X(ra);
			Vector3D vb=b->getVel()-b->getW().X(rb);
			Vector3D rv=va-vb;

			// Relative velocity along the normal
			double contactVel=rv*normal;
			// Do not resolve if velocities are separating
			if(contactVel>0) return;
			if(abs(contactVel)<0.01) return;
			Vector3D raCrossN=ra.X(normal);
			Vector3D rbCrossN=rb.X(normal);
			Vector3D invIraxN=asVector3D(a->getInvI()*raCrossN);
			Vector3D invIrbxN=asVector3D(b->getInvI()*rbCrossN);
			double invMassSum=a->getInvM()+b->getInvM()+
					          normal*invIraxN.X(ra)+
							  normal*invIrbxN.X(rb);
			// Calculate impulse scalar
			double j= -contactVel*(1+me);
			j/=invMassSum;
			j/=(double)contactCount;
			//Apply impulse
			Vector3D impulse=normal*j;
			in.push_back(impulse);
			ran.push_back(ra);
			rbn.push_back(rb);
			// Friction impulse
			//va=a->getVel()+a->getW().X(ra);
			//vb=b->getVel()-b->getW().X(rb);
			//rv=va-vb;
			Vector3D t=rv-normal*(rv*normal);
			// Don't appy tiny friction impulses
			if(nearZero(t.length())) break;
			t.normalize();
			// j tangent magnitude
			double jt= -rv*t;
			jt/=invMassSum;
			jt/=(double)contactCount;
			// Don't appy tiny friction impulses
			if(nearZero(jt)) break;
			// Coulumb's law
			Vector3D tangentImpulse;
			if(abs(jt)<j*msf)
				tangentImpulse=t*jt;
			else
				tangentImpulse=t*-j*mdf;
			// Apply friction impulse
			a->applyImpulse( tangentImpulse,ra);
			b->applyImpulse(-tangentImpulse,rb);
		}
		for(unsigned i=0;i<in.size();i++){
			Vector3D &impulse=in[i];
			Vector3D &ra=ran[i];
			Vector3D &rb=rbn[i];
			a->applyImpulse( impulse,ra);
			b->applyImpulse(-impulse,rb);
		}
	}
	void positionalCorrection(){
		const double k_slop=0.001; //Penetration allowance
		const double percent=0.2; //Penetration percentage to correct
		double   correctionMag=(max(penetration-k_slop,0.0)/(a->getInvM()+b->getInvM()));
		Vector3D correction=normal*correctionMag*percent;
		a->setPos(a->getPos()+correction*a->getInvM());
		b->setPos(b->getPos()-correction*b->getInvM());
	}
	void infiniteMassCorrection(){
		a->setVel(Vector3D());
		b->setVel(Vector3D());
	}
};

