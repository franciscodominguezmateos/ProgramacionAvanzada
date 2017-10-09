#include <GL/glut.h>
#include <iostream>
#include "vector3d.h"
#include "esfera.h"
#include "cubo.h"
#include "escena.h"

GLfloat t=0.0f;
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
   glVertex3f(0.0,0.0,0.0);
   glVertex3f(0.5,0.0,0.0);
   glVertex3f(0.5,0.0,0.5);
   glVertex3f(0.0,0.0,0.5);
 glEnd();
 glFlush();
 glutSwapBuffers();
 //t++;
}

void idle(){
 //update();
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
  t++;
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
 gluPerspective(90.0f,(GLfloat)width/(GLfloat)height,1.0f,200.0f);
 glMatrixMode(GL_MODELVIEW);
}
int main(int argc, char** argv){
 Cubo c;
 Esfera f;
 Cilindro l;
 Vector3D v;
 c.setPos(Vector3D(0.6,0,0));
 c.setVel(Vector3D(-0.001,0,0));
 c.setCol(Vector3D(1,0,0));
 c.setS(0.4);
 e.add(c);
 v.setX(-0.4);
 v.setY(0);
 v.setZ(0);
 f.setPos(v);
 v.setX(-0.03);
 v.setY(0);
 v.setZ(0.08);
 f.setVel(v);
 v.setX(1);
 v.setY(0);
 v.setZ(1);
 f.setCol(v);
 f.setR(0.4);
 e.add(f);
 l.setPos(Vector3D(0,0,-1));
 e.add(l);
 
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

