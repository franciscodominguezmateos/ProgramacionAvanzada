//#include <GL/glut.h>
#include <stdlib.h>
#include <iostream>
#include <mutex>
#include "opencv2/opencv.hpp"
#include "shader.h"
#include "vector3d.h"
#include "esfera.h"
#include "cubo.h"
#include "cilindro.h"
#include "rosco.h"
#include "compuesto.h"
#include "escena.h"
#include "pared.h"
#include "camara.h"
//#include "texture.h"
#include "rectangulo.h"
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
#include "walking_inverted_pendulum.h"
#include "solido_rigido.h"
#include "quaternion.h"
#include "loader_obj.h"
#include "view.h"


//Declaration needed in next .h
Mat global_img;
#include "socket_TCP_server.h"

using namespace cv;

CamaraFPSVR camVR;
/* SENSOR */
vector<CGI> sensorEvents;
mutex mtxSensorEvents;
class StringCGIProcessor:public StringProcessor{
public:
	string process(string &si){
		CGI event(si);
		mtxSensorEvents.lock();
		sensorEvents.clear();
		sensorEvents.push_back(si);
		mtxSensorEvents.unlock();
	    return "OK";
	}
};
void updateSensors(){
	if(!sensorEvents.empty()){
		vector<CGI>::iterator pos=sensorEvents.begin();
		mtxSensorEvents.lock();
		CGI e=sensorEvents.front();
		mtxSensorEvents.unlock();
		//sensorEvents.clear();
		//sensorEvents.erase(pos);
		if(e.size()==4){
			//cout << "e=" << e << endl;
			double x=stod(e["y"]);
			double y=stod(e["x"]);
			double z=stod(e["z"]);
			camVR.setRot(Vector3D(x, y, z));
		}
	}
}

double t=0.0;
double dt=1.0/30;

int mx=-1,my=-1;        // Previous mouse coordinates

Mat tablero;

double vel;


ModeloMaterial* circuit;
ModeloMaterial* mariokart;
LoaderOBJ *lo;

Escena e;
Cubo *pc;
ModeloMaterial* m;
Texture tex,ladrillos,paredTex,texTv,texTablero,spiderTex,marioKartTex,minionTex,mariokartTex;
VideoCapture cap(0);
CuboElastico *ce;
CajaElastica* cje;
CajaModeloElastico* cme;

// Pendulum masses
Solido *pt1;
Solido *pt2;

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
///vector<Vista> vistas={{0.0,0.0,0.5,1,&proyeccion},{0.5,0.0,0.5,1,&pCam}};//,{0.0,0.5,0.5,0.5},{0.5,0.5,0.5,0.5}};
vector<View> vistas={{0.0,0.0,0.5,1,&proyeccion},{0.5,0.0,0.5,1,&proyeccion}};
vector<CamaraTPS> camaras(vistas.size());

