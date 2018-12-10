#include <GL/glut.h>
#include <iostream>

GLfloat t=0.0f;
GLfloat pitch=0.0f; 

struct Vector3D{
 float x,y,z;
 Vector3D operator+(Vector3D b);
};
Vector3D Vector3D::operator+(Vector3D b){
 Vector3D ret;
 ret.x=x+b.x;
 ret.y=y+b.y;
 ret.z=z+b.z;
 return ret;
}
struct Esfera{
 Vector3D pos;
 Vector3D vel;
 Vector3D color;
 float r;
 //void render();
 //void update();
};
struct Cubo{
 Vector3D pos;
 Vector3D vel;
 Vector3D color;
 float s;
};

Cubo cubos[10];
Esfera *esferas;

void render(Cubo c){
   glPushMatrix();
     glColor3f(c.color.x,c.color.y,c.color.z);
     glTranslatef(c.pos.x,c.pos.y,c.pos.z);
     glutSolidCube(c.s);
   glPopMatrix();
}
void render(Esfera e){
   glPushMatrix();
     glColor3f(e.color.x,e.color.y,e.color.z);
     glTranslatef(e.pos.x,e.pos.y,e.pos.z);
     glutSolidSphere(e.r,10,10);
   glPopMatrix();
}
void renderEscena(Esfera *e,Cubo *c){
 for(int i=0;i<1;i++)
  render(e[i]);
 for(int i=0;i<1;i++)
  render(c[i]);
}
void displayMe(void){
 glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
 glLoadIdentity();
 glTranslatef(0.0f,0.0f,-3.0f);
 glRotatef(pitch,1.0,0.0,0.0);
 //glRotatef(t/2,0.0,1.0,0.0);
 GLfloat lightpos[]={5.0,15.0,5.0,0.0};
 glLightfv(GL_LIGHT0,GL_POSITION,lightpos);
 renderEscena(esferas,cubos);
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
void update(){
 for(int i=0;i<1;i++){
  esferas[i].pos=esferas[i].pos + esferas[i].vel;
  if (abs(esferas[i].pos.x)>1.1){
    //esferas[i].pos.x=1;
    esferas[i].vel.x=-esferas[i].vel.x;
    esferas[i].color.y=1;
  }
  if (abs(esferas[i].pos.y)>1.1){
    //esferas[i].pos.x=1;
    esferas[i].vel.y=-esferas[i].vel.y;
    esferas[i].color.y=1;
  }
  if (abs(esferas[i].pos.z)>1.1){
    //esferas[i].pos.x=1;
    esferas[i].vel.z=-esferas[i].vel.z;
    esferas[i].color.y=1;
  }
 }
 for(int i=0;i<1;i++)
  cubos[i].pos=cubos[i].pos+cubos[i].vel;  
} 
void idle(){
 update();
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
 esferas=new Esfera[10];
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
 esferas[0].pos.x=-1;
 esferas[0].pos.y=0;
 esferas[0].pos.z=0;
 esferas[0].vel.x=0.06;
 esferas[0].vel.y=0.0;
 esferas[0].vel.z=0.08;
 esferas[0].color.x=1;
 esferas[0].color.y=0;
 esferas[0].color.z=1;
 esferas[0].r=0.4;
 
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


