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

class TestImageFilters:public GameStandard{
	Texture texGround;
	Axis* ax;
	Axis* lookAtAxis;
public:
	TestImageFilters(string title):GameStandard(title){}
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

		double min, max;

		string path("/media/francisco/FREEDOS/datasets/pictures/RGBD/ICL-NUIM/living_room_traj0_frei_png");
		DepthImageFactory dif(path,"ICL_NUIM");
		DepthImage di=dif.get(0);

		Mat cimg=di.getImg();
		imshow("original",cimg);
		cimg.convertTo(cimg,CV_32FC3);
		cimg/=255;

		Mat fboImg;
    	Mat dimg=di.getDepth();
		minMaxLoc(dimg, &min, &max);
		cout << "maxd="<<max<<endl;
		//dimg/=max;
		vector<Mat> channels;
		split(cimg, channels);
		Mat img;
		img.convertTo(img,CV_32FC4);
		vector<Mat> vd = { channels[0], channels[1],channels[2], dimg };
		merge(vd, img);

		ShaderImageFilterSobelX sifX;
		fboImg=sifX.filter(di.getImgDepth());
		minMaxLoc(fboImg, &min, &max);
		cout << "maxfbo="<<min<<","<<max<<endl;
		//dimg/=max;
		imshow("holax",fboImg);

		ShaderImageFilterSobelY sifY;
		fboImg=sifY.filter(di.getImgDepth());
		minMaxLoc(fboImg, &min, &max);
		cout << "maxfbo="<<min<<","<<max<<endl;
		//dimg/=max;
		imshow("holay",fboImg);

		ShaderImageFilter sif;
		sif.init();
		fboImg=sif.filter(di.getImgDepth());
		minMaxLoc(fboImg, &min, &max);
		cout << "maxfbo="<<min<<","<<max<<endl;
		//dimg/=max;
		imshow("ShaderImageFilter",fboImg);

		ShaderImageFilterGauss sifg(&sif.getOutTex());
		//fboImg=sifg.filter(di.getImgDepth());
		sifg.render();
		fboImg=sifg.downloadResult();
		minMaxLoc(fboImg, &min, &max);
		cout << "maxfsifg="<<min<<","<<max<<endl;
		//dimg/=max;
		imshow("sifG",fboImg);

		fboImg=sifg.filter(cimg);
		imshow("Gauss",fboImg);

		ShaderImageFilterBilateral sifb;
		fboImg=sifb.filter(di.getImgDepth());
		minMaxLoc(fboImg, &min, &max);
		cout << "maxfbo="<<min<<","<<max<<endl;
		//dimg/=max;
		imshow("holaB",fboImg);

		//dilation max
		//erode    min
		//medianBlur
		ShaderImageFilterKernel sifk;
		Mat k=(Mat_<float>(3,3)<<1,0,-1,
				                 1,0,-1,
								 1,0,-1);
		k=k*1.0/18.0;
		sifk.setKernel(sifk.meanKernel);
		fboImg=sifk.filter(cimg);
		//sifk.render();
		//fboImg=sifk.downloadResult();
		minMaxLoc(fboImg, &min, &max);
		cout << "maxfbo="<<min<<","<<max<<endl;
		//dimg/=max;
		imshow("holaK",fboImg);

		ShaderReduceN srn(5);
		srn.setImage(cimg);
		srn.render();
		fboImg=srn.downloadResult();
		imshow("ShadeRenduceN",fboImg);

		ShaderBinOp sbo;
		sbo.setImage(cimg);
		Mat m=Mat::zeros(480,640,CV_32FC3);
		circle(m,Point( 240, 320 ), 32*4.0, Scalar( 0, 0, 1 ), 10, 8 );
		imshow("m",m);
		TexturePtr ptex1=new Texture(m);
		ptex1->init();
		sbo.setTex1(ptex1);
		sbo.render();
		fboImg=sbo.downloadResult();
		imshow("ShaderBinOp",fboImg);

		ShaderMap sm;
		fboImg=sm.filter(cimg);
		imshow("ShaderMap",fboImg);

		ax=new Axis();
		stage.add(ax);
		lookAtAxis=new Axis();
		lookAtAxis->setName(Text("LookAtAxis"));
		stage.add(lookAtAxis);
		//Floor
		texGround.init();
		//Mat imgGround=imread("brick_pavement_0077_01_preview.jpg");
		Mat imgGround=imread("brown_brick_texture_map.jpg");
		texGround.setImage(cimg);
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
} testShaderToy("cPATestImageFilters");

int main(int argc, char** argv) try{
	srand(10);
	GameEngine::setGame(&testShaderToy);
	GameEngine::gameInit(argc,argv);
	GameEngine::gameMainLoop();
	return 0;
}
catch (exception &e){
	cout << "Error in cPATestImageFilters::main(): "<<e.what() <<endl;
	return 1;
}
