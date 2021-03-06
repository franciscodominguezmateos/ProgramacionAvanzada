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

using namespace cv;

double t=0.0;
double dt=1.0/10;
GLfloat pitch=0.0f; 

int mx=-1,my=-1;        // Previous mouse coordinates

Escena e;
CamaraFPS cam;
Cubo *pc;
Modelo *m;
Textura tex,ladrillos,paredTex,texTv;
VideoCapture cap(0);

void displayMe(void){
 glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
 glLoadIdentity();
 cam.render();
 GLfloat lightpos[]={50.0,50.0,15.0,0.0};
 glLightfv(GL_LIGHT0,GL_POSITION,lightpos);
 e.render();
 tex.activar();
 glPushMatrix();
  glTranslatef(0,1,-2);
  glRotatef(t*10.0,0,1,0);
  glColor3f(1,1,1);
  glutSolidTeapot(1);
 glPopMatrix();
 tex.desactivar();
 /*11
 glRotatef(t*200,0,1,0);
 tex.activar();
 float w=640/128,h=480/128;
 glColor3f(1,1,1);
 glBegin(GL_QUADS);
	glTexCoord2f(1, 1);
	glVertex2f(-w/2,  h/2);
	glTexCoord2f(0, 1);
	glVertex2f( w/2,  h/2);
	glTexCoord2f(0, 0);
	glVertex2f( w/2, -h/2);
	glTexCoord2f(1, 0);
	glVertex2f(-w/2, -h/2);
 glEnd();
 tex.desactivar();
 */
 glutSwapBuffers();
}

void idle(){
	t+=dt;
 e.update(dt);
 Mat i;
 cap>>i;
 tex.setImageFlipped(i);
 texTv.setImage(i);
 //imshow("ladri",ladrillos.getImage());
 //waitKey(1);
 //tex.update();
 displayMe();
}
void keyPressed(unsigned char key,int x,int y){
 switch(key){
 case 'p':
 case 'P':
 pitch++;
 break;
 case 'o':
 case 'O':
 pitch--;
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
	 cam.update(dt*2);
 break;
 case 'g':
 case 'G':
    cam.update(-dt*2);
 break;
  case 27:
   exit(0);
   break;
 }
}
void mouseMoved(int x, int y)
{
    if (mx>=0 && my>=0) {
    	Vector3D r;
    	r=cam.getRot()+Vector3D(y-my,x-mx,0);
    	cam.setRot(r);
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
 glEnable(GL_COLOR_MATERIAL);
 glClearColor(0.0,0.0,0.0,0.0);
 tex.init();
 ladrillos.init();
 paredTex.init();
 texTv.init();
}
void reshape(int width,int height){
 glViewport(0,0,width,height);
 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,1.0f,200.0f);
 glMatrixMode(GL_MODELVIEW);
}
float getRand(float max,float min=0){
	float n=max-min;
	int ir=rand()%1000;
	return min+n*(float)ir/1000;
}
int main(int argc, char** argv){
 srand(10);
 cam.setPos(Vector3D(0,1.65,10));
 m=new Modelo("/home/francisco/git/ProgramacionAvanzada/cPAmotorJuevosV0.9/minion01.obj");
 m->setPos(Vector3D(0,-0.5,1));
 //m->setVel(Vector3D(0,0,-0.1));
 //e.add(m);

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
	 pf->setVel(Vector3D(getRand(3,-3),0,getRand(3,-3)));
	 pf->setCol(Vector3D(getRand(1),getRand(1),getRand(1)));
	 pf->setF(Vector3D(0,-9.8,0));
	 pf->setR(getRand(0.25)+0.25);
	 e.add(pf);
 }
 cout<< pf->getPos() << endl;
 cout<<"Escena guardada"<<endl;
 cout <<e0<<endl;

 glutInit(&argc,argv);
 //glutInitDisplayMode(GLUT_SINGLE);
 glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
 glutInitWindowSize(640,480);
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
 ladrillos.setImage(imread("/home/francisco/Downloads/brick_pavement_0077_01_preview.jpg"));
 ret->getTex()=ladrillos;
 ret->setNU(50);
 ret->setNV(50);
 e.add(ret);
 p0=Vector3D(  0, 0,-10);
 p1=Vector3D( 20, 0,-10);
 p2=Vector3D( 20,10,-10);
 p3=Vector3D(  0,10,-10);
 Rectangulo *pared;
 pared=new Rectangulo(p0,p1,p2,p3);
 ret->setCol(Vector3D(1,1,1));
 paredTex.setImage(imread("/home/francisco/Downloads/brown_brick_texture_map.jpg"));
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

 glutDisplayFunc(displayMe);
 glutIdleFunc(idle);
 glutReshapeFunc(reshape);
 glutKeyboardFunc(keyPressed);
 glutMotionFunc(&mouseMoved);
 glutMouseFunc(&mousePress);
 glutMainLoop();
 return 0;
}


