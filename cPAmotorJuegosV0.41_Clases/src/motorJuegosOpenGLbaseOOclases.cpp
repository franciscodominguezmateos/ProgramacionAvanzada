#include <GL/glut.h>
#include <iostream>
#include "vector3d.h"
#include "esfera.h"
#include "cubo.h"
#include "escena.h"

double t=0.0;
double dt=1.0/30;
GLfloat pitch=0.0f; 

int mx=-1,my=-1;        // Previous mouse coordinates
int rotangles[2] = {0}; // Panning angles


Escena e;

void displayMe(void){
 glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
 glLoadIdentity();
 glTranslatef(0.0f,0.0f,-3.0f);
 glRotatef(pitch,1.0,0.0,0.0);
 glRotatef(rotangles[0], 1,0,0);
 glRotatef(rotangles[1], 0,1,0);
 //glRotatef(t/2,0.0,1.0,0.0);
 GLfloat lightpos[]={5.0,15.0,5.0,0.0};
 glLightfv(GL_LIGHT0,GL_POSITION,lightpos);
 e.render();
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
   glVertex3f(0.0,0.0,0.0);
   glVertex3f(0.5,0.0,0.0);
   glVertex3f(0.5,0.0,0.5);
   glVertex3f(0.0,0.0,0.5);
 glEnd();
 glFlush();
 glutSwapBuffers();
 t++;
}

void idle(){
 e.update(dt);
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
  case 't':
  case 'T':
  t--;
  break;
  case 27:
   exit(0);
   break;
 }
}
void mouseMoved(int x, int y)
{
    if (mx>=0 && my>=0) {
        rotangles[0] += y-my;
        rotangles[1] += x-mx;
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
 gluPerspective(60.0f,(GLfloat)width/(GLfloat)height,1.0f,200.0f);
 glMatrixMode(GL_MODELVIEW);
}
int main(int argc, char** argv){
 Cubo c;
 Esfera f;
 Vector3D v;
 c.setPos(Vector3D(0.6,1,0));
 c.setVel(Vector3D(0.7,0,-0.71));
 c.setCol(Vector3D(1,0,0));
 c.setF(Vector3D(0,-0.98,0));
 c.setM(1);
 c.setS(0.4);
 e.add(c);
 f.setPos(Vector3D(-0.4,1.2,0));
 f.setVel(Vector3D(-0.3,0,0.7));
 f.setCol(Vector3D(1,0,1));
 f.setF(Vector3D(0,-0.98,0));
 f.setM(1);
 f.setR(0.4);
 e.add(f);

 f.setR(0.1);
 for(double i=-3;i<4;i++){
	 for(double j=0-3;j<4;j++){
		 f.setPos(Vector3D(i/2,0,j/2));
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


