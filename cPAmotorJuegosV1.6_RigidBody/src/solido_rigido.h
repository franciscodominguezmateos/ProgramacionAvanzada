/*
 * solido_rigido.h
 *
 *  Created on: 8 Oct 2019
 *      Author: Francisco Dominguez
 */

#ifndef SOLIDO_RIGIDO_H_
#define SOLIDO_RIGIDO_H_
#include <opencv2/opencv.hpp>
#include "solido.h"

class SolidoRigido: public Solido {
	Mat R;      // Orientation matrix
	Mat w;      // Angular velocity (Vector3D column)
	Mat Ib;     // Inertia tensor of initial body pose
	Mat Ibinv;  // Inverse inertia tensor of initial body pose
	Mat Iinv;   // Inverse of Inertia Tensor
	Mat L;      // Angular momentum (Vector3D column)
	Mat T;      // Total Torque or Total Angular Force
	vector<Vector3D> corners;
public:
	Textura tex;
	SolidoRigido(double w2,double h2,double d2){
		R=Mat::eye(3,3,CV_64F);   // Identity matrix
		//w=Mat:.zeros(3,1,CV_64F); // Not angular velocity yet
		L=Mat::zeros(3,1,CV_64F); // Not angular mometum yet
		L.at<double>(0,0)=0.01;
		L.at<double>(2,0)=0.02;
		L.at<double>(2,0)=0.01;
		T=Mat::zeros(3,1,CV_64F); // Not torque yet
		// Rectangular block Inertial tensor
		Ib=Mat::zeros(3,3,CV_64F);
		double m=getM();
		Ib.at<double>(0,0)=m*(w2*w2+h2*h2)/12;;
		Ib.at<double>(1,1)=m*(d2*d2+h2*h2)/12;;
		Ib.at<double>(2,2)=m*(w2*w2+d2*d2)/12;;
		Ibinv=Ib.inv();
		updateWIinv();

		// shape
		double w=w2/2;
		double h=h2/2;
		double d=d2/2;
		//     5---1
		//    /   /|
		//   4---0 |
		//   |   | 2
		//   |   |/
		//   7---3
		//square of the right
		//right/up/front point
		Vector3D p;
		p=Vector3D( w, h, d);
		corners.push_back(p);
		//right/up/back point
		p=Vector3D( w, h,-d);
		corners.push_back(p);
		//right/bottom/back point
		p=Vector3D( w,-h,-d);
		corners.push_back(p);
		//right/bottom/front point
		p=Vector3D( w,-h, d);
		corners.push_back(p);
		//squre of the left
		p=Vector3D(-w, h, d);
		corners.push_back(p);
		p=Vector3D(-w, h,-d);
		corners.push_back(p);
		p=Vector3D(-w,-h,-d);
		corners.push_back(p);
		p=Vector3D(-w,-h, d);
		corners.push_back(p);
		setCol(Vector3D(0.9,0.9,0));
	}
	virtual ~SolidoRigido(){}
	inline Vector3D getCorner(int i){
		Mat c=asMat(corners[i]);
		Mat p;
		p=R*c;
		return asVector3D(p);
	}
	void updateWIinv(){
		Iinv=R*Ibinv*R.t();
		w=Iinv*L;
	}
	void update(double dt){
		// ground collision
		Vector3D pcm=getPos();
		cout << "pcm="<<pcm<<endl;
		for(unsigned int i=0;i<corners.size();i++){
			Vector3D v=getCorner(i)+pcm; //corner in global frame
			cout << "v="<<v<<endl;
			if(v.getY()<0){
				calculaTorque(Vector3D(0,-v.getY(),0),v);
				aplicaFuerza(Vector3D(0,-20*v.getY(),0));
			}
		}

		// LINEAR UPDATE
		//Solido::update(dt);
		Vector3D vel=getVel()+getF()/getM()*dt;
		setVel(vel);
		Vector3D pos=getPos()+vel*dt;
		setPos(pos);

		// ANGULAR UPDATE
		// Update orientation
		// Get the orientation increment in axis angle form
		Mat dw;
		dw=w*dt;
		// Get the orientation increment in matrix form with Rodrigues formula
		Mat dR;
		Rodrigues(dw,dR);
		// update orientation matrix
		R=dR*R;
		// Update angular momentum
		L=L+dt*T;
		cout <<"F="<<getF()<<endl;
		cout <<"V="<<getVel()<<endl;
		cout <<"T="<< T.at<double>(0,0)<<","<< T.at<double>(1,0)<<","<< T.at<double>(2,0)<<endl;
		cout <<"L="<< L.at<double>(0,0)<<","<< L.at<double>(1,0)<<","<< L.at<double>(2,0)<<endl;
		// Update Inverse inertia tensor with new orientation
		// and Angular velocity with new inverse Inertia tensor
		updateWIinv();
	}
	virtual void limpiaTorque(){T=Mat::zeros(3,1,CV_64F);}
	void limpiaFuerza(){
		Solido::limpiaFuerza();
		limpiaTorque();
	}
	virtual void calculaTorque(Vector3D fv, Vector3D pw){
		Vector3D pcm=getPos();
		Vector3D pb=pcm-pw;// position on body frame
		Vector3D tv=pb.X(fv);
		Mat tm=asMat(tv);
		T=T+tm;
		//cout <<"T="<< T.at<double>(0,0)<<","<< T.at<double>(1,0)<<","<< T.at<double>(2,0)<<endl;
	}
	virtual void aplicaTorque (Vector3D tv){
		Mat tm=asMat(tv);
		T=T+tm;
	}
	//     5---1
	//    /   /|
	//   4---0 |
	//   |   | 2
	//   |   |/
	//   7---3
	//square of the right
	//right/up/front point
	inline void render(){
		glPushMatrix();
		glTranslatef(this->getPos().getX(),this->getPos().getY(),this->getPos().getZ());
		Rectangulo r0(
				getCorner(0),
				getCorner(1),
				getCorner(2),
				getCorner(3));
		//r0.getTex()=tex;
		r0.setCol(getCol());
		r0.render();
		Rectangulo r1(
				getCorner(4),
				getCorner(5),
				getCorner(6),
				getCorner(7));
		//r1.getTex()=tex;
		r1.render();
		Rectangulo r2(
				getCorner(0),
				getCorner(3),
				getCorner(7),
				getCorner(4));
		//r2.getTex()=tex;
		r2.render();
		Rectangulo r3(
				getCorner(1),
				getCorner(2),
				getCorner(6),
				getCorner(5));
		//r3.getTex()=tex;
		r3.render();
		Rectangulo r4(
				getCorner(1),
				getCorner(0),
				getCorner(4),
				getCorner(5));
		//r2.getTex()=tex;
		r4.render();
		Rectangulo r5(
				getCorner(3),
				getCorner(7),
				getCorner(6),
				getCorner(2));
		//r3.getTex()=tex;
		r5.setCol(getCol());
		r5.render();
		glPopMatrix();
	}
};

#endif /* SOLIDO_RIGIDO_H_ */
