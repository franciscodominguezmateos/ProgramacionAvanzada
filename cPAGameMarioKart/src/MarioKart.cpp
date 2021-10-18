// this must be the first include it initialize OpenGL/glut and Shaders
#include "game_engine.h"
#include "modelo_material.h"

/*
#include "shader.h"
#include <stdlib.h>
#include <view.h>
#include <iostream>
#include <thread>
#include <mutex>
#include "opencv2/opencv.hpp"
#include "vector3d.h"
#include "esfera.h"
#include "cubo.h"
#include "cilindro.h"
#include "rosco.h"
#include "compuesto.h"
#include "escena.h"
#include "pared.h"
#include "camera_ar.h"
#include "texture.h"
#include "rectangulo.h"
#include "pendulo.h"
#include "cubo_elastico.h"
#include "fondo_textura.h"
#include "luz.h"
#include "proyeccion_perspectiva.h"
#include "pose_estimation_chessboard.h"
#include "material.h"
#include "modelo_material.h"
#include "caja_elastica.h"
#include "caja_modelo_elastico.h"
*/
//Declaration needed in next .h
Mat global_img;
#include "socket_TCP_server.h"
#include "sensors.h"

using namespace cv;

double t=0.0;
double dt=1.0/30;

int mx=-1,my=-1;        // Previous mouse coordinates

Mat tablero;

double vel;

ModeloMaterial* circuit;
ModeloMaterial* mariokart;

Stage e;
Cubo *pc;
ModeloMaterial* m;
Texture tex,ladrillos,paredTex,texTv,spiderTex,marioKartTex,minionTex,mariokartTex;
VideoCapture cap(0);
CuboElastico *ce;
CajaElastica* cje;
CajaModeloElastico* cme;

FondoTextura fondo,fondoTablero;

// Augmented Reality
Mat K=(Mat_<double>(3,3) <<
		 1053.755323784897,                 0, 317.160420774003,
		                 0, 1067.981896308737, 203.4946241428054,
						 0,                 0,                 1);

Mat dist=Mat::zeros(4,1,cv::DataType<double>::type); // Assuming no lens distortion
CamaraAR *camAR=nullptr;
ProjectionCamera pCam(K);
PoseEstimationChessBoard peChessBoard(K,dist);
Texture texTablero;
void setPoseCamAR(Mat &tablero){
	 if(peChessBoard.estimatePose(tablero)){
		 if(camAR)
			 camAR->setPose(peChessBoard.getRvec(),peChessBoard.getTvec());
		 else
			 camAR=new CamaraAR(peChessBoard.getRvec(),peChessBoard.getTvec());
	 }
	 else{
		 cout << "Tablero no detectado!!!"<< endl;
		 camAR->setPose(Mat::zeros(3,1,cv::DataType<double>::type),
				        Mat::zeros(3,1,cv::DataType<double>::type));
	 }
	 texTablero.setImage(tablero);
}
void initCamAR(){
	 tablero=imread("2017-11-03-091218.jpg");
	 setPoseCamAR(tablero);
}
ProjectionPerspective proyeccion;
vector<View> vistas={{0.0,0.0,0.5,1,&proyeccion},{0.5,0.0,0.5,1,&pCam}};//,{0.0,0.5,0.5,0.5},{0.5,0.5,0.5,0.5}};
//vector<Vista> vistas={{0.0,0.0,0.5,1,&proyeccion},{0.5,0.0,0.5,1,&proyeccion}};//,{0.0,0.5,0.5,0.5},{0.5,0.5,0.5,0.5}};
vector<CamaraTPS> camaras(vistas.size());

/* SENSOR */
class SensorWiiMote:public SensorObserver{
	void onSensorEvent(SensorEventData &e){
	    CamaraTPS &cam=camaras[0];
		Solido* mario = cam.getSolido();
		//cout << "wiimote->" << e << endl;
		//double x=stod(e["x"]);
		double y=e.getDouble("y");
		double z=e.getDouble("z");
		//double b=stod(e["buttons"]);
		y=linearMap(y,94,140,-2,2);
		z=linearMap(z,94,140,-0.01,0.01);
		mario->setRot(mario->getRot() + Vector3D(0, y, 0));
		vel+=z;
	};
};

