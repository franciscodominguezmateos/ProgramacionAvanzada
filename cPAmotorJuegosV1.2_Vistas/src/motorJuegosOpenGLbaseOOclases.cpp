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

using namespace cv;

double t=0.0;
double dt=1.0/30;

int mx=-1,my=-1;        // Previous mouse coordinates

Mat tablero;

Escena e;
Cubo *pc;
Modelo *m;
Textura tex,ladrillos,paredTex,texTv,texTablero;
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

ProyeccionPerspectiva proyeccion;
vector<Vista> vistas={{0.0,0.0,0.5,1,&proyeccion},{0.5,0.0,0.5,1,&pCam}};//,{0.0,0.5,0.5,0.5},{0.5,0.5,0.5,0.5}};
vector<CamaraFly> camaras(vistas.size());


float getRand(float max,float min=0){
	float n=max-min;
	int ir=rand()%1000;
	return min+n*(float)ir/1000;
}

void displayMe(void){
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	vistas[0].render();
	fondoTablero.render();
    glLoadIdentity();
    camaras[0].render();
    e.render();

	vistas[1].render();
	fondoTablero.render();
    glLoadIdentity();
    camAR->render();
    //camaras[1].render();
    e.render();

 tex.activar();
 glPushMatrix();
  glTranslatef(0,1,-2);
  glRotatef(t*10.0,0,1,0);
  glColor3f(1,1,1);
  glutSolidTeapot(1);
 glPopMatrix();
 tex.desactivar();

 glutSwapBuffers();
}

void idle(){
	t+=dt;
 e.update(dt);
 Mat i;
 cap>>i;
 tex.setImageFlipped(i);
 texTv.setImage(i);
 //tex.update();
 displayMe();
}
void keyPressed(unsigned char key,int x,int y){
 switch(key){
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
 			CamaraFly &cam=camaras[i];
 			cam.update(dt*2);
 		}
 	}
 break;
 case 'g':
 case 'G':
	 	for(unsigned int i=0;i<vistas.size();i++){
	 		if(vistas[i].contain(x,y)){
	 			Vector3D r;
	 			CamaraFly &cam=camaras[i];
	 			cam.update(-dt*2);
	 		}
	 	}
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
void init(void){
 glEnable(GL_DEPTH_TEST);
 glEnable(GL_LIGHTING);
 glEnable(GL_LIGHT0);
 glEnable(GL_LIGHT1);
 glEnable(GL_COLOR_MATERIAL);
 tex.init();
 ladrillos.init();
 paredTex.init();
 texTv.init();
 texTablero.init();
}
void reshape(int width,int height){
	for(Vista &v:vistas)
		v.reshape(width,height);
}
void initCamAR(){
	 tablero=imread("imgname.bmp");
	 Size patternsize(8,6); //interior number of corners
	 Mat gray;
	 cvtColor(tablero,gray, COLOR_BGR2GRAY);
	 vector<Point2f> corners; //this will be filled by the detected corners

	 //CALIB_CB_FAST_CHECK saves a lot of time on images
	 //that do not contain any chessboard corners
	 bool patternfound = findChessboardCorners(gray, patternsize, corners,
	         CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE
	         + CALIB_CB_FAST_CHECK);

	 if(patternfound)
	   cornerSubPix(gray, corners, Size(11, 11), Size(-1, -1),
	     TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));

	 drawChessboardCorners(tablero, patternsize, Mat(corners), patternfound);

	 vector<Point3d> model_points;
	 double squareSize=1.0;
	 for(double x=0;x<8*squareSize-0.1;x+=squareSize)
		 for(double y=0;y<6*squareSize-0.1;y+=squareSize)
			 model_points.push_back(Point3d(y,x,0));
	 //Solve for pose
	 Mat rvec,tvec;
	 solvePnP(model_points,corners,K,dist,rvec,tvec);
	 camAR=new CamaraAR(rvec,tvec);
	 texTablero.setImage(tablero);
	 //imshow("tablero",tablero);
	 //waitKey(1);
}
int main(int argc, char** argv){
 srand(10);
 for(Camera &c:camaras)
	 c.setPos(Vector3D(0,1.65,10));
 e.add(new Luz(Vector3D( 50,50,15)));
 e.add(new Luz(Vector3D(-50,50,15)));
 //m=new Modelo("/home/francisco/git/ProgramacionAvanzada/cPAmotorJuevosV0.9/minion01.obj");
 m=new Modelo("Debug/minion.obj");
 m->setPos(Vector3D(0,-0.5,1));
 m->setVel(Vector3D(getRand(10,-10),0,-1.1));
 e.add(m);


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
	 pa->setPos(Vector3D(getRand(40,-40),0,getRand(40,-40)));
	 e.add(pa);
 }
 for(int i=1;i<100;i++){
	 pf=new Esfera();
	 pf->setPos(Vector3D(getRand(50,-50),getRand(2),-getRand(50,-50)));
	 pf->setVel(Vector3D(getRand(1,-1),0,getRand(1,-1)));
	 pf->setCol(Vector3D(getRand(1),getRand(1),getRand(1)));
	 pf->setF(Vector3D(0,-9.8,0));
	 pf->setR(getRand(0.25)+0.1);
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
 ret->setNU(25);
 ret->setNV(25);
 //e.add(ret);
 p0=Vector3D(  0, 0,-10);
 p1=Vector3D( 20, 0,-10);
 p2=Vector3D( 20,10,-10);
 p3=Vector3D(  0,10,-10);
 Rectangulo *pared;
 pared=new Rectangulo(p0,p1,p2,p3);
 ret->setCol(Vector3D(1,1,1));
 paredTex.setImage(imread("brown_brick_texture_map.jpg"));
 pared->getTex()=paredTex;
 pared->setNU(5);
 pared->setNV(5);
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

 ce=new CuboElastico(1);
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


