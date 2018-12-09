#include <GL/glut.h>
#include <stdlib.h>
#include <iostream>
#include "opencv2/opencv.hpp"
#include "vector3d.h"
#include "esfera.h"
#include "cubo.h"
#include "cilindro.h"
#include "rosco.h"
#include "compuesto.h"
#include "modelo.h"
#include "escena.h"
#include "pared.h"
#include "camara.h"
#include "textura.h"
#include "rectangulo.h"
#include "pendulo.h"
#include "cubo_elastico.h"
#include "fondo_textura.h"
#include "vista.h"
#include "luz.h"
#include "proyeccion_perspectiva.h"
#include "pose_estimation_chessboard.h"

using namespace cv;

double t=0.0;
double dt=1.0/30;

int mx=-1,my=-1;        // Previous mouse coordinates

Mat tablero;

double vel;

Escena e;
Cubo *pc;
Modelo *m;
Textura tex,ladrillos,paredTex,texTv,texTablero,spiderTex,marioKartTex,minionTex,mariokartTex;
VideoCapture cap(0);
CuboElastico *ce;

FondoTextura fondo,fondoTablero;

Mat K=(Mat_<double>(3,3) <<
		 1053.755323784897,                 0, 317.160420774003,
		                 0, 1067.981896308737, 203.4946241428054,
						 0,                 0,                 1);

Mat dist=Mat::zeros(4,1,cv::DataType<double>::type); // Assuming no lens distortion
CamaraAR *camAR;
ProyeccionCamara pCam(K);
PoseEstimationChessBoard peChessBoard(K,dist);

ProyeccionPerspectiva proyeccion;
vector<Vista> vistas={{0.0,0.0,0.5,1,&proyeccion},{0.5,0.0,0.5,1,&pCam}};//,{0.0,0.5,0.5,0.5},{0.5,0.5,0.5,0.5}};
vector<CamaraTPS> camaras(vistas.size());

float getRand(float max,float min=0){
	float n=max-min;
	int ir=rand()%1000;
	return min+n*(float)ir/1000;
}

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
    //camaras[1].render();
    e.render();
/*
 tex.activar();
 glPushMatrix();
  glTranslatef(0,1,-2);
  glRotatef(t*10.0,0,1,0);
  glColor3f(1,1,1);
  glutSolidTeapot(5);
 glPopMatrix();
 tex.desactivar();
*/
 glutSwapBuffers();
}
void setPoseCamAR(Mat &tablero){
	 if(peChessBoard.estimatePose(tablero)){
		 camAR->setPose(peChessBoard.getRvec(),peChessBoard.getTvec());
	 }
	 else{
		 cout << "Tablero no detectado!!!"<< endl;
		 camAR->setPose(Mat::zeros(3,1,cv::DataType<double>::type),
				        Mat::zeros(3,1,cv::DataType<double>::type));
	 }
	 texTablero.setImage(tablero);
}

