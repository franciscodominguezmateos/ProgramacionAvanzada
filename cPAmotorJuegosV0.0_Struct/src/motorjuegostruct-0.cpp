#include <GL/glut.h>
#include <iostream>

GLfloat t=0.0f;
GLfloat pitch=0.0f;

struct Vector3D{
 float x,y,z;
};
Vector3D suma(Vector3D a,Vector3D b){
 Vector3D ret;
 ret.x=a.x+b.x;
 ret.y=a.y+b.y;
 ret.z=a.z+b.z;
 return ret;
}
Vector3D resta(Vector3D a,Vector3D b){
 Vector3D ret;
 ret.x=a.x-b.x;
 ret.y=a.y-b.y;
 ret.z=a.z-b.z;
 return ret;
}
struct Esfera{
 Vector3D pos;
 Vector3D vel;
 Vector3D color;
 float r;
};
struct Cubo{
 Vector3D pos;
 Vector3D vel;
 Vector3D color;
 float s;
};

Cubo cubos[10];

void renderCubo(Cubo c){
   glPushMatrix();
     glColor3f(c.color.x,c.color.y,c.color.z);
     glTranslatef(c.pos.x,c.pos.y,c.pos.z);
     glutSolidCube(c.s);
   glPopMatrix();
}
void displayMe(void){
 glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
 glLoadIdentity();
 glTranslatef(0.0f,0.0f,-3.0f);
 glRotatef(pitch,1.0,0.0,0.0);
 //glRotatef(t/2,0.0,1.0,0.0);
 GLfloat lightpos[]={5.0,15.0,5.0,0.0};
 glLightfv(GL_LIGHT0,GL_POSITION,lightpos);
 renderCubo(cubos[0]);
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
}
void idle(){
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
 cubos[0].pos.x=0.6;
 cubos[0].pos.y=0;
 cubos[0].pos.z=0;
 cubos[0].vel.x=-0.001;
 cubos[0].vel.y=0;
 cubos[0].vel.z=0;
 cubos[0].color.x=1;
 cubos[0].color.y=0;
 cubos[0].color.z=0;
 cubos[0].s=0.4;

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







