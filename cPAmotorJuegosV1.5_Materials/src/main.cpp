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
#include "material.h"
#include "modelo_material.h"
#include "caja_elastica.h"
#include "caja_modelo_elastico.h"

using namespace cv;

double t=0.0;
double dt=1.0/30;

int mx=-1,my=-1;        // Previous mouse coordinates

Mat tablero;

double vel;


ModeloMaterial* circuit;
ModeloMaterial* mariokart;

Escena e;
Cubo *pc;
ModeloMaterial* m;
Textura tex,ladrillos,paredTex,texTv,texTablero,spiderTex,marioKartTex,minionTex,mariokartTex;
VideoCapture cap(0);
CuboElastico *ce;
CajaElastica* cje;
CajaModeloElastico* cme;

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
void upKart(){
	Vector3D up(0,1,0);
	circuit->setDrawNormals(false);
	vector<Triangulo*> &triangulos=circuit->getTriangulos();
	Vector3D p=mariokart->getPos();
	double min=1e40;
	Triangulo* nearest=nullptr;
	for(Triangulo* &t:triangulos){
/*
		 double d=t->getCenter().distancia(p);
		 if(d<min){
			 min=d;
			 nearest=t;
		 }
		*/
		if(t->getNormal()*up>0.5){
			if(t->isIn(p)){
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
		cout << "dv="<<dv<<" cosv="<<cosv<< "dr="<<dr<<" cosr="<<cosr<<endl;
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
 Mat i;
 cap>>i;
 tex.setImage(i);
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
		 e.add(circuit);
	 }
}
int main(int argc, char** argv){
 srand(10);
 vel=0;
 //cout << t.isIn(Vector3D(0.25,0.25,0))<<endl;
 for(Camara &c:camaras){
	 c.setPos(Vector3D(0,1.65,10));
	 c.setRot(Vector3D(0,180,0));
 }
 Luz* l1=new Luz(Vector3D( 50,50,15));
 l1->hazFija();
 e.add(l1);
 //e.add(new Luz(Vector3D(-50,50,15)));

 /*  M E N U  */
 int ci=4;
 //cout << "Please enter the circuit number from 0 to 4: ";
 //cin >>ci;

 glutInit(&argc,argv);
 //glutInitDisplayMode(GLUT_SINGLE);
 glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
 glutInitWindowSize(640*2,480);
 glutInitWindowPosition(300,300);
 glutCreateWindow("Hello wold :D");
 init();


 // In order to use textures fist init() as to be called
 /*  M A R I O   K A R T */
 mariokart=new ModeloMaterial("mk_kart.obj");
 mariokart->hazFija();
 //mariokart->doScale(10);
 e.add(mariokart);
 camaras[0].setSolido(mariokart);

 loadCircuit(ci);

 m=new ModeloMaterial("TheAmazingSpiderman.obj");
 //m->setScale(Vector3D(4,4,4));
 //m->setPos(Vector3D(0,-0.5,1));
 //m->setVel(Vector3D(getRand(10,-10),0,-1.1));
 //e.add(m);

 ModeloMaterial* mm=new ModeloMaterial("M-FF_iOS_HERO_Natasha_Romanoff_Black_Widow_Age_Of_Ultron.obj");
 mm->setPos(Vector3D(4,0,0));
 mm->setScale(Vector3D(4,4,4));
 mm->setVel(Vector3D(getRand(10,-10),0,getRand(10,-10)));
 e.add(mm);

 ModeloMaterial* felicia=new ModeloMaterial("Felicia.obj");
 felicia->setPos(Vector3D(4,0,0));
 felicia->setScale(0.5);
 felicia->setVel(Vector3D(getRand(10,-10),0,getRand(10,-10)));
 e.add(felicia);

 ModeloMaterial* shrek=new ModeloMaterial("shrek.obj");
 shrek->setPos(Vector3D(-8,0,0));
 shrek->doScale(0.25);
 shrek->setVel(Vector3D(getRand(10,-10),0,getRand(10,-10)));
 //e.add(shrek);

 ModeloMaterial* minion_golf=new ModeloMaterial("mc_golf.obj");
 minion_golf->setPos(Vector3D(-4,0,0));
 minion_golf->setScale(4);
 minion_golf->setVel(Vector3D(getRand(10,-10),10,getRand(10,-10)));
 //e.add(minion_golf);

 m->doCenter();
 cme=new CajaModeloElastico(m);
 e.add(cme);

 Vector3D p0(-80,0,-80);
 Vector3D p1(-80,0, 80);
 Vector3D p2( 80,0, 80);
 Vector3D p3( 80,0,-80);
 Rectangulo *ret;
 ret=new Rectangulo(p0,p1,p2,p3);
 ret->setCol(Vector3D(1,1,1));
 ladrillos.setImage(imread("mario_kart_circuit.jpg"));
 ret->getTex()=ladrillos;
 ret->setNU(1);
 ret->setNV(1);
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
 tex.setImage(i);
 tv->getTex()=tex;
 e.add(tv);

 cje=new CajaElastica(2,4,2);
 //e.add(cje);

 ce=new CuboElastico(2);
 ce->setTexture(tex);
 //e.add(ce);

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


