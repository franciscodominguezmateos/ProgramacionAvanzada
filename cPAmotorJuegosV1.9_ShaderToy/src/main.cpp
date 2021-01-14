#include <stdlib.h>
#include <iostream>
#include <mutex>
#include "opencv2/opencv.hpp"
#include "game_engine.h"
#include "game_standard.h"
#include "vector3d.h"
#include "esfera.h"
#include "rosco.h"
#include "pared.h"
#include "pendulo.h"
#include "cubo_elastico.h"
#include "fondo_textura.h"
#include "luz.h"
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
#include "projection_camera.h"
#include "shader_toy.h"

class cPAmotorJuegosShaderToy:public GameStandard{
	TexturePtr pTexGround;
	VideoCapture cap;
	RectanglePtr tv;
	Texture tex;
public:
	cPAmotorJuegosShaderToy(string title):GameStandard(title),cap(0){}
	void init(){
		Camera &cam   =*getCamera();
		Stage  &stage =*getStage();
		//cam.setPos(Vector3D(0,0,10));
		Luz* lightFront=new Luz(Vector3D(0,0,1));
		stage.add(lightFront);
		//stage.add(new Luz(Vector3D(-1,1, -1)));

		ShaderToy* st=new ShaderToy();
		st->init();
		stage.add(st);

		AxisPtr origin=new Axis();
		origin->setName(Text("Origin"));
		stage.add(origin);

		ModeloMaterial* mm=new ModeloMaterial("shrek.obj");
		mm->setPos(Vector3D(0,0,-3));
		mm->setScale(0.125);
		//mm->setVel(Vector3D(getRand(10,-10),0,getRand(10,-10)));
		stage.add(mm);

		pTexGround=new Texture("brown_brick_texture_map.jpg");
		pTexGround->init();
		float l=10;
		Rectangle* r=new Rectangle(Vector3D(-l,0,-l*2),Vector3D(-l,0,l*2),Vector3D(l,0,l*2),Vector3D(l,0,-l*2));
		r->setTextura(*pTexGround);
		r->setNU(10);
		r->setNV(10);
		stage.add(r);

		 // Walking inverted pendulum
		 // updated on idle
		 SolidPtr pt1=new Solid(0,0,0);
		 SolidPtr pt2=new Solid(0.1,2.5,0);
		 WalkingInvertedPendulum* wip=new WalkingInvertedPendulum(pt1,pt2);
		 stage.add(wip);

		 //StageRigidBody* srb=new StageRigidBody();
		 //stage.add(srb);

		 Vector3D p0(  2, 2,-2.9);
		 Vector3D p1(  4, 2,-2.9);
		 Vector3D p2(  4, 3,-2.9);
		 Vector3D p3(  2, 3,-2.9);
		 tv=new Rectangle(p0,p1,p2,p3);
		 tv->setCol(Vector3D(1,1,1));

		 //bool rwc=cap.open(0);
		 //if(!rwc) cout<<"Error openning the webcam 0";
		 Mat i;
		 cap>>i;
		 tex.init();
		 tex.setImage(i);
		 tv->getTex()=tex;
		 stage.add(tv);

		 //CajaElastica* cje=new CajaElastica(2,4,2);
		 //stage.add(cje);

		 CuboElastico* ce=new CuboElastico(1);
		 ce->setPos(Vector3D(-4,3,0));
		 ce->setTexture(tex);
		 //stage.add(ce);

		 //initCamAR();
		 //fondo.setTextura(texTv);
		 //fondoTablero.setTextura(texTablero);

		 MarioKart* cme=new MarioKart();
		 cme->setVel(Vector3D(1,0,1));
		 stage.add(cme);
	}
	void update(double dt){
		 Mat i;
		 cap>>i;
		 if(tv!=nullptr) tv->getTex().setImage(i);
	}
} motorJuegosShaderToy("cPAmotorJuegosShaderToy");

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

FondoTextura fondo,fondoTablero;

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
	GameEngine::setGame(&motorJuegosShaderToy);
	GameEngine::gameInit(argc,argv);
	GameEngine::gameMainLoop();

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

 return 0;
}
catch (exception &e){
	cout << "Error at cPAmotorJuegosShaderToy::main() "<<e.what() <<endl;
}