void displayMe(void){
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	vistas[0].render();
	fondo.render();
    glLoadIdentity();
    camaras[0].render();
    e.render();

	vistas[1].render();
	fondoTablero.render();
    glLoadIdentity();
    camAR->render();
    e.render();

    glutSwapBuffers();
}
void upKart(){
	Vector3D up(0,1,0);
	circuit->setDrawNormals(false);
	vector<Triangle*> &triangulos=circuit->getTriangulos();
	Vector3D p=mariokart->getPos();
	double min=1e40;
	Triangle* nearest=nullptr;
	for(Triangle* &t:triangulos){
/*
		 double d=t->getCenter().distancia(p);
		 if(d<min){
			 min=d;
			 nearest=t;
		 }
		*/
		if(t->getNormal()*up>0.5){
			if(t->isOver(p)){
				double d=t->distancia(p);
				if(fabs(d)<3)
				if(d<min){
					min=d;
					nearest=t;
				}
			}
		}
	}
	if(nearest!=nullptr){
		nearest->setDrawNormals(true);
//		cout << nearest->isIn(p);

		/*
		//Show kart track
		Esfera* ef=new Esfera();
		ef->setCol(Vector3D(1,1,0));
		ef->setR(0.2);
		ef->hazFija();
		ef->setPos(p);
		e.add(ef);
		*/

		Vector3D f=nearest->getNormal();
		//adapt kart orientation to road
		double ry=deg2rad(nearest->getRot().getY());
		Vector3D vel={-sin(ry),0,cos(ry)};
		Vector3D vr=vel.X(f); vr.normalize();
		Vector3D vu=vr.X(vel);
		double cosv=f*vel;
		double cosr=f*vr;
		double dv=rad2deg(M_PI/2-acos(cosv));
		double dr=rad2deg(M_PI/2-acos(cosr));
		//cout << "dv="<<dv<<" cosv="<<cosv<< "dr="<<dr<<" cosr="<<cosr<<endl;
		mariokart->setRot(Vector3D(dv,mariokart->getRot().getY(),dr));
		double dist=nearest->distancia(p);
		//up fast down slow
		if(dist<0)
			dist*=0.95;
		else
			dist*=0.95;
//		cout<<"dist="<<dist<<" p="<<p<<endl;
		Vector3D newPos=p-f*dist;
		mariokart->setPos(newPos);
		//mariokart->aplicaFuerza(f*10);
		//mariokart->update(dt);
	}
}
void idle(){
 t+=dt;
 e.update(dt);
 upKart();
// Mat i;
// cap>>i;
// tex.setImage(i);
// texTv.setImage(i);
 //setPoseCamAR(i);
 //tex.update();
 CamaraTPS &cam=camaras[0];
 cam.update(dt*vel);
 displayMe();
 //scp.updateSensors();
}
void keyPressed(unsigned char key,int x,int y){
	Solido* s;
 switch(key){
 case '.':{
	 Vector3D p=mariokart->getPos();
	 cout << "kart pos="<<p<<endl;
	 break;
 }
 case 'o':
 case 'O':{
	    CamaraTPS &cam=camaras[0];
		s = cam.getSolido();
		s->setRot(s->getRot() + Vector3D(0, 2, 0));
		cam.update(dt);
		break;}
 case 'p':
 case 'P':{
	    CamaraTPS &cam=camaras[0];
		s = cam.getSolido();
		s->setRot(s->getRot() - Vector3D(0, 2, 0));
		cam.update(dt);
		break;}
 break;
 case 'q':
 case 'Q':
	 m->setPos(m->getPos()+Vector3D(0.25,0,0));
 break;
 case 'w':
 case 'W':
	 m->setPos(m->getPos()-Vector3D(0.25,0,0));
 break;
 case ' ':
	 Esfera *pf;
	 pf=new Esfera();
	 pf->setPos(m->getPos());
	 pf->setVel(Vector3D(0,0,-1.0));
	 pf->setCol(Vector3D(1,0.5,0));
	 pf->setR(0.04);
	 e.add(pf);
 break;
 case 't':
 case 'T':
 	for(unsigned int i=0;i<vistas.size();i++){
 		if(vistas[i].contain(x,y)){
 			Vector3D r;
 			CamaraTPS &cam=camaras[i];
 			cam.update(dt*2);
 		}
 	}
 	vel++;
 break;
 case 'g':
 case 'G':
	 	for(unsigned int i=0;i<vistas.size();i++){
	 		if(vistas[i].contain(x,y)){
	 			Vector3D r;
	 			CamaraTPS &cam=camaras[i];
	 			cam.update(-dt*2);
	 		}
	 	}
	 vel--;
 break;
 case 'a':
 case 'A':
	 //for(Solido *s:ce->getParticulas()){
	//	 s->setVel(s->getVel()+Vector3D(getRand(7,-7),10,-10));
	 for(Solido *s:cme->getParticulas()){
		 s->setVel(s->getVel()+Vector3D(getRand(1,-1),1,-1));
	 }
 break;
 case 27:
   exit(0);
 break;
 }
}
void mouseMoved(int x, int y)
{
    if (mx>=0 && my>=0) {
    	for(unsigned int i=0;i<vistas.size();i++){
    		if(vistas[i].contain(x,y)){
    			Vector3D r;
    			Camera &cam=camaras[i];
    			r=cam.getRot()+Vector3D(y-my,x-mx,0);
    			cam.setRot(r);
    		}
    	}
    }
    mx = x;
    my = y;
}

