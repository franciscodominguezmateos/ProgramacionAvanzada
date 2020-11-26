
//============================================================================
// Name        : CPAGameMarioKartVrPf project. MarioKartVrFp.cpp
// Author      : Francisco Dominguez
// Version     : 2019/12/31
// Copyright   :
// Description : Linus Trinus and WiiMote Virtual Relity Mario Kart game
//============================================================================
// In order to use GLSL
#define GL_GLEXT_PROTOTYPES
#include <GL/glut.h>
// In order to use GLSL
#include <GL/glext.h>
////////////////////////////
#include <stdlib.h>
#include <iostream>
#include <thread>
#include <mutex>
#include "opencv2/opencv.hpp"
#include "vector3d.h"
#include "esfera.h"
#include "cubo.h"
#include "cilindro.h"
#include "rosco.h"
#include <composite.h>
#include <stage.h>
#include "pared.h"
#include "camara.h"
#include "texture.h"
#include <rectangle.h>
#include "pendulo.h"
#include "cubo_elastico.h"
#include "fondo_textura.h"
#include <view.h>
#include "luz.h"
#include "proyeccion_perspectiva.h"
#include "pose_estimation_chessboard.h"
#include "material.h"
#include "modelo_material.h"
#include "caja_elastica.h"
#include "caja_modelo_elastico.h"

//Declaration needed in next .h
Mat global_img;
#include "socket_TCP_server.h"

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

ProyeccionPerspectiva proyeccion;
vector<View> vistas={{0.0,0.0,0.5,1,&proyeccion},{0.5,0.0,0.5,1,&proyeccion}};//,{0.0,0.5,0.5,0.5},{0.5,0.5,0.5,0.5}};

CamaraTPSVR camVR;
/* SENSOR LinusTrinus */
vector<CGI> sensorEventsLT;
mutex mtxSensorEventsLT;
class StringCGIProcessorLT:public StringProcessor{
public:
	string process(string &si){
		CGI event(si);
		//cout << si << endl;
		mtxSensorEventsLT.lock();
		sensorEventsLT.clear();
		sensorEventsLT.push_back(si);
		mtxSensorEventsLT.unlock();
	    return "OK";
	}
};
void updateSensorsLT(){
	if(!sensorEventsLT.empty()){
		vector<CGI>::iterator pos=sensorEventsLT.begin();
		mtxSensorEventsLT.lock();
		CGI e=sensorEventsLT.front();
		mtxSensorEventsLT.unlock();
		//sensorEvents.clear();
		//sensorEvents.erase(pos);
		if(e.size()==4){
			//cout << "e=" << e << endl;
			double x=stod(e["y"]);
			double y=stod(e["x"]);
			double z=stod(e["z"]);
			camVR.setRot(Vector3D(x, y, z));
		}
	}
}

/* SENSOR WiiMote*/
vector<CGI> sensorEventsWM;
mutex mtxSensorEventsWM;
class StringCGIProcessorWM:public StringProcessor{
public:
	string process(string &si){
		CGI event(si);
		//cout << si << endl;
		mtxSensorEventsWM.lock();
		sensorEventsWM.clear();
		sensorEventsWM.push_back(si);
		mtxSensorEventsWM.unlock();
	    return "OK";
	}
};
void updateSensorsWM(){
	if(!sensorEventsWM.empty()){
		vector<CGI>::iterator pos=sensorEventsWM.begin();
		mtxSensorEventsWM.lock();
		CGI e=sensorEventsWM.front();
		mtxSensorEventsWM.unlock();
		if(e.size()==4){
			Solido* mario = camVR.getSolido();
			cout << "e=" << e << endl;
			//double x=stod(e["x"]);
			double y=stod(e["y"]);
			double z=stod(e["z"]);
			//double b=stod(e["buttons"]);
			y=linearMap(y,94,140,-2,2);
			z=linearMap(z,94,140,-0.01,0.01);
			mario->setRot(mario->getRot() + Vector3D(0, y, 0));
			//vel+=z;
		}
	}
}

