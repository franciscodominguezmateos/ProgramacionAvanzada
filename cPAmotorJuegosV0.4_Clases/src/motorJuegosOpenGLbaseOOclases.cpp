#include <GL/glut.h>
#include <iostream>
#include "vector3d.h"
#include "esfera.h"
#include "cubo.h"
#include "escena.h"

GLfloat t=0.0f;
GLfloat pitch=0.0f; 

Escena e;

void displayMe(void){
 glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
 glLoadIdentity();
 glTranslatef(0.0f,0.0f,-3.0f);
 glRotatef(pitch,1.0,0.0,0.0);
 //glRotatef(t/2,0.0,1.0,0.0);
 GLfloat lightpos[]={5.0,15.0,5.0,0.0};
 glLightfv(GL_LIGHT0,GL_POSITION,lightpos);
 e.render();
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
 t++;
}
/*
void update(){
	Vector3D v;
 for(int i=0;i<1;i++){
  esferas[i].setPos(esferas[i].getPos() + esferas[i].getVel());
  if (abs(esferas[i].getPos().getX())>1.1){
    //esferas[i].pos.x=1;
	  v=esferas[i].getVel();
	  v.setX(-v.getX());
	  esferas[i].setVel(v);
    //esferas[i].color.y=1;
  }
  if (abs(esferas[i].getPos().getY())>1.1){
	  v=esferas[i].getVel();
	  v.setY(-v.getY());
	  esferas[i].setVel(v);
    //esferas[i].pos.x=1;
    //esferas[i].color.y=1;
  }
  if (abs(esferas[i].getPos().getZ())>1.1){
    //esferas[i].pos.x=1;
	  v=esferas[i].getVel();
	  v.setZ(-v.getZ());
	  esferas[i].setVel(v);
    //esferas[i].color.y=1;
  }
 }
 for(int i=0;i<1;i++)
  cubos[i].setPos(cubos[i].getPos()+cubos[i].getVel());
} 
*/
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
  t--;
  break;
  case 27:
   exit(0);
   break;
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
 glutMainLoop();
 return 0;
}


