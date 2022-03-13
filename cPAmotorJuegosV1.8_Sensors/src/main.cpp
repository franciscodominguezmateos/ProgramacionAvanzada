#include <stdlib.h>
#include <iostream>
#include <mutex>
#include "opencv2/opencv.hpp"
#include "shader.h"
#include "vector3d.h"
#include "esfera.h"
#include "rosco.h"
#include "pared.h"
#include "pendulo.h"
#include "cubo_elastico.h"
#include "fondo_textura.h"
#include "luz.h"
#include "proyeccion_perspectiva.h"
#include "pose_estimation_chessboard.h"
#include "material.h"
#include "modelo_material.h"
#include "caja_elastica.h"
#include "caja_modelo_elastico.h"
#include "camera_ar.h"
#include "composite.h"
#include "walking_inverted_pendulum.h"
#include "quaternion.h"
#include "loader_obj.h"
#include "solid_rigid_body.h"
#include "view.h"
#include "contact.h"
#include "cylinder.h"
#include "rectangle.h"
#include "stage.h"
#include "stage_rigid_body.h"
#include "line.h"
#include "arrow.h"
#include "axis.h"
#include "cube.h"

using namespace cv;

double t=0.0;
double dt=1.0/100;

int mx=-1,my=-1;        // Previous mouse coordinates

Mat tablero;

double vel;

ModeloMaterial* mariokart;
LoaderOBJ *lo;

Stage e;
Cube *pc;
ModeloMaterial* m;
Texture tex,ladrillos,paredTex,texTv,texTablero,spiderTex,marioKartTex,minionTex,mariokartTex;
VideoCapture cap(0);
CuboElastico *ce;
CajaElastica* cje;
CajaModeloElastico* cme;
StageRigidBody* srb;

// Pendulum masses
Solid *pt1;
Solid *pt2;

BackgroundTexture fondo,fondoTablero;

Mat K=(Mat_<double>(3,3) <<
		 1053.755323784897,                 0, 317.160420774003,
		                 0, 1067.981896308737, 203.4946241428054,
						 0,                 0,                 1);

Mat dist=Mat::zeros(4,1,cv::DataType<double>::type); // Assuming no lens distortion
CameraAR *camAR;
ProjectionCamera pCam(K);
PoseEstimationChessBoard peChessBoard(K,dist);

ProjectionPerspective proyeccion;
vector<View> vistas={{0.0,0.0,1,1,&proyeccion}};
vector<Camera> camaras(vistas.size());