Mat opengl_default_frame_to_opencv() {
	//cv::Mat img(480, 640*2, CV_8UC3);
    cv::Mat img(1080, 1920, CV_8UC3);
    glPixelStorei(GL_PACK_ALIGNMENT, (img.step & 3)?1:4);
    glPixelStorei(GL_PACK_ROW_LENGTH, img.step/img.elemSize());
    glReadPixels(0, 0, img.cols, img.rows, GL_BGR_EXT, GL_UNSIGNED_BYTE, img.data);
    cv::Mat flipped(img);
    cv::flip(img, flipped, 0);
    return img;
}
void displayMe(void){
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	vistas[0].render();
	fondo.render();
    glLoadIdentity();
    //camaras[0].render();
    camVR.renderLeft();
    e.render();

	vistas[1].render();
	//fondoTablero.render();
	fondo.render();
    glLoadIdentity();
    //camAR->render();
    camVR.renderRight();
    //camaras[1].render();
    e.render();
    glutSwapBuffers();
    global_img=opengl_default_frame_to_opencv();
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
Triangle* nearestTriangle(vector<Triangle*> &triangles,Vector3D p){
	Vector3D up(0,1,0);
	double min=1e40;
	Triangle* nearest=nullptr;
	for(Triangle* &t:triangles){
		/* if triangle is up */
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
    return nearest;
}
void upKart(){
	Vector3D up(0,1,0);
	circuit->setDrawNormals(false);
	vector<Triangle*> &triangulos=circuit->getTriangulos();
	Vector3D p=mariokart->getPos();
	double min=1e40;
	Triangle* nearest=nullptr;
	for(Triangle* &t:triangulos){
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
	nearest=nearestTriangle(triangulos,p);
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
		//Vector3D vu=vr.X(vel);
		double cosv=f*vel;
		double cosr=f*vr;
		double dv=rad2deg(M_PI/2-acos(cosv));
		double dr=rad2deg(M_PI/2-acos(cosr));
		//cout << "dv="<<dv<<" cosv="<<cosv<< "dr="<<dr<<" cosr="<<cosr<<endl;
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
 e.limpiaFuerzas();
 //upKart();
 Mat i;
 cap>>i;
 tex.setImage(i);
 texTv.setImage(i);
 //setPoseCamAR(i);
 //tex.update();
 CamaraTPS &cam=camaras[0];
 cam.update(dt*vel);
 e.update(dt);
 updateSensors();
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
	    //CamaraTPS &cam=camaras[0];
		camVR.setRot(camVR.getRot() - Vector3D(0, 2, 0));
		//cam.update(dt);
	 /*axisZ=cme->getAxisZ();
	 for(Solido *s:cme->getParticulas()){
		 s->setVel(axisZ*5);
	 }
		axisX=cme->getAxisX();
		for(int i=0;i<cme->getParticulas().size();i++){
			Solido* s=cme->getParticula(i);
		//for(Solido *s:cme->getParticulas()){
			if (i==0 || i==4 || i==7 || i==3 )
				s->setVel(s->getVel()+axisX);
			//else
				//s->setVel(s->getVel()-axisX);
		}*/
		break;
 }
 case 'p':
 case 'P':{
	    //CamaraTPS &cam=camaras[0];
		//s = cam.getSolido();
		//s->setRot(s->getRot() - Vector3D(0, 2, 0));
		//cam.update(dt);
		camVR.setRot(camVR.getRot() + Vector3D(0, 2, 0));
	 /*axisZ=cme->getAxisZ();
	 for(Solido *s:cme->getParticulas()){
		 s->setVel(axisZ*5);
	 }
	 axisX=cme->getAxisX();
		for(int i=0;i<cme->getParticulas().size();i++){
			Solido* s=cme->getParticula(i);
		//for(Solido *s:cme->getParticulas()){
			if (i==0 || i==4 || i==7 || i==3 )
				s->setVel(s->getVel()-axisX);
			//else
				//s->setVel(s->getVel()-axisX);
		}*/
		break;
 }
 break;
 case 'q':
  case 'Q':
 	 m->setPos(m->getPos()+Vector3D(0.25,0,0));
  break;
  case 'w':
  case 'W':
 	 m->setPos(m->getPos()-Vector3D(0.25,0,0));
  break;
  case 'v':
   case 'V':
  	 camVR.setBaseline(camVR.getBaseline()-0.1);
   break;
   case 'B':
   case 'b':
  	 camVR.setBaseline(camVR.getBaseline()+0.1);
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
	 /*
 	for(unsigned int i=0;i<vistas.size();i++){
 		if(vistas[i].contain(x,y)){
 			Vector3D r;
 			CamaraTPS &cam=camaras[i];
 			cam.update(dt*2);
 		}
 	}
 	vel++;*/
	 camVR.update(dt);
 break;
 case 'g':
 case 'G':
	 /*
	 	for(unsigned int i=0;i<vistas.size();i++){
	 		if(vistas[i].contain(x,y)){
	 			Vector3D r;
	 			CamaraTPS &cam=camaras[i];
	 			cam.update(-dt*2);
	 		}
	 	}
	 vel--;*/
	 camVR.update(-dt);
 break;
 case 'a':
 case 'A':
	 axisZ=cme->getAxisZ();
	 for(Solido *s:cme->getParticulas()){
		 s->setVel(axisZ*5);
	 }
	 //for(Solido *s:cme->getParticulas()){
		 //s->setVel(s->getVel()+Vector3D(getRand(1,-1),1,-1));
	 //}
 break;
 case 's':
 case 'S':{
	    Mat img;
        img=opengl_default_frame_to_opencv();
        //img=global_img;
	    cv::imwrite("snapshot.png", img);
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
	for(View &v:vistas)
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
		 circuit->setDrawNormals(true);
		 //circuit->doTranslate(Vector3D(0,circuit->getMinY()*3,0));
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
		 circuit->doTranslate(Vector3D(20,0,-100));
		 e.add(circuit);
	 }
}
int main(int argc, char** argv) try{
	srand(10);
	//Quaternion test
	Quaternion q1(M_PI/2,Vector3D(0,0,1));
	Vector3D v1=q1*Vector3D(1,0,0);
	cout << "v1="<<v1<<endl;
	/* THREADS */
	//this thread stream the rendered game
	bool stop=false;
	thread server_th(video_jpeg_stream_server,&stop,4097);
	// wait a minute
	this_thread::sleep_for(chrono::milliseconds(100));
	// Launch input sensor server thread
	StringCGIProcessor scp;
	thread string_th(string_server,&stop,&scp,8881);
	// wait a minute
	this_thread::sleep_for(chrono::milliseconds(100));

	/*        PAGameVRLinusTrinusTest  */
 vel=0;
 //cout << t.isIn(Vector3D(0.25,0.25,0))<<endl;
 for(Camara &c:camaras){
	 c.setPos(Vector3D(0,1.65,10));
	 c.setRot(Vector3D(0,90,0));
 }
 Luz* l1=new Luz(Vector3D( 50,50,15));
 l1->hazFija();
 e.add(l1);
 //e.add(new Luz(Vector3D(-50,50,15)));

 /*  M E N U  */
 //int ci=2;
 //cout << "Please enter the circuit number from 0 to 4: ";
 //cin >>ci;

 glutInit(&argc,argv);
 //glutInitDisplayMode(GLUT_SINGLE);
 glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
 //glutInitWindowSize(640*2,480);
 //glutInitWindowSize(1920,1080);
 //glutInitWindowPosition(0,0);
 glutCreateWindow("Hello wold :D");
 glutFullScreen();
 init();


 // In order to use textures fist init() as to be called
 /*  M A R I O   K A R T */
 mariokart=new ModeloMaterial("mk_kart.obj");
 mariokart->hazFija();
 //mariokart->doScale(10);
 //e.add(mariokart);
 //camaras[0].setSolido(mariokart);

 //loadCircuit(ci);

 SolidoRigido *sr=new SolidoRigido(1,0.5,2);
 sr->setPos(Vector3D(0,2,0));
 //e.add(sr);

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
 e.add(minion_golf);


 mariokart->doCenter();
 //cme=new CajaModeloElastico(mariokart);
 cme=new MarioKart();
 e.add(cme);
 camaras[0].setSolido(cme);

 // Walking inverted pendulum
 // updated on idle
 pt1=new Solido(0,0,0);
 pt2=new Solido(0.1,2.5,0);
 WalkingInvertedPendulum* wip=new WalkingInvertedPendulum(pt1,pt2);
 e.add(wip);

 Vector3D p0(-80,0,-80);
 Vector3D p1(-80,0, 80);
 Vector3D p2( 80,0, 80);
 Vector3D p3( 80,0,-80);
 Rectangulo *ret;
 ret=new Rectangulo(p0,p1,p2,p3);
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
 Rectangulo *pared;
 pared=new Rectangulo(p0,p1,p2,p3);
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
catch (exception &e){
	cout << "Error at main() in V1.6_RigidBody:"<<e.what() <<endl;
}


