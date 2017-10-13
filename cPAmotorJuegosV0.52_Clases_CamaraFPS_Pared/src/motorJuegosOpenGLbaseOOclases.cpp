#include <GL/glut.h>
#include <iostream>
#include "vector3d.h"
#include "esfera.h"
#include "cubo.h"
#include "pared.h"
#include "escena.h"
#include "camara.h"

double t=0.0;
double dt=1.0/30;
GLfloat pitch=0.0f;

int mx=-1,my=-1;        // Previous mouse coordinates
//int rotangles[2] = {0}; // Panning angles

Escena e;
CamaraFPS cam;

void displayMe(void){
 glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
 glLoadIdentity();
 cam.render();
 //glRotatef(t/2,0.0,1.0,0.0);
 GLfloat lightpos[]={5.0,15.0,5.0,0.0};
 glLightfv(GL_LIGHT0,GL_POSITION,lightpos);
 e.render();
 glColor3f(1.0,1.0,1);
 //renderCubo(cubos[0]);
 //renderEsfera(esferas[0]);
 glPushMatrix();
  glColor3f(1.0f,1.0f,0.0f);
  glRotatef(t,0.0,1.0,0.0);
  glutSolidCube(0.2);
  glPushMatrix();
   glColor3f(1.0f,1.0f,1.0f);
   glRotatef(t*2,0.0,1.0,0.0);
   glTranslatef(0.6,0,0);
   glutSolidCube(0.1);
    glPushMatrix();
     glColor3f(0.0f,1.0f,1.0f);
     glRotatef(t*4,0.0,1.0,0.0);
     glTranslatef(0.2,0,0);
     glutSolidCube(0.05);
   glPopMatrix();
  glPopMatrix();
 glPopMatrix();
 glColor3f(1.0f,0.0f,0.0f);
 glBegin(GL_POLYGON);
   glVertex3f(0.0,0.0,0.0);
   glVertex3f(0.5,0.0,0.0);
   glVertex3f(0.5,0.5,0.0);
   glVertex3f(0.0,0.5,0.0);
 glEnd();
 glColor3f(1.0f,1.0f,1.0f);
 glBegin(GL_POLYGON);
   glVertex3f(-50.0,0.0,-50.0);
   glVertex3f( 50,0.0  ,-50.0);
   glVertex3f( 50  ,0.0, 50.0);
   glVertex3f(-50.0,0.0, 50.0);
 glEnd();
 glFlush();
 glutSwapBuffers();
 //t++;
}

void idle(){
 t+=dt;
 e.update(dt);
 displayMe();
}
void keyPressed(unsigned char key,int x,int y){
 switch(key){
  case 'p':
  case 'P':
    cam.setRot(cam.getRot()+Vector3D(0,1,0));
  break;
  case 'o':
  case 'O':
	cam.setRot(cam.getRot()-Vector3D(0,1,0));
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
}
void reshape(int width,int height){
 glViewport(0,0,width,height);
 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 gluPerspective(60.0f,(GLfloat)width/(GLfloat)height,0.1f,200.0f);
 glMatrixMode(GL_MODELVIEW);
}
int main(int argc, char** argv){
	 cam.setPos(Vector3D(0,1.65,10));
	 Cubo *c;
	 Esfera *f;
	 Pared *p;
	 Vector3D v;
	 c=new Cubo();
	 f=new Esfera();
	 c->setPos(Vector3D(0.6,1,0));
	 c->setVel(Vector3D(0.7,0,-0.71));
	 c->setCol(Vector3D(1,0,0));
	 c->setF(Vector3D(0,-0.98,0));
	 c->setM(1);
	 c->setS(0.4);
	 e.add(c);
	 f->setPos(Vector3D(-0.4,1.2,0));
	 f->setVel(Vector3D(-0.3,0,0.7));
	 f->setCol(Vector3D(1,0,1));
	 f->setF(Vector3D(0,-0.98,0));
	 f->setM(1);
	 f->setR(0.4);
	 e.add(f);
	 p=new Pared(10);
	 p->setPos(Vector3D(0,0,-2));
	 e.add(p);
	 p=new Pared(10);
	 p->setPos(Vector3D(0,0, 2));
	 e.add(p);
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

	 for(double i=-3;i<4;i++){
		 for(double j=0-3;j<4;j++){
			 f=new Esfera();
			 f->setPos(Vector3D(i/2,i*j/4,j/2));
			 f->setVel(Vector3D(i*j/5,0,0.7));
			 f->setCol(Vector3D(1,0,1));
			 f->setF(Vector3D(0,-0.98,0));
			 f->setM(1);
			 f->setR(0.1);
			 e.add(f);
		 }
	 }
	 for(double i=-30;i<31;i+=4){
		 for(double j=-30;j<31;j+=4){
			 f=new Esfera();
			 f->setPos(Vector3D(i,0,j));
			 f->setCol(Vector3D(1,1,0));
			 f->setM(1);
			 f->setR(0.5);
			 e.add(f);
		 }
	 }

 glutInit(&argc,argv);
 //glutInitDisplayMode(GLUT_SINGLE);
 glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
 glutInitWindowSize(300,300);
 glutInitWindowPosition(100,100);
 glutCreateWindow("Hello wold :D");
 init();
 glutDisplayFunc(displayMe);
 glutIdleFunc(idle);
 glutReshapeFunc(reshape);
 glutKeyboardFunc(keyPressed);
 glutMotionFunc(&mouseMoved);
 glutMouseFunc(&mousePress);
 glutMainLoop();
 return 0;
}