void displayMe(void){
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	vistas[0].render();
	fondo.render();
    glLoadIdentity();
    camaras[0].render();

    e.render();

    glutSwapBuffers();
}
void idle(){
 t+=dt;
 e.limpiaFuerzas();

 Mat i;
 cap>>i;
 tex.setImage(i);
 texTv.setImage(i);

 e.update(dt);

 displayMe();
}
void keyPressed(unsigned char key,int x,int y){
	//Solido* s;
	Vector3D axisZ;
	Vector3D axisX;
 switch(key){
 case '.':{
	 Vector3D p=mariokart->getPos();
	 cout << "kart pos="<<p<<endl;
	 break;
 }
 case 'o':
 case 'O':{
		break;
 }
 case 'p':
 case 'P':{
		break;
 }
 break;
 case 'q':
  case 'Q':
  break;
  case 'w':
  case 'W':
  break;
  case 'v':
  case 'V':
  break;
  case 'B':
  case 'b':
  break;
 case ' ':
	 Sphere *pf;
	 pf=new Sphere();
	 pf->setPos(m->getPos());
	 pf->setVel(Vector3D(0,0,-1.0));
	 pf->setCol(Vector3D(1,0.5,0));
	 pf->setR(0.04);
	 e.add(pf);
 break;
 case 't':
 case 'T':
 break;
 case 'g':
 case 'G':
 break;
 case 'a':
 case 'A':
 break;
 case 's':
 case 'S':{
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
 //glShadeModel(GL_FLAT);
 //glShadeModel(GL_SMOOTH);
 tex.init();
 ladrillos.init();
 paredTex.init();
 texTv.init();
 texTablero.init();
 spiderTex.init();
 //this is to read transparency, but I have to prepare the texture object to support this
 //15/04/2020
 //spiderTex.setImage(imread("TheAmazingSpiderman1Tex.png"),cv2.IMREAD_UNCHANGED);
 spiderTex.setImage(imread("TheAmazingSpiderman1Tex.png"));
 //marioKartTex.init();
 //marioKartTex.setImage(imread("E_main.png"));
 mariokartTex.init();
 mariokartTex.setImage(imread("tex_0301.png"));
}
void reshape(int width,int height){
	for(View &v:vistas)
		v.reshape(width,height);
}
void lds_test(){
    //String filename = "/home/francisco/Pictures/Webcam/2020-12-10-192656.jpg";
    //String filename = "/home/francisco/Pictures/Webcam/2020-12-11-091815.jpg";
    String filename = "/home/francisco/Pictures/Webcam/2020-12-11-093613.jpg";
    bool useRefine = true;
    bool useCanny = false;
    bool overlay = true;

    Mat image = imread(filename, IMREAD_GRAYSCALE);

    if( image.empty() )
    {
        cout << "Unable to load " << filename<<endl;
        return;
    }

    imshow("Source Image", image);

    if (useCanny)
    {
        Canny(image, image, 50, 200, 3); // Apply Canny edge detector
    }

    // Create and LSD detector with standard or no refinement.
    Ptr<LineSegmentDetector> ls = useRefine ? createLineSegmentDetector(LSD_REFINE_STD) : createLineSegmentDetector(LSD_REFINE_NONE);

    double start = double(getTickCount());
    vector<Vec4f> lines_std;

    // Detect the lines
    ls->detect(image, lines_std);

    double duration_ms = (double(getTickCount()) - start) * 1000 / getTickFrequency();
    std::cout << "It took " << duration_ms << " ms." << std::endl;

    // Show found lines
    if (!overlay || useCanny)
    {
        image = Scalar(0, 0, 0);
    }

    ls->drawSegments(image, lines_std);

    String window_name = useRefine ? "Result - standard refinement" : "Result - no refinement";
    window_name += useCanny ? " - Canny edge detector used" : "";

    imshow(window_name, image);

    waitKey(0);
}
int main(int argc, char** argv) try{
	srand(10);
	lds_test();
	//Quaternion test
	Quaternion q1(M_PI/2,Vector3D(0,0,1));
	Vector3D v1=q1*Vector3D(1,0,0);
	cout << "v1="<<v1<<endl;

	Line l(Vector3D(2,3,-1),Vector3D(2,3,-1)+Vector3D(1,2,1));
	cout << "distance="<<l.distance(Vector3D(2,4,1))<<"="<<sqrt(7.0/3.0)<<endl;

	Line r0(Vector3D(2,-3,0),Vector3D(2,-3,0)+Vector3D(1,2,1));
	Line r1(Vector3D(0,0,0),Vector3D(1,1,1));
	cout << "distance="<<r0.distance(r1)<<"="<<sqrt(2.0)<<endl;
    Line r2(Vector3D(1,5,-2),Vector3D(1,5,-2)+Vector3D(2,-1,3));
    Line r3(Vector3D(-7,4,1),Vector3D(-7,4,1)+Vector3D(1,2,-6));
	cout << "interPoint"<<r2.intersectionPoint(r3)<<endl;

 vel=0;
 //cout << t.isIn(Vector3D(0.25,0.25,0))<<endl;
 for(Camera &c:camaras){
	 c.setPos(Vector3D(0,1.65,20));
	 c.setRot(Vector3D(0,90,0));
 }

 glutInit(&argc,argv);
 //glutInitDisplayMode(GLUT_SINGLE);
 glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
 //glutInitWindowSize(640,480);
 glutInitWindowSize(640,480);
 //glutInitWindowPosition(0,0);
 glutCreateWindow("Hello wold :D");
 //glutFullScreen();
 init();

 Light* l1=new Light(Vector3D( 50,50,15));
 l1->hazFija();
 e.add(l1);
 e.add(new Light(Vector3D(-50,50,15)));
 // In order to use textures fist init() as to be called
 /*  M A R I O   K A R T */
 //mariokart=new ModeloMaterial("mk_kart.obj");
 //mariokart->hazFija();
 //mariokart->doScale(10);
 //e.add(mariokart);
 //camaras[0].setSolido(mariokart);

 //loadCircuit(ci);

 //SolidRigidBody *sr=new SolidRigidBody(1,0.5,2);
 //sr->setPos(Vector3D(0,2,0));
 //sr->setRot(Vector3D(30,15,15));
 //e.add(sr);

 //srb=new StageRigidBody();
 //e.add(srb);
 Arrow* aw=new Arrow(0.05,1);
 aw->setPos(Vector3D(0,1,0));
 aw->setCol(Vector3D(0,0,1));
 aw->hazFija();
 e.add(aw);
 Axis* a=new Axis();
 e.add(a);
 //m=new ModeloMaterial("leia.obj");
 //m->setPos(Vector3D(0,0,-20));
 //m->setScale(0.1);
 //e.add(m);
 //m=new ModeloMaterial("TheAmazingSpiderman.obj");
 //m->setScale(Vector3D(4,4,4));
 //m->setPos(Vector3D(3,0,-40));
 //m->setVel(Vector3D(getRand(10,-10),0,-1.1));
 //e.add(m);
/*
 lo=new LoaderOBJ("TheAmazingSpiderman.obj");
 lo->setPos(Vector3D(3,0,-40));
 //lo->setScale(0.25);
 //m->setVel(Vector3D(getRand(10,-10),0,-1.1));
 e.add(lo);
*/
 ModeloMaterial* mm=new ModeloMaterial("M-FF_iOS_HERO_Natasha_Romanoff_Black_Widow_Age_Of_Ultron.obj");
 mm->setPos(Vector3D(0,0,-40));
 mm->setScale(3.5);
 //mm->setVel(Vector3D(getRand(10,-10),0,getRand(10,-10)));
 e.add(mm);

 /*ModeloMaterial* felicia=new ModeloMaterial("Lara Croft MAX 2010.obj");
 felicia->setPos(Vector3D(4,0,-20));
 felicia->setScale(0.5);
 //felicia->setVel(Vector3D(getRand(10,-10),0,getRand(10,-10)));
 e.add(felicia);
*/
 ModeloMaterial* shrek=new ModeloMaterial("shrek.obj");
 shrek->setPos(Vector3D(-4,0,-45));
 shrek->doScale(0.25);
 //shrek->setVel(Vector3D(getRand(10,-10),0,getRand(10,-10)));
 e.add(shrek);

 ModeloMaterial* minion_golf=new ModeloMaterial("mc_golf.obj");
 minion_golf->setPos(Vector3D(-4,0,0));
 minion_golf->setScale(4);
 minion_golf->setVel(Vector3D(getRand(10,-10),10,getRand(10,-10)));
 //e.add(minion_golf);


 //mariokart->doCenter();
 //cme=new CajaModeloElastico(mariokart);
 //cme=new MarioKart();
 //cme->setVel(Vector3D(1,0,1));
 //e.add(cme);
 //camaras[0].setSolido(cme);

 // Walking inverted pendulum
 // updated on idle
 pt1=new Solid(0,0,0);
 pt2=new Solid(0.1,2.5,0);
 WalkingInvertedPendulum* wip=new WalkingInvertedPendulum(pt1,pt2);
 e.add(wip);

 Vector3D p0(-80,0,-80);
 Vector3D p1(-80,0, 80);
 Vector3D p2( 80,0, 80);
 Vector3D p3( 80,0,-80);
 Rectangle *ret;
 ret=new Rectangle(p0,p1,p2,p3);
 ret->setCol(Vector3D(1,2.5,1));
 ladrillos.setImage(imread("mario_kart_circuit.jpg"));
 ret->getTex()=ladrillos;
 ret->setNU(1);
 ret->setNV(1);
 e.add(ret);

 p0=Vector3D(  0, 0,-10);
 p1=Vector3D( 20, 0,-10);
 p2=Vector3D( 20,10,-10);
 p3=Vector3D(  0,10,-10);
 Rectangle *pared;
 pared=new Rectangle(p0,p1,p2,p3);
 ret->setCol(Vector3D(1,1,1));
 paredTex.setImage(imread("brown_brick_texture_map.jpg"));
 pared->getTex()=paredTex;
 pared->setNU(1);
 pared->setNV(1);
 //e.add(pared);

 p0=Vector3D(  9, 2,-9.9);
 p1=Vector3D( 11, 2,-9.9);
 p2=Vector3D( 11, 3,-9.9);
 p3=Vector3D(  9, 3,-9.9);
 Rectangle *tv;
 tv=new Rectangle(p0,p1,p2,p3);
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

 //initCamAR();
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
catch (exception &e){
	cout << "Error at main() in V1.7_Audio:"<<e.what() <<endl;
}


