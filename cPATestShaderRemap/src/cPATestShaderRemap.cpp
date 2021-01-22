//============================================================================
// Name        : cPATestImageFilters.cpp
// Author      : Francisco Dominguez
// Version     :
// Copyright   : Your copyright notice
// Description : Testing classes ShaderImageFilter and family
// Date: 07/01/2021
//============================================================================
#include <iostream>
#include "game_engine.h"
#include "game_standard.h"
#include "axis.h"
#include "rectangle.h"
#include "shader_image_filter.h"
#include "shader_image_filter_sobel_x.h"
#include "shader_image_filter_sobel_y.h"
#include "shader_image_filter_bilateral.h"
#include "shader_image_filter_gauss.h"
#include "shader_image_filter_kernel.h"
#include "shader_gpgpu_reduce_half.h"
#include "shader_gpgpu_reduce_n.h"
#include "shader_gpgpu_binop.h"
#include "depth_image.h"

using namespace std;

Mat getLeftMap(){
	/*
	 * LEFT CAMERA
	 camera_matrix:
	  rows: 3
	  cols: 3
	  data: [951.4741259832514, 0, 293.3416170604175, 0, 950.3903009136468, 272.6355355349708, 0, 0, 1]
	distortion_model: plumb_bob
	distortion_coefficients:
	  rows: 1
	  cols: 5
	  data: [-0.4953024672257269, 0.3330644888148587, -0.004006958790423586, 0.008548218245607087, 0]
	rectification_matrix:
	  rows: 3
	  cols: 3
	  data: [0.9960354394631838, 0.007417679552695122, -0.0886475118851934, -0.008281288486258502, 0.9999217313542504, -0.009378242187402474, 0.08857100876917536, 0.01007517719803191, 0.996018909062493]
	projection_matrix:
	  rows: 3
	  cols: 4
	  data: [953.0537522907438, 0, 384.4876556396484, 0, 0, 953.0537522907438, 276.9204998016357, 0, 0, 0, 1, 0]
*/
	Mat K=(Mat_<double>(3,3)<<951.4741259832514, 0, 293.3416170604175, 0, 950.3903009136468, 272.6355355349708, 0, 0, 1);
	Mat d=(Mat_<double>(1,5)<<-0.4953024672257269, 0.3330644888148587, -0.004006958790423586, 0.008548218245607087, 0);
	Mat R=(Mat_<double>(3,3)<<0.9960354394631838, 0.007417679552695122, -0.0886475118851934, -0.008281288486258502, 0.9999217313542504, -0.009378242187402474, 0.08857100876917536, 0.01007517719803191, 0.996018909062493);
	Mat P=(Mat_<double>(3,4)<<953.0537522907438, 0, 384.4876556396484, 0, 0, 953.0537522907438, 276.9204998016357, 0, 0, 0, 1, 0);
	Mat map1,map2;
	initUndistortRectifyMap(K,d,R,P,Size(640,480),CV_32FC2,map1,map2);
	return map1;
}
Mat getRightMap(){
	/*
	   RIGHT CAMERA
	camera_matrix:
	  rows: 3
	  cols: 3
	  data: [958.8710177402639, 0, 310.8323417297909, 0, 954.3393084573635, 282.4460052293588, 0, 0, 1]
	distortion_model: plumb_bob
	distortion_coefficients:
	  rows: 1
	  cols: 5
	  data: [-0.5177046435417091, 0.4190207743279942, -0.01204088578226774, -0.002396026919730114, 0]
	rectification_matrix:
	  rows: 3
	  cols: 3
	  data: [0.9968596736669864, 0.008162426114800682, -0.07876652725917205, -0.007394536192554946, 0.9999222997971208, 0.01003569643492776, 0.07884232271468458, -0.009421739136563212, 0.9968425748234252]
	projection_matrix:
	  rows: 3
	  cols: 4
	  data: [953.0537522907438, 0, 384.4876556396484, -57.95554829812738, 0, 953.0537522907438, 276.9204998016357, 0, 0, 0, 1, 0]
	 */
	Mat K=(Mat_<double>(3,3)<<958.8710177402639, 0, 310.8323417297909, 0, 954.3393084573635, 282.4460052293588, 0, 0, 1);
	Mat d=(Mat_<double>(1,5)<<-0.5177046435417091, 0.4190207743279942, -0.01204088578226774, -0.002396026919730114, 0);
	Mat R=(Mat_<double>(3,3)<<0.9968596736669864, 0.008162426114800682, -0.07876652725917205, -0.007394536192554946, 0.9999222997971208, 0.01003569643492776, 0.07884232271468458, -0.009421739136563212, 0.9968425748234252);
	Mat P=(Mat_<double>(3,4)<<953.0537522907438, 0, 384.4876556396484, 0/*-57.95554829812738*/, 0, 953.0537522907438, 276.9204998016357, 0, 0, 0, 1, 0);
	Mat map1,map2;
	initUndistortRectifyMap(K,d,R,P,Size(640,480),CV_32FC2,map1,map2);
	return map1;
}
class TestShaderRemap:public GameStandard{
	Texture texGround;
	Axis* ax;
	Axis* lookAtAxis;
public:
	TestShaderRemap(string title):GameStandard(title){}
	virtual void mouseMoved(int x, int y){
		GameStandard::mouseMoved(x,y);
	}
	virtual void mousePress(int button, int state, int x, int y){
		GameStandard::mousePress(button,state,x,y);
	}
	virtual void keyPressed(char key,int x,int y){
		GameStandard::keyPressed(key,x,y);
		lookAtAxis->setPos(getCamera()->getLookAtPos());
	}
	Mat stackH(Mat im1,Mat im2){
		Mat im3;
		hconcat(im1,im2,im3);
		return im3;
	}
	//TestImageFilters::init()
	void init(){
		Camera* pCam  =getCamera();
		Stage*  pStage=getStage();
		Camera &cam   =*pCam;
		Stage  &stage =*pStage;
		cam.setPos(Vector3D(0,0,10));
		Light* lightFront=new Light(Vector3D(150.0,150.0, 100.0));
		stage.add(lightFront);
		stage.add(new Light(Vector3D(-100.0,150.0, -150.0)));

		cv::namedWindow("imgL");
		cv::moveWindow("imgL", 0, 50);
		double min, max;
		int width=640;
		int height=480;
	    Rect roiL = Rect(    0, 0, width, height);
	    Rect roiR = Rect(width, 0, width, height);

	    String filename = "/home/francisco/Pictures/Webcam/2020-12-11-093613.jpg";
	    //Mat image = imread(filename, IMREAD_GRAYSCALE);
	    //Mat frame=image;
	    Mat image = imread(filename);
	    Mat frame;
	    image.convertTo(frame,CV_32FC3);
	    frame/=255;

	    //split image in imageLeft and imageRight
	    //read intrinsic and distortion for both cameras
	    //Compute map with initUndistortRectifyMap with CV_32FC2 for both cameras
	    //Prepare the map as a texture to be uploaded with sbo.setTex1
        Mat imgLo=frame(roiL);
        Mat imgRo=frame(roiR);
        Mat imgL=imgLo.clone();
        Mat imgR=imgRo.clone();

        //imshow("imgL",imgL);
        Mat mapL=getLeftMap();

        Mat imgLrect;
        //remap(imgL,imgLrect,mapL,Mat(),INTER_LINEAR);
        //imshow("imgLrect",imgLrect);

        Mat fboImg;
	    ShaderBinOp remap(nullptr,640,480,"vec4 opt(vec4 v0,vec4 v1){return remap(v0,v1);}");
	    //ShaderBinOp remap;
	    remap.setImage(imgL);
		//Mat m=Mat::zeros(480,640,CV_32FC3);
		//circle(m,Point( 240, 320 ), 32*4.0, Scalar( 0, 0, 1 ), 10, 8 );
		//imshow("mapL",m);
		//TexturePtr ptex1=new Texture(m);
		TexturePtr ptex1=new Texture(mapL);
		ptex1->init();
		remap.setTex1(ptex1);
		remap.render();
		imgLrect=remap.downloadResult();

        //imshow("imgR",imgR);
        Mat mapR=getRightMap();

        Mat imgRrect;
	    ShaderBinOp remapR(nullptr,640,480,"vec4 opt(vec4 v0,vec4 v1){return remap(v0,v1);}");
	    remapR.setImage(imgR);
		//Mat m=Mat::zeros(480,640,CV_32FC3);
		//circle(m,Point( 240, 320 ), 32*4.0, Scalar( 0, 0, 1 ), 10, 8 );
		//imshow("mapL",m);
		//TexturePtr ptex1=new Texture(m);
		TexturePtr ptex1R=new Texture(mapR);
		ptex1R->init();
		remapR.setTex1(ptex1R);
		remapR.render();
		imgRrect=remapR.downloadResult();

		fboImg=stackH(imgLrect,imgRrect);
		for(int row=32;row<480  ;row+=32)line(fboImg,Point(0,row),Point(640*2,row),Scalar(255,0,0),1);
		for(int col=32;col<640*2;col+=32)line(fboImg,Point(col,0),Point(col  ,480),Scalar(255,0,0),1);
		imshow("ShaderBinOp/remapLR",fboImg);

		ax=new Axis();
		stage.add(ax);
		lookAtAxis=new Axis();
		lookAtAxis->setName(Text("LookAtAxis"));
		stage.add(lookAtAxis);
		//Floor
		texGround.init();
		//Mat imgGround=imread("brick_pavement_0077_01_preview.jpg");
		Mat imgGround=imread("brown_brick_texture_map.jpg");
		texGround.setImage(imgGround);
		float l=10;
		Rectangle* r=new Rectangle(Vector3D(-l,0,-l*2),Vector3D(-l,0,l*2),Vector3D(l,0,l*2),Vector3D(l,0,-l*2));
		r->setTextura(texGround);
		r->setNU(10);
		r->setNV(10);
	    stage.add(r);
	}
	void update(double dt){
		if (waitKey(1) == 27) exit(0);
	}
} testShaderRemap("cPATestShaderRemap");

int main(int argc, char** argv) try{
	srand(10);
	GameEngine::setGame(&testShaderRemap);
	GameEngine::gameInit(argc,argv);
	GameEngine::gameMainLoop();
	return 0;
}
catch (exception &e){
	cout << "Error in cPATestShaderRemap::main(): "<<e.what() <<endl;
	return 1;
}