void idle(){
	t+=dt;
 e.update(dt);
 Mat i;
 cap>>i;
 tex.setImageFlipped(i);
 texTv.setImage(i);
 //setPoseCamAR(i);
 //tex.update();
 CamaraTPS &cam=camaras[0];
 cam.update(dt*vel);
 displayMe();
}
void keyPressed(unsigned char key,int x,int y){
	Solido* s;
 switch(key){
 case 'o':
 case 'O':{
	    CamaraTPS &cam=camaras[0];
		s = cam.getSolido();
		s->setRot(s->getRot() + Vector3D(0, 2, 0));
		cout << "s->getRot()=" << s->getRot() << endl;
		cam.update(dt);
		break;}
 case 'p':
 case 'P':{
	    CamaraTPS &cam=camaras[0];
		s = cam.getSolido();
		s->setRot(s->getRot() - Vector3D(0, 2, 0));
		cout << "s->getRot()=" << s->getRot() << endl;
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
	 for(Solido *s:ce->getParticulas()){
		 s->setVel(s->getVel()+Vector3D(getRand(7,-7),10,-10));
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
    			Camara &cam=camaras[i];
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
void init(void){
 glEnable(GL_DEPTH_TEST);
 glEnable(GL_LIGHTING);
 glEnable(GL_LIGHT0);
 glEnable(GL_LIGHT1);
 glEnable(GL_COLOR_MATERIAL);
 //glShadeModel(GL_FLAT);
 //glShadeModel(GL_SMOOTH);
 tex.init();
 ladrillos.init();
 paredTex.init();
 texTv.init();
 texTablero.init();
 spiderTex.init();
 spiderTex.setImage(imread("TheAmazingSpiderman1Tex.png"));
 marioKartTex.init();
 marioKartTex.setImage(imread("E_main.png"));
 minionTex.init();
 minionTex.setImage(imread("mc_golf_df.png"));
 mariokartTex.init();
 mariokartTex.setImage(imread("tex_0301.png"));
}
void reshape(int width,int height){
	for(Vista &v:vistas)
		v.reshape(width,height);
}
void initCamAR(){
	 tablero=imread("2017-11-03-091218.jpg");
	 if(peChessBoard.estimatePose(tablero)){
		 camAR=new CamaraAR(peChessBoard.getRvec(),peChessBoard.getTvec());
	 }
	 else{
		 cout << "Tablero no detectado!!!"<< endl;
		 camAR= new CamaraAR(Mat::zeros(3,1,cv::DataType<double>::type),
				             Mat::zeros(3,1,cv::DataType<double>::type));
	 }
	 texTablero.setImage(tablero);
}

int main(int argc, char** argv){
 srand(10);
 vel=0;
 for(Camara &c:camaras)
	 c.setPos(Vector3D(0,1.65,10));
 e.add(new Luz(Vector3D( 50,50,15)));
 e.add(new Luz(Vector3D(-50,50,15)));
 //m=new Modelo("/home/francisco/git/ProgramacionAvanzada/cPAmotorJuevosV0.9/minion01.obj");
 m=new Modelo("TheAmazingSpiderman.obj");
 m->setTexture(&spiderTex);
 //m->setPos(Vector3D(0,-0.5,1));
 //m->setVel(Vector3D(getRand(10,-10),0,-1.1));
 e.add(m);

 //Modelo* marioKart=new Modelo("mk_kart.obj");
 //marioKart->setTexture(&marioKartTex);
 //e.add(marioKart);

 Modelo* minion=new Modelo("mc_golf.obj");
 minion->setTexture(&minionTex);
 minion->setScale(Vector3D(4,4,4));
 minion->setVel(Vector3D(getRand(10,-10),0,getRand(10,-10)));
 e.add(minion);

 Modelo* mariokart=new Modelo("mariokart.obj");
 mariokart->setTexture(&mariokartTex);
 mariokart->setScale(Vector3D(0.25,0.25,0.25));
 e.add(mariokart);
 camaras[0].setSolido(mariokart);

 Solido *pt1=new Solido();
 Solido *pt2=new Solido();
 pt1->setPos(Vector3D(0,3.5,0));
 pt2->setPos(Vector3D(1,3,0));
 pt2->setF(Vector3D(0,-0.98,0));
 Pendulo *pdl=new Pendulo(pt1,pt2);
 e.add(pdl);
 Pared *p;
 p=new Pared(10);
 p->setPos(Vector3D(0,0,-2));
 //e.add(p);
 p=new Pared(10);
 p->setPos(Vector3D(0,0, 2));
 //e.add(p);

 p=new Pared(100);
 p->setPos(Vector3D(0,0,-50));
 e.add(p);
 p=new Pared(100);
 p->setPos(Vector3D(0,0, 50));
 e.add(p);
 p=new Pared(100);
 p->setPos(Vector3D(-50,0,0));
 p->setVertical(true);
 e.add(p);
 p=new Pared(100);
 p->setPos(Vector3D(50,0,0));
 p->setVertical(true);
 e.add(p);

 Vector3D v;
 pc=new Cubo();
 pc->setPos(Vector3D(0.6,0,0));
 pc->setVel(Vector3D(-0.01,0,0));
 pc->setCol(Vector3D(1,0,0));
 pc->setS(0.4);
 //e.add(pc);
 cout <<"Cubo="<<*pc<<endl;

 Esfera *pf;
 pf=new Esfera();
 pf->setPos(Vector3D(-0.4,0,0));
 pf->setVel(Vector3D(-0.03,0,0.08));
 pf->setCol(Vector3D(1,0,1));
 pf->setR(0.4);
 //e.add(pf);

 Cilindro *pl;
 pl=new Cilindro();
 pl->setPos(Vector3D(0,0,-1));
 //e.add(pl);

 Rosco *pr;
 pr=new Rosco();
 pr->setPos(Vector3D(0,0,0.8));
 //e.add(pr);
 //cout <<"Rosco="<<*pr<<endl;

 cout << e <<endl;

 Escena e0(e);

 Compuesto *arbol;
 arbol=new Compuesto();
 pf=new Esfera();
 pf->setR(3);
 pf->setPos(Vector3D(0,7.5,0));
 pf->setCol(Vector3D(0,1,0));
 arbol->add(pf);
 pl=new Cilindro(0.6,5);
 pl->setPos(Vector3D(0,5,0));
 pl->setCol(Vector3D(0.5,0.1,0));
 arbol->add(pl);
 Compuesto cp1=*arbol;
 //e.add(arbol);
 Compuesto *pa;
 for(int i=1;i<10;i++){
	 pa=new Compuesto(*arbol);
	 pa->setPos(Vector3D(getRand(50,-50),0,getRand(50,-50)));
	 e.add(pa);
 }
 for(int i=1;i<100;i++){
	 pf=new Esfera();
	 pf->setPos(Vector3D(getRand(50,-50),getRand(2),-getRand(50,-50)));
	 pf->setVel(Vector3D(getRand(1,-1),0,getRand(1,-1)));
	 pf->setCol(Vector3D(getRand(1),getRand(1),getRand(1)));
	 pf->setF(Vector3D(0,-9.8,0));
	 pf->setR(getRand(3,0.25));
	 e.add(pf);
 }
 cout<< pf->getPos() << endl;
 cout<<"Escena guardada"<<endl;
 cout <<e0<<endl;

 glutInit(&argc,argv);
 //glutInitDisplayMode(GLUT_SINGLE);
 glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
 glutInitWindowSize(640*2,480);
 glutInitWindowPosition(300,300);
 glutCreateWindow("Hello wold :D");
 init();

 Vector3D p0(-50,0,-50);
 Vector3D p1( 50,0,-50);
 Vector3D p2( 50,0, 50);
 Vector3D p3(-50,0, 50);
 Rectangulo *ret;
 ret=new Rectangulo(p0,p1,p2,p3);
 ret->setCol(Vector3D(1,1,1));
 ladrillos.setImage(imread("brick_pavement_0077_01_preview.jpg"));
 ret->getTex()=ladrillos;
 ret->setNU(2);
 ret->setNV(2);
 e.add(ret);
 p0=Vector3D(  0, 0,-10);
 p1=Vector3D( 20, 0,-10);
 p2=Vector3D( 20,10,-10);
 p3=Vector3D(  0,10,-10);
 Rectangulo *pared;
 pared=new Rectangulo(p0,p1,p2,p3);
 ret->setCol(Vector3D(1,1,1));
 paredTex.setImage(imread("brown_brick_texture_map.jpg"));
 pared->getTex()=paredTex;
 pared->setNU(1);
 pared->setNV(1);
 e.add(pared);

 p0=Vector3D(  9, 2,-9.9);
 p1=Vector3D( 11, 2,-9.9);
 p2=Vector3D( 11, 3,-9.9);
 p3=Vector3D(  9, 3,-9.9);
 Rectangulo *tv;
 tv=new Rectangulo(p0,p1,p2,p3);
 tv->setCol(Vector3D(1,1,1));
 Mat i;
 cap>>i;
 texTv.setImage(i);
 tv->getTex()=texTv;
 e.add(tv);

 ce=new CuboElastico(2);
 ce->setTexture(texTv);
 e.add(ce);

 initCamAR();
 fondo.setTextura(texTv);
 fondoTablero.setTextura(texTablero);

 glutDisplayFunc(displayMe);
 glutIdleFunc(idle);
 glutReshapeFunc(reshape);
 glutKeyboardFunc(keyPressed);
 glutMotionFunc(&mouseMoved);
 glutMouseFunc(&mousePress);
 glutMainLoop();
 return 0;
}


