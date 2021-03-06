/*
 * solido_rigido.h
 *
 *  Created on: 8 Oct 2019
 *      Author: Francisco Dominguez
 */
#pragma once
#include <opencv2/opencv.hpp>
#include "solido.h"
#include "segment.h"

//for the moment are just boxes
class SolidRigidBody: public Solido {
	Mat R;      // Orientation matrix
	Mat w;      // Angular velocity (Vector3D column)
	Mat Ib;     // Inertia tensor of initial body pose
	Mat Ibinv;  // Inverse inertia tensor of initial body pose
	Mat Iinv;   // Inverse of Inertia Tensor
	Mat L;      // Angular momentum (Vector3D column)
	Mat T;      // Total Torque or Total Angular Force
	double staticFriction;
	double dynamicFriction;
	double restitution;
	vector<Vector3D> corners;
	//this is the radius of a sphere where is embedded the object
	double r;
	bool showSphere;
public:
	Texture tex;
	inline double getRadius(){return r;}
	inline double getRestitution()    {return restitution;}
	inline double getStaticFriction() {return staticFriction;}
	inline double getDynamicFriction(){return dynamicFriction;}
	double getInvM(){
		if(esFija()) return 0;
		return 1.0/getM();
	}
	Mat getInvI(){
		if(esFija()) return Mat::zeros(3,3,CV_64F);
		Iinv=R*Ibinv*R.t();
		return Iinv;
	}
	void setRot(Vector3D r){
		Solido::setRot(r);
		R=eulerAnglesToRotationMatrix(r);
	}
	void setRot(Mat r){R=r;}
	//Pos is the COM location
	void setPos(Vector3D p){Solido::setPos(p);}
	void setPos(Mat p){setPos(Vector3D(p.at<double>(0,0),
			                           p.at<double>(1,0),
									   p.at<double>(2,0)));
	}
	void setVel(Vector3D p){Solido::setVel(p);}
	void setVel(Mat v){setVel(Vector3D(v.at<double>(0,0),
			                           v.at<double>(1,0),
									   v.at<double>(2,0)));
	}
	inline Vector3D getRot(){return rotationMatrixToEulerAngles(R);}
	inline Mat     &getRotMat(){return R;}
	inline Vector3D getW(){return Vector3D(w.at<double>(0,0),w.at<double>(1,0),w.at<double>(2,0));}
	inline Mat      getPosMat(){return getPos().asMat();}
	inline Mat      getVelMat(){return getVel().asMat();}
	inline Vector3D getVelAtPoint(Vector3D p){
		Vector3D rp=p-getPos();
		Vector3D vp=getVel()+getW().X(rp);
		return vp;
	}
	SolidRigidBody(double w2,double h2,double d2){
		R=Mat::eye(3,3,CV_64F);   // Identity matrix
		//w=Mat:.zeros(3,1,CV_64F); // Not angular velocity yet
		L=Mat::zeros(3,1,CV_64F); // Not angular mometum yet
		//L.at<double>(0,0)=0.0;
		//L.at<double>(2,0)=0.0;
		//L.at<double>(2,0)=0.0;
		T=Mat::zeros(3,1,CV_64F); // Not torque yet
		// Rectangular block Inertial tensor
		Ib=Mat::zeros(3,3,CV_64F);
		setM(1);
		double m=getM();
		Ib.at<double>(0,0)=m*(d2*d2+h2*h2)/12;;
		Ib.at<double>(1,1)=m*(w2*w2+d2*d2)/12;;
		Ib.at<double>(2,2)=m*(w2*w2+h2*h2)/12;;
		Ibinv=Ib.inv();
		updateWIinv();
		staticFriction=0.1;
		dynamicFriction=0.1;
		restitution=0.1;
		// shape
		double w=w2/2;
		double h=h2/2;
		double d=d2/2;
		//r=max(max(w,h),d);
		r=sqrt(w*w+h*h+d*d);
		showSphere=false;
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
	//     5---1
	//    /   /|
	//   4---0 |
	//   |   | 2
	//   |   |/
	//   7---3
	inline vector<Segment> getEdges(){
		vector<Segment> e;
		e.push_back(Segment(getCorner(0),getCorner(1)));
		e.push_back(Segment(getCorner(1),getCorner(2)));
		e.push_back(Segment(getCorner(2),getCorner(3)));
		e.push_back(Segment(getCorner(3),getCorner(0)));
		e.push_back(Segment(getCorner(4),getCorner(5)));
		e.push_back(Segment(getCorner(5),getCorner(6)));
		e.push_back(Segment(getCorner(6),getCorner(7)));
		e.push_back(Segment(getCorner(7),getCorner(4)));
		e.push_back(Segment(getCorner(4),getCorner(0)));
		e.push_back(Segment(getCorner(5),getCorner(1)));
		e.push_back(Segment(getCorner(6),getCorner(2)));
		e.push_back(Segment(getCorner(7),getCorner(3)));
		return e;
	}
	inline vector<Vector3D> getCorners(){
		vector<Vector3D> vc;
		for(unsigned int i=0;i<corners.size();i++)
			vc.push_back(getCorner(i));
		return vc;
	}
	inline Vector3D getCorner(int i){
		Vector3D &c=corners[i];
		Mat p=R*c+getPos();
		Vector3D vp=asVector3D(p);
		return vp;
	}
	void updateWIinv(){
		w=getInvI()*L;
	}
	inline void workOutLinearSpeed(double dt){
		// LINEAR UPDATE
		//Solido::update(dt);
		Vector3D vel=getVel()+getF()*getInvM()*dt;
		setVel(vel);
	}
	inline void workOutAngularMomentum(double dt){
		// Update angular momentum
		L=L+T*dt;
	}
	inline void workOutSpeeds(double dt){
		workOutLinearSpeed(dt);
		workOutAngularMomentum(dt);
	}
	inline void updatePose(double dt){
		Vector3D pos=getPos()+getVel()*dt;
		setPos(pos);

		// ANGULAR UPDATE
		// Update orientation
		// Get the orientation increment in axis angle form
		updateWIinv();
		Mat dw;
		dw=w*dt;
		// Get the orientation increment in matrix form with Rodrigues formula
		Mat dR;
		Rodrigues(dw,dR);
		// update orientation matrix
		R=dR*R;

	}
	void update(double dt){
		// ground collision
		Vector3D pcm=getPos();

		cout <<"F="<<getF()<<endl;
		cout <<"T="<< T.at<double>(0,0)<<","<< T.at<double>(1,0)<<","<< T.at<double>(2,0)<<endl;
		cout <<"L="<< L.at<double>(0,0)<<","<< L.at<double>(1,0)<<","<< L.at<double>(2,0)<<endl;
		cout <<"w="<<getW()<<endl;
		cout <<"V="<<getVel()<<endl;
		cout <<"R="<< getRot() <<endl;
		cout <<"p="<< getPos() <<endl;
		// Update Inverse inertia tensor with new orientation
		// and Angular velocity with new inverse Inertia tensor
	}
	virtual void limpiaTorque(){T=Mat::zeros(3,1,CV_64F);}
	void limpiaFuerza(){
		Solido::limpiaFuerza();
		limpiaTorque();
	}
	//Compute torque generated by a force f in a point pw of this solid
	virtual Vector3D torqueFromForce(Vector3D f, Vector3D pw){
		Vector3D pcm=getPos();
		Vector3D pb=pw-pcm;// position on body frame
		Vector3D torque=pb.X(f);
		return torque;
	}
	virtual void aplicaTorque (Vector3D tv){
		Mat tm=asMat(tv);
		T=T+tm;
	}
	void applyImpulse(Vector3D impulse,Vector3D contactVector){
		Vector3D vi=impulse*getInvM();
		setVel(getVel()+vi);
		Vector3D cvi=contactVector.X(impulse);
		L=L+cvi;
		//w+=getInvI()*cvi;
	}
	//     5---1
	//    /   /|
	//   4---0 |
	//   |   | 2
	//   |   |/
	//   7---3
	//square of the right
	//right/up/front point
	vector<Rectangle> getRectangles(){
		vector<Rectangle> vr;
		Rectangle r0(
				getCorner(3),
				getCorner(2),
				getCorner(1),
				getCorner(0));
		Rectangle r1(
				getCorner(4),
				getCorner(5),
				getCorner(6),
				getCorner(7));
		Rectangle r2(
				getCorner(0),
				getCorner(4),
				getCorner(7),
				getCorner(3));
		Rectangle r3(
				getCorner(1),
				getCorner(2),
				getCorner(6),
				getCorner(5));
		Rectangle r4(
				getCorner(1),
				getCorner(5),
				getCorner(4),
				getCorner(0));
		Rectangle r5(
				getCorner(3),
				getCorner(7),
				getCorner(6),
				getCorner(2));
		vr.push_back(r0);
		vr.push_back(r1);
		vr.push_back(r2);
		vr.push_back(r3);
		vr.push_back(r4);
		vr.push_back(r5);
		return vr;
	}
	bool colisionEsphere(SolidRigidBody* sr){
		double rr=r+sr->r;
		double dist=getPos().distance(sr->getPos());
		return dist<rr;
	}
	bool contain(Vector3D pt){
		for(Rectangle r:getRectangles()){
			if(r.distance(pt)>0)
				return false;
		}
		return true;
	}
	Triangle nearestTriangle(Vector3D pt){
		vector<Triangle> vt=getTriangles();
		double minDist=10e40;
		double minIdx=-1;
		for(unsigned int i=0;i<vt.size();i++){
			double dist=vt[i].distance(pt);
			if(abs(dist)<abs(minDist)){
				minDist=dist;
				minIdx=i;
			}
		}
		return vt[minIdx];
	}
	vector<Triangle> getTriangles(){
		vector<Rectangle> vr=getRectangles();
		vector<Triangle> vt;
		for(Rectangle &r:vr){
			vt.push_back(r.getTriangle0());
			vt.push_back(r.getTriangle1());
		}
		return vt;
	}
	void setShowSphere(bool b=true){showSphere=b;}
	inline void render(){
		for(Rectangle &r:getRectangles()){
			r.setCol(getCol());
			//r.setDrawNormals();
			r.render();
		}
		if(showSphere){
		 glPushMatrix();
		   //glColor3f(this->getCol().getX(),this->getCol().getY(),this->getCol().getZ());
		   glColor3f(1,0,0);
		   glTranslatef(this->getPos().getX(),this->getPos().getY(),this->getPos().getZ());
		   //glutSolidSphere(r,10,10);
		   glutWireSphere(r,10,10);
		 glPopMatrix();
		}
	}
};