Mat opengl_default_frame_to_opencv() {
	//cv::Mat img(480, 640*2, CV_8UC3);
    cv::Mat img(1080, 1920, CV_8UC3);
    glPixelStorei(GL_PACK_ALIGNMENT, (img.step & 3)?1:4);
    glPixelStorei(GL_PACK_ROW_LENGTH, img.step/img.elemSize());
    glReadPixels(0, 0, img.cols, img.rows, GL_BGR_EXT, GL_UNSIGNED_BYTE, img.data);
    cv::Mat flipped(img);
    cv::flip(img, flipped, 0);
    return img;
}
SocketMJPEGServer sms;
void displayMe(void){
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	vistas[0].render();
    glLoadIdentity();
    camVR.renderLeft();
    e.render();

	vistas[1].render();
    glLoadIdentity();
    camVR.renderRight();
    e.render();

    glutSwapBuffers();
    global_img=opengl_default_frame_to_opencv();
    sms.setImg(global_img);
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
 camVR.update(dt*vel);
 displayMe();
 updateSensorsLT();
 updateSensorsWM();
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
		s = camVR.getSolido();
		s->setRot(s->getRot() + Vector3D(0, 2, 0));
		break;}
 case 'p':
 case 'P':{
		s = camVR.getSolido();
		s->setRot(s->getRot() - Vector3D(0, 2, 0));
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
 case 'x':
 case 'X':
	 s = camVR.getSolido();
	 cout << "Caracter pos="<< s->getPos()<<" R=" <<s->getRot() << endl;
 break;
 case 'v':
 case 'V':
 	 camVR.setBaseline(camVR.getBaseline()-0.05);
	 cout << "Baseline="<< camVR.getBaseline()<<endl;
 break;
 case 'B':
 case 'b':
 	 camVR.setBaseline(camVR.getBaseline()+0.05);
	 cout << "Baseline="<< camVR.getBaseline()<<endl;
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
 	vel++;
 	break;
 case 'g':
 case 'G':
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
    			r=camVR.getRot()+Vector3D(y-my,x-mx,0);
    			camVR.setRot(r);
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

void reshape(int width,int height){
	for(View &v:vistas)
		v.reshape(width,height);
}

void loadCircuit(int i){
	//must finish on slash '/'
	ModeloMaterial::model_base_path="modelos/courses/";
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
		 mariokart->setPos(Vector3D(-27.8264,1.50484,-119.87));
		 //mariokart->setRot(Vector3D(0,90,0));
		 circuit=new ModeloMaterial("SherbetLand.obj");
		 circuit->hazFija();
		 //circuit->setDrawNormals(true);
		 circuit->doScale(0.5);
		 circuit->doTranslate(Vector3D(20,0,-100));
		 e.add(circuit);
	 }
	 if(i==6){
		 //Initial pose of mariokart in this circuit
		 mariokart->setPos(Vector3D(-29.4694,40,-74.9258));
		 //mariokart->setRot(Vector3D(0,90,0));
		 circuit=new ModeloMaterial("old_sky_agb.obj");
		 circuit->hazFija();
		 circuit->setDrawNormals(true);
		 //circuit->doScale(1.0);
		 //circuit->doTranslate(Vector3D(0,0,0));
		 e.add(circuit);
	 }
	 if(i==7){
		 //Initial pose of mariokart in this circuit
		 mariokart->setPos(Vector3D(0,0,0));
		 //mariokart->setRot(Vector3D(0,90,0));
		 circuit=new ModeloMaterial("test_WiFiTest1.obj");
		 circuit->hazFija();
		 circuit->setDrawNormals(true);
		 circuit->doScale(0.01);
		 //circuit->doTranslate(Vector3D(0,0,0));
		 e.add(circuit);
	 }
	 ModeloMaterial::model_base_path="modelos/";
}
void init(void){
 glEnable(GL_DEPTH_TEST);
 glEnable(GL_LIGHTING);
 glEnable(GL_LIGHT0);
 glEnable(GL_LIGHT1);
 glEnable(GL_COLOR_MATERIAL);
 //glShadeModel(GL_FLAT);
 glShadeModel(GL_SMOOTH);
 glEnable(GL_BLEND);
 glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
void gameInit(int argc, char** argv){
	//string glv((char*)glGetString(GL_VERSION));
	//cout << "OpenGL:" <<  glv <<endl;
	 glutInit(&argc,argv);
	 //glutInitDisplayMode(GLUT_SINGLE);
	 glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	 glutInitWindowSize(640*2,480);
	 glutInitWindowPosition(300,300);
	 glutCreateWindow("Mario Kart Virtual Reality First Person :D");
	 init();
}
void gameMainLoop(){
	 glutDisplayFunc(displayMe);
	 glutIdleFunc(idle);
	 glutReshapeFunc(reshape);
	 glutKeyboardFunc(keyPressed);
	 glutMotionFunc(&mouseMoved);
	 glutMouseFunc(&mousePress);
	 glutFullScreen();
	 glutMainLoop();
}

int main(int argc, char** argv){
 srand(10);
 // Launch input sensor server thread
 bool stop=false;
 // THREADs
 // JPEG stream server
 //thread server_th(video_jpeg_stream_server,&stop,4097);
 //this_thread::sleep_for(chrono::milliseconds(100));
 //sms=new SocketMJPEGServer();
 //sms->setImg(Mat::eye(320,240,CV_32F));
 // Linus Trinus head tracking sensor server
 StringCGIProcessorLT scp;
 SocketTCPStringServer string_svr(&scp,8882);
 /*
 thread string_th_LT(string_server,&stop,&scp,8881);
 this_thread::sleep_for(chrono::milliseconds(100));
 */
 // WiiMotion wheel sensor server
 StringCGIProcessorWM scpWM;
 SocketTCPStringServer string_wm_svr(&scpWM,8881);
 /*
 thread string_th_WM(string_server,&stop,&scpWM,8882);
 // wait a minute
 this_thread::sleep_for(chrono::milliseconds(100));
*/
 vel=0;
 //cout << t.isIn(Vector3D(0.25,0.25,0))<<endl;
 camVR.setLookSolido(false);
 camVR.setPos(Vector3D(0,1.50,0));
 //camVR.setRot(Vector3D(0,180,0));

 Luz* l1=new Luz(Vector3D( 50,50,15));
 l1->hazFija();
 e.add(l1);
 //e.add(new Luz(Vector3D(-50,50,15)));

 /*  M E N U  */
 int ci=0;
 cout << "Please enter the circuit number from 0 to 6: ";
 //cin >>ci;
 ci=0;
 gameInit(argc,argv);

 // In order to use textures fist gameInit() as to be called
 /*  M A R I O   K A R T */
 mariokart=new ModeloMaterial("mariokart.obj");
 mariokart->hazFija();
 mariokart->doScale(0.10);
 e.add(mariokart);
 camVR.setSolido(mariokart);

 loadCircuit(ci);

 gameMainLoop();
 return 0;
}


