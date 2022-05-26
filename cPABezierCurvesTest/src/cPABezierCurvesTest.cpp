//============================================================================
// Name        : cPABezierCurvesTest.cpp
// Author      : Francisco Dominguez
// Version     :
// Copyright   : Your copyright notice
// Description : Bezier Cubic Curves
// Date: 22/05/2022
// from: The Beauty of Bézier Curves
//       https://www.youtube.com/watch?v=aVwxzDHniEw
//============================================================================

#include <iostream>
#include "game_engine.h"
#include "game_standard.h"
#include "axis.h"
#include "rectangle.h"
#include "sphere.h"
#include "shader_toy.h"
#include "segment.h"
#include "axis.h"

using namespace std;

class BezierCurveSegments{
public:
	vector<Vector3D> points;
	vector<Segment> segments;
	BezierCurveSegments(){}
	BezierCurveSegments(vector<Vector3D>& points):points(points){buildSegments();}
	void add(Vector3D point){points.push_back(point);buildSegments();}
	void buildSegments(){segments=buildSegments(points);}
	vector<Segment> buildSegments(vector<Vector3D>& points){
		vector<Segment> segments;
		for(unsigned int i=0;i<points.size()-1;i++){
			segments.push_back(Segment(points[i],points[i+1]));
		}
		return segments;
	}
	vector<Vector3D> buildPoints(vector<Segment>& segments,double t){
		vector<Vector3D> points;
		for(Segment& s:segments){
			Vector3D p=s.lerp(t);
			points.push_back(p);
		}
		return points;
	}
	BezierCurveSegments reduce(double t){
		BezierCurveSegments bcs;
		bcs.points=buildPoints(segments,t);
		bcs.buildSegments();
		return bcs;
	}
};
class BezierCurveCubic{
	vector<Vector3D> points;
public:
	BezierCurveCubic(Vector3D p0,Vector3D p1,Vector3D p2,Vector3D p3){
		points.push_back(p0);
		points.push_back(p1);
		points.push_back(p2);
		points.push_back(p3);
	}
	Vector3D P(double t){
		//Bernstein Polynomial Form
		double w0=-  t*t*t+3*t*t-3*t+1;
		double w1= 3*t*t*t-6*t*t+3*t;
		double w2=-3*t*t*t+3*t*t;
		double w3=   t*t*t;
		Vector3D& p0=points[0];
		Vector3D& p1=points[1];
		Vector3D& p2=points[2];
		Vector3D& p3=points[3];
		Vector3D p=p0*w0+p1*w1+p2*w2+p3*w3;
		return p;
	}
	// Velocity
	Vector3D dP(double t){
		//Bernstein Polynomial Form
		double w0=-3*t*t+ 6*t-3;
		double w1= 9*t*t-12*t+3;
		double w2=-9*t*t+ 6*t;
		double w3= 3*t*t;
		Vector3D& p0=points[0];
		Vector3D& p1=points[1];
		Vector3D& p2=points[2];
		Vector3D& p3=points[3];
		Vector3D p=p0*w0+p1*w1+p2*w2+p3*w3;
		return p;
	}
	// Acceleration
	Vector3D ddP(double t){
		//Bernstein Polynomial Form
		double w0=- 6*t+ 6;
		double w1= 18*t-12;
		double w2=-18*t+ 6;
		double w3=  6*t;
		Vector3D& p0=points[0];
		Vector3D& p1=points[1];
		Vector3D& p2=points[2];
		Vector3D& p3=points[3];
		Vector3D p=p0*w0+p1*w1+p2*w2+p3*w3;
		return p;
	}
	// Jerk
	Vector3D dddP(double t){
		//Bernstein Polynomial Form
		double w0=- 6;
		double w1= 18;
		double w2=-18;
		double w3=  6;
		Vector3D& p0=points[0];
		Vector3D& p1=points[1];
		Vector3D& p2=points[2];
		Vector3D& p3=points[3];
		Vector3D p=p0*w0+p1*w1+p2*w2+p3*w3;
		return p;
	}
	// Fernet Frame
	Vector3D tangent(double t){
		Vector3D T=dP(t);
		T.normalize();
		return T;
	}
	Vector3D normal(double t){
		Vector3D vt=tangent(t);
		Vector3D va=ddP(t);
		Vector3D r=va.rejectOn(vt);
		r.normalize();
		return r;
	}
	Vector3D binormal(double t){
		Vector3D T=tangent(t);
		Vector3D N=normal(t);
		Vector3D B=T.X(N);
		return B;
	}
	double curvature(double t){
		Vector3D V=dP(t);
		double v=V.length();
		double v2=v*v;
		Vector3D T=V.normalized();
		Vector3D va=ddP(t);
		Vector3D aN=va.rejectOn(T);
		double an=aN.length();
		double c=an/v2;
		return c;
	}
	double radious(double t){
		//TODO: Division by ZERO
		return 1/curvature(t);
	}
	Vector3D OscilatingCircleCenter(double t){
		Vector3D p=P(t);
		Vector3D n=normal(t);
		Vector3D c=p+n*radious(t);
		return c;
	}
	vector<Vector3D> buildPoints(double n){
		vector<Vector3D> r;
		double dt=1.0/n;
		for(double t=0;t<1;t+=dt){
			Vector3D p=P(t);
			r.push_back(p);
		}
		return r;
	}
	float dist2T(vector<double> LUT,double distance){
		unsigned int N=LUT.size();
		double arcLength=LUT[N-1];
		if(distance >=0 && distance <=arcLength){
			for(unsigned int i=0;i<N-1;i++){
				if(distance>=LUT[i] && distance <LUT[i+1]){
					distance=linearMap(distance,LUT[i],LUT[i+1],(double)i/(N-1),(double)(i+1)/(N-1));
				}
			}
		}
		return distance/arcLength;
	}
};
class PolyLine: public Solid{
	vector<Vector3D> points;
public:
	void setPoints(const vector<Vector3D>& pts){
		points=pts;
	}
	inline void addPoint(Vector3D p){points.push_back(p);}
	void addPoints(const vector<Vector3D> pts){

	}
	void render(){
		Vector3D c=this->getCol();
		glColor3f(c.getX(),c.getY(),c.getZ());
		glLineWidth(3);
		glBegin(GL_LINE_STRIP);
		  for(Vector3D& p:points)
			  glVertex3f(p.getX(),p.getY(),p.getZ());
		glEnd();
	}
};
class BezierCurvesTest:public GameStandard{
	vector<Vector3D> points;
	vector<Segment> segments;
	BezierCurveSegments bcs;
	Sphere* psp;
	PolyLine* ppl;
	BezierCurveCubic* pbcc;
public:
	BezierCurvesTest(string title):GameStandard(title){}
	//virtual void mouseMoved(int x, int y){	}
	//virtual void mousePress(int button, int state, int x, int y){}
	//virtual void keyPressed(char key,int x,int y){}
	void init(){
		Camera& cam  =getCamera();
		Stage&  stage=getStage();
		cam.setPos(Vector3D(0,0,10));
		Light* lightFront=new Light(Vector3D(150.0,150.0, 100.0));
		stage.add(lightFront);
		stage.add(new Light(Vector3D(-100.0,150.0, -150.0)));

		vector<Vector3D> points={{0.5,0,0},{0.8,1,0}};

		bcs=BezierCurveSegments(points);
		//points.push_back(Vector3D(0.5,0,0));
		//points.push_back(Vector3D(0.8,1,0));
		//points.push_back(Vector3D(1.8,1));
		//points.push_back(Vector3D(1.5,0));
		//for(unsigned int i=0;i<points.size()-1;i++){
		//	segments.push_back(Segment(points[i],points[i+1]));
		//}
		for(Segment& sg:bcs.segments){
			stage.add(&sg);
		}
		pbcc=new BezierCurveCubic(Vector3D(0.5,0,0),
				Vector3D(0.8,1,0),
				Vector3D(1.8,1),
				Vector3D(1.5,0));


		psp=new Sphere(0.05);
		psp->hazFija();
		stage.add(psp);

		Axis* pa=new Axis();
		stage.add(pa);

		ppl=new PolyLine();
		//ppl->addPoint(Vector3D(0,0,0));
		//ppl->addPoint(Vector3D(1,1,0));
		//ppl->addPoint(Vector3D(0,1,1));
		//ppl->addPoint(Vector3D(0,0,1));
		ppl->setPoints(pbcc->buildPoints(100));
		stage.add(ppl);

	}
	void update(double dt){
		double t=(1+sin(getTime()/10))/2;
		/*Vector3D p0=pseg0->lerp(t);
		Vector3D p1=pseg1->lerp(t);
		Vector3D p2=pseg2->lerp(t);
		Segment seg01(p0,p1);
		Segment seg12(p1,p2);
		Vector3D p01=seg01.lerp(t);
		Vector3D p12=seg12.lerp(t);
		Segment seg0112(p01,p12);
		Vector3D p=seg0112.lerp(t);
		psp->setPos(p);*/
		//if (waitKey(1) == 27) exit(0);
	}
} bezierCurvesTest("cPABezierCurvesTest");

int main(int argc, char** argv) try{
	srand(10);
	GameEngine::setGame(&bezierCurvesTest);
	GameEngine::gameInit(argc,argv);
	GameEngine::gameMainLoop();
	return 0;
}
catch (exception &e){
	cout << "Error in BezierCurvesTest::main(): "<<e.what() <<endl;
	return 1;
}