void mousePress(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        mx = x;
        my = y;
    }
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        mx = -1;
        my = -1;
    }
}
void loadCircuit(int i){
	 /*  C I R C U I T O S */
	 if(i==0){
		 //Initial pose of mariokart in this circuit
		 mariokart->setPos(Vector3D(4.71491,4,69.8178));
		 circuit=new ModeloMaterial("mariocircuit.obj");
		 circuit->hazFija();
		 circuit->setDrawNormals(true);
		 circuit->doScale(0.004);
		 e.add(circuit);
	 }
	 if(i==1){
		 //Initial pose of mariokart in this circuit
		 mariokart->setPos(Vector3D(46,10,-45));
		 mariokart->setRot(Vector3D(0,90,0));
		 circuit=new ModeloMaterial("mario_course.obj");
		 circuit->hazFija();
		 //circuit->setDrawNormals(true);
		 circuit->doScale(0.5);
		 e.add(circuit);
	 }
	 if(i==2){
		 //Initial pose of mariokart in this circuit
		 mariokart->setPos(Vector3D(0,6,0));
		 //mariokart->setRot(Vector3D(0,90,0));
		 circuit=new ModeloMaterial("course_model.obj");
		 circuit->hazFija();
		 //circuit->setDrawNormals(true);
		 //circuit->doScale(0.5);
		 e.add(circuit);
	 }
	 if(i==3){
		 //Initial pose of mariokart in this circuit
		 mariokart->setPos(Vector3D(66,12,38));
		 //mariokart->setRot(Vector3D(0,90,0));
		 circuit=new ModeloMaterial("Kinoko.obj");
		 circuit->hazFija();
		 //circuit->setDrawNormals(true);
		 circuit->doScale(0.5);
		 e.add(circuit);
	 }
	 if(i==4){
		 //Initial pose of mariokart in this circuit
		 mariokart->setPos(Vector3D(0,0,0));
		 //mariokart->setRot(Vector3D(0,90,0));
		 circuit=new ModeloMaterial("KoopaTroopaBeach.obj");
		 circuit->hazFija();
		 //circuit->setDrawNormals(true);
		 circuit->doScale(0.5);
		 circuit->doTranslate(Vector3D(20,0,-100));
		 e.add(circuit);
	 }
	 if(i==5){
		 //Initial pose of mariokart in this circuit
		 mariokart->setPos(Vector3D(0,0,0));
		 //mariokart->setRot(Vector3D(0,90,0));
		 circuit=new ModeloMaterial("AnimalCrossingSummer.obj");
		 circuit->hazFija();
		 //circuit->setDrawNormals(true);
		 circuit->doScale(0.5);
		 circuit->doTranslate(Vector3D(20,0,-100));
		 e.add(circuit);
	 }
}
int main(int argc, char** argv) try{
	srand(10);
	SensorWiiMote wiim;
	Game marioKart("PAGame Mario Kart :-D");
	marioKart.addScene(&view,&cam,&hpall);
	marioKart.addScene(&view1,&camk,&hpall);

	 vel=0;
	 //cout << t.isIn(Vector3D(0.25,0.25,0))<<endl;
	 //camaras[0].setLookSolido(false);
	 for(Camera &c:camaras){
		 c.setPos(Vector3D(0,2.50,10));
		 //c.setPos(Vector3D(0,1.50,0));
		 c.setRot(Vector3D(0,180,0));
	 }
	 Light* l1=new Light(Vector3D( 50,50,15));
	 l1->hazFija();
	 e.add(l1);
	 //e.add(new Luz(Vector3D(-50,50,15)));

	 /*  M E N U  */
	 int ci=4;
	 cout << "Please enter the circuit number from 0 to 5: ";
	 //cin >>ci;
	 ci=0;


	GameEngine::setGame(&marioKart);
	GameEngine::gameInit(argc,argv);

	 // In order to use textures fist gameInit() as to be called
	 /*  M A R I O   K A R T */
	 mariokart=new ModeloMaterial("pk_kart.obj");
	 mariokart->hazFija();
	 //mariokart->doScale(0.10);
	 e.add(mariokart);
	 camaras[0].setSolido(mariokart);



	 loadCircuit(ci);

	 initCamAR();
	 fondoTablero.setTextura(texTablero);



	//gameSetup();
	marioKart.addSensorObserver(&wiim);
	GameEngine::gameMainLoop();
	return 0;
}
catch (exception &e){
	cout << "Error in main():"<<e.what() <<endl;
}
