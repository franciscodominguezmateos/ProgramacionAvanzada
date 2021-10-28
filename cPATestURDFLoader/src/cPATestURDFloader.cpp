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
#include "xml_parser.h"
#include "model_joint.h"
#include "solid_articulated.h"
#include "loader_dae.h"
#include "solid_vao.h"

using namespace std;

class JointURDF:public Joint{
	Vector3D axis;
public:

};
class LoaderURDF: public Loader{
    XMLNode urdf;
    map<string,SolidPtr> solids;
    GLSLShaderProgram shaderProgram;
public:
	// return all joints where linkName appear as parent link
	vector<string> getParentJoints(XMLNode& robot,string& linkName){
		vector<string> jointNames;
	    for(XMLNode &xn:robot["joint"]){
	    	string name=xn.getAttribute("name");
	    	string type=xn.getAttribute("type");
	    	//cout << xn.getAttribute("name") <<": " <<xn.getAttribute("type")<< endl << "    ";
    		try{
				string parentLinkName=xn("parent").getAttribute("link");
				if(parentLinkName==linkName){
					jointNames.push_back(name);
					//cout << "[" << parentLinkName << "->" << xn("child").getAttribute("link") << "]";
				}
			}catch(...){}
    		//cout<<endl;
	    }
	    return jointNames;
	}
	// return all joints where linkName appears as child link
	// since this is a tree this must be only one.
	vector<string> getChildJoints(XMLNode& robot,string& linkName){
		vector<string> jointNames;
	    for(XMLNode &xn:robot["joint"]){
	    	string name=xn.getAttribute("name");
	    	string type=xn.getAttribute("type");
	    	//cout << xn.getAttribute("name") <<": " <<xn.getAttribute("type")<< endl << "    ";
    		try{
				string childLinkName=xn("child").getAttribute("link");
				if(childLinkName==linkName){
					jointNames.push_back(name);
					//cout << "[" << parentLinkName << "->" << xn("child").getAttribute("link") << "]";
				}
			}catch(...){}
    		//cout<<endl;
	    }
	    return jointNames;
	}
	//counter is the joint idx field
	Joint createJoint(XMLNode& robot,string& linkName,unsigned int& counter,string indent=""){
		//cout << indent << linkName ;
		//XMLNode xnLink=robot("link","name",linkName);
		//if(xnLink.hasChild("visual")){
		//	string fileName=split(xnLink("visual")("geometry")("mesh").getAttribute("filename"),':')[1];
		//	cout << ":"<<fileName<<endl;
		//}
		//else cout<<endl;
		XMLNode& xnJoint=robot("joint","name",getChildJoints(robot,linkName)[0]);
		Mat m=getMat(xnJoint);
		//Vector3D axis=split_Vector3D(xnJoint("axis").getAttribute("xyz"));
		Joint joint(counter,linkName,m);
		if(indent.size()>=5*2) return joint;
		for(string sParent:getParentJoints(robot,linkName)){
			XMLNode& xnJoint=robot("joint","name",sParent);
			string linkChildName=xnJoint("child").getAttribute("link");
			Joint jointChild=createJoint(robot,linkChildName,++counter,indent+"  ");
			joint.addChild(jointChild);
		}
		return joint;
	}
	XMLNode& getJointParent(XMLNode& robot,XMLNode& joint){
		string parnetLinkName=joint("parent").getAttribute("link");
		vector<string> lj=getChildJoints(robot,parnetLinkName);
		assert(lj.size()==1);
		string parentJointName=lj[0];
		XMLNode& jointParent=robot("joint","name",parentJointName);
		return jointParent;
	}
void printJoint(XMLNode& xn){
		string name=xn.getAttribute("name");
		string type=xn.getAttribute("type");
		cout << "    "<<xn.getAttribute("name") <<": " <<xn.getAttribute("type")<< endl << "        ";
		if(type=="revolute" || type=="continuous" || type=="fixed"){
			if(xn.hasChild("parent"))
				cout << "[" << xn("parent").getAttribute("link") << "->" << xn("child").getAttribute("link") << "]";
			if(xn.hasChild("origin"))
				cout << " = " << xn("origin").getAttribute("rpy")<<","<< xn("origin").getAttribute("xyz");
			if(xn.hasChild("axis"))
				cout <<" , "<< xn("axis").getAttribute("xyz");
		}
		cout<<endl;
	}
    Mat getMat(XMLNode& joint){
    	if(joint.hasChild("origin")){
    		string rpy=joint("origin").getAttribute("rpy");
    		string xyz=joint("origin").getAttribute("xyz");
    		Vector3D pos=split_Vector3D(xyz);
    		Vector3D theta=split_Vector3D(rpy);
    		Mat m=posEulerAnglesToTransformationMatrix<float>(pos,theta*RAD2DEG);
    		return m;
    	}
    	else throw runtime_error("Joint "+joint.getAttribute("name")+" doesn't have attribute 'origin' in getMat()");
    }
	SolidVAOPtr loadSolidVAO(string fileName,float scale=0.1){
		SolidVAOPtr pvao=new SolidVAO(&shaderProgram);
		SolidVAO& vao=*pvao;
        ModelMesh mm=loadModelMesh(fileName,scale);
        vao.init(mm);
        vao.setMaterial("meshes/1.0/juliette_face.png");
        return pvao;
	}
	ModelMesh loadModelMesh(string fileName,float scale=0.1){
	    string package=".";
	    string folder="/meshes/1.0/";
	    Loader::model_base_path=package;
	    LoaderDAE ldae(fileName,0,folder);
	    ldae.loadNodeRootFromFileName(package+folder+fileName);
	    ldae.loadGeometry();
        ModelMesh mm=ldae.getModel();
        mm.doScale(scale);
        mm=mm.buildShaderReadyMeshModel();
        return mm;
	}
    void init(GLSLShaderProgram& shaderProgram,Stage& stage){
	    ifstream ifdae("pepper.urdf");
	    ifdae >> urdf;

	    string jointRootLinkName="torso";
	    unsigned int counter=0;
	    Joint jointRoot=createJoint(urdf("robot"),jointRootLinkName,counter);
	    //I DON'T KNOW WHY !!!!!!!!!!!!!!!!!!!!!!!!!!!!
	    for(Joint& j:jointRoot.getChildren()) j.setParent(&jointRoot);
	    cout << jointRoot << endl;
	    Mat I=Mat::eye(4,4,CV_32F);
	    jointRoot.calcInverseBindTransform(I);


	    SolidArticulated* sa=new SolidArticulated();
	    sa->setJointsRoot(jointRoot);
	    sa->setJointNames(jointRoot.getJointNames());
	    sa->setPose(jointRoot.getLocalBindPose());
	    //stage.add(sa);

	    //Joint& j=jointRoot.getJointByName("RThumb2_link");
	    Joint& j=jointRoot.getJointByName("r_wrist");
	    JointPtr pj=&j;
	    while(!pj->isRoot()){
	    	cout << pj->getName() << "->";
	    	pj=pj->getParentPtr();
	    }
    	cout << pj->getName() <<  endl;
	    Joint& jbf=jointRoot.getJointByName("base_footprint");
	    pj=&jbf;
	    while(!pj->isRoot()){
	    	cout << pj->getName() << "->";
	    	pj=pj->getParentPtr();
	    }
    	cout << pj->getName() <<  endl;
	    //string package="/opt/ros/kinetic/share/pepper_meshes/";
	    string package=".";
	    string folder="/meshes/1.0/";

	    cout << "*************** LINK ***************"<<endl;
	    for(XMLNode &xn:urdf("robot")["link"]) try {
	    	string name=xn.getAttribute("name");
	    	//cout << name << endl;
	    	//for(string& s:getChildJoints(urdf("robot"),name)){
	    		//cout << "    " << s << endl;
	    		//printJoint(urdf("robot")("joint","name",s));
	    	//}
	    	//cout << "    " << xn("visual")("geometry")("mesh").getAttribute("filename") << endl;
	    	if(xn.hasChild("visual") /*&& name[0]=='L'*/){
	    		string fileName=split(xn("visual")("geometry")("mesh").getAttribute("filename"),':')[1];
	    		fileName=ltrim(fileName,"/");
	    		fileName=split(fileName,'/').back();
	    		//cout << "    " << "fileName= "<< fileName <<endl;
	    		SolidVAOPtr pvao=loadSolidVAO(fileName,0.001);
	    		Joint& joint=jointRoot.getJointByName(name);
	    		Mat bt=joint.getBindTransform();
   				bt.convertTo(bt, CV_64F);
	    		pvao->setTransformationMat(bt);
	    		pvao->hazFija();
	    		stage.add(pvao);
	    		solids[name]=pvao;
	    	}
	    	if(!xn.hasChild("visual")){
	    		AxisPtr paxis=new Axis(Vector3D(0.05,0.05,0.05));
	    		Joint& joint=jointRoot.getJointByName(name);
	    		Mat bt=joint.getBindTransform();
   				bt.convertTo(bt, CV_64F);
	    		paxis->setTransformationMat(bt);
	    		//stage.add(paxis);
	    	}
	    }
	    catch(...){}
    }
};
class TestURDFloader:public GameStandard{
	Texture texGround;
	Axis* ax;
	Axis* lookAtAxis;
    GLSLShaderProgram shaderProgram;
    SolidVAO mvao;
public:
	TestURDFloader(string title):GameStandard(title),mvao(&shaderProgram){}
	virtual void mouseMoved(int x, int y){
		GameStandard::mouseMoved(x,y);
	}
	virtual void mousePress(int button, int state, int x, int y){
		GameStandard::mousePress(button,state,x,y);
	}
	virtual void keyPressed(char key,int x,int y){
		GameStandard::keyPressed(key,x,y);
		Vector3D p=getCamera().getLookAtPos();
		lookAtAxis->setPos(p);
	}
	Mat stackH(Mat im1,Mat im2){
		Mat im3;
		hconcat(im1,im2,im3);
		return im3;
	}
	// return all joints where linkName appear as parent link
	vector<string> getParentJoints(XMLNode& robot,string& linkName){
		vector<string> jointNames;
	    for(XMLNode &xn:robot["joint"]){
	    	string name=xn.getAttribute("name");
	    	string type=xn.getAttribute("type");
	    	//cout << xn.getAttribute("name") <<": " <<xn.getAttribute("type")<< endl << "    ";
    		try{
				string parentLinkName=xn("parent").getAttribute("link");
				if(parentLinkName==linkName){
					jointNames.push_back(name);
					//cout << "[" << parentLinkName << "->" << xn("child").getAttribute("link") << "]";
				}
			}catch(...){}
    		//cout<<endl;
	    }
	    return jointNames;
	}
	// return all joints where linkName appears as child link
	// since this is a tree this must be only one.
	vector<string> getChildJoints(XMLNode& robot,string& linkName){
		vector<string> jointNames;
	    for(XMLNode &xn:robot["joint"]){
	    	string name=xn.getAttribute("name");
	    	string type=xn.getAttribute("type");
	    	//cout << xn.getAttribute("name") <<": " <<xn.getAttribute("type")<< endl << "    ";
    		try{
				string childLinkName=xn("child").getAttribute("link");
				if(childLinkName==linkName){
					jointNames.push_back(name);
					//cout << "[" << parentLinkName << "->" << xn("child").getAttribute("link") << "]";
				}
			}catch(...){}
    		//cout<<endl;
	    }
	    return jointNames;
	}
	//counter is the joint idx field
	Joint createJoint(XMLNode& robot,string& linkName,unsigned int& counter,string indent=""){
		//cout << indent << linkName ;
		//XMLNode xnLink=robot("link","name",linkName);
		//if(xnLink.hasChild("visual")){
		//	string fileName=split(xnLink("visual")("geometry")("mesh").getAttribute("filename"),':')[1];
		//	cout << ":"<<fileName<<endl;
		//}
		//else cout<<endl;
		XMLNode& xnJoint=robot("joint","name",getChildJoints(robot,linkName)[0]);
		Mat m=getMat(xnJoint);
		//Vector3D axis=split_Vector3D(xnJoint("axis").getAttribute("xyz"));
		Joint joint(counter,linkName,m);
		if(indent.size()>=5*2) return joint;
		for(string sParent:getParentJoints(robot,linkName)){
			XMLNode& xnJoint=robot("joint","name",sParent);
			string linkChildName=xnJoint("child").getAttribute("link");
			Joint jointChild=createJoint(robot,linkChildName,++counter,indent+"  ");
			joint.addChild(jointChild);
		}
		return joint;
	}
	XMLNode& getJointParent(XMLNode& robot,XMLNode& joint){
		string parnetLinkName=joint("parent").getAttribute("link");
		vector<string> lj=getChildJoints(robot,parnetLinkName);
		assert(lj.size()==1);
		string parentJointName=lj[0];
		XMLNode& jointParent=robot("joint","name",parentJointName);
		return jointParent;
	}
void printJoint(XMLNode& xn){
		string name=xn.getAttribute("name");
		string type=xn.getAttribute("type");
		cout << "    "<<xn.getAttribute("name") <<": " <<xn.getAttribute("type")<< endl << "        ";
		if(type=="revolute" || type=="continuous" || type=="fixed"){
			if(xn.hasChild("parent"))
				cout << "[" << xn("parent").getAttribute("link") << "->" << xn("child").getAttribute("link") << "]";
			if(xn.hasChild("origin"))
				cout << " = " << xn("origin").getAttribute("rpy")<<","<< xn("origin").getAttribute("xyz");
			if(xn.hasChild("axis"))
				cout <<" , "<< xn("axis").getAttribute("xyz");
		}
		cout<<endl;
	}
    Mat getMat(XMLNode& joint){
    	if(joint.hasChild("origin")){
    		string rpy=joint("origin").getAttribute("rpy");
    		string xyz=joint("origin").getAttribute("xyz");
    		Vector3D pos=split_Vector3D(xyz);
    		Vector3D theta=split_Vector3D(rpy);
    		Mat m=posEulerAnglesToTransformationMatrix<float>(pos,theta*RAD2DEG);
    		return m;
    	}
    	else throw runtime_error("Joint "+joint.getAttribute("name")+" doesn't have attribute 'origin' in getMat()");
    }
	//TestURDFloader::init()
	void init(){
		Camera& cam   =getCamera();
		Stage&  stage =getStage();
		cam.setPos(Vector3D(0,0,10));
		Light* lightFront=new Light(Vector3D(150.0,150.0, 100.0));
		stage.add(lightFront);
		stage.add(new Light(Vector3D(-100.0,150.0, -150.0)));

		ax=new Axis(Vector3D(0.5,0.5,0.5));
		stage.add(ax);
		lookAtAxis=new Axis();
		lookAtAxis->setName(Text("LookAtAxis"));
		//stage.add(lookAtAxis);
		//Floor
		texGround.init();
		//Mat imgGround=imread("brick_pavement_0077_01_preview.jpg");
		Mat imgGround=imread("brown_brick_texture_map.jpg");
		texGround.setImage(imgGround);
		float l=10;
		Rectangle* r=new Rectangle(Vector3D(-l,0,-l*2),Vector3D(-l,0,l*2),Vector3D(l,0,l*2),Vector3D(l,0,-l*2));
		r->setTextura(texGround);
		r->setNU(1);
		r->setNV(1);
	    //stage.add(r);

	    XMLNode urdf;
	    ifstream ifdae("pepper.urdf");
	    ifdae >> urdf;

	    string jointRootLinkName="torso";
	    unsigned int counter=0;
	    Joint jointRoot=createJoint(urdf("robot"),jointRootLinkName,counter);
	    //I DON'T KNOW WHY !!!!!!!!!!!!!!!!!!!!!!!!!!!!
	    for(Joint& j:jointRoot.getChildren()) j.setParent(&jointRoot);
	    cout << jointRoot << endl;
	    Mat I=Mat::eye(4,4,CV_32F);
	    Mat_<float> TI = (Mat_<float>(4,4) <<
	               1,    0,    0, 0,
	               0,    0,    1, 0,
	               0,   -1,    0, 0,
	               0,    0,    0, 1);

	    jointRoot.calcInverseBindTransform(TI);


	    SolidArticulated* sa=new SolidArticulated();
	    sa->setJointsRoot(jointRoot);
	    sa->setJointNames(jointRoot.getJointNames());
	    sa->setPose(jointRoot.getLocalBindPose());
	    //stage.add(sa);

	    //Joint& j=jointRoot.getJointByName("RThumb2_link");
	    Joint& j=jointRoot.getJointByName("r_wrist");
	    JointPtr pj=&j;
	    while(!pj->isRoot()){
	    	cout << pj->getName() << "->";
	    	pj=pj->getParentPtr();
	    }
    	cout << pj->getName() <<  endl;
	    Joint& jbf=jointRoot.getJointByName("base_footprint");
	    pj=&jbf;
	    while(!pj->isRoot()){
	    	cout << pj->getName() << "->";
	    	pj=pj->getParentPtr();
	    }
    	cout << pj->getName() <<  endl;
	    //string package="/opt/ros/kinetic/share/pepper_meshes/";
	    string package=".";
	    string folder="/meshes/1.0/";
	    //string name="HeadPitch.dae";
	    //string name="HeadYaw.dae";
	    //string name="HipPitch.dae";
	    string name="KneePitch.dae";
	    //string name="Torso.dae";
	    //string name="HipRoll.dae";
	    //string name="LShoulderRoll.dae";
	    //string name="LShoulderPitch.dae";
/*	    Loader::model_base_path=package;
	    LoaderDAE ldae(name,0,folder);
	    ldae.loadNodeRootFromFileName(package+folder+name);
	    ldae.loadGeometry();

	    XMLNode lg=ldae.getNodeLibraryGeometries()["geometry"][0]("mesh");
	    cout << "geom size=" << lg["polylist"].size() << endl;
*/
	    shaderProgram.compileFromFileNames("src/SimpleMeshVertex.glsl",
				                           "src/SimpleMeshFragment.glsl");
		shaderProgram.start();
	    shaderProgram["lightPosition"]=Vector3D(100,100,100);
	    shaderProgram["lightColor"]   =Vector3D(1,1,1);
	    shaderProgram.stop();
        addShader(&shaderProgram);
        //ModelMesh mm=ldae.getModel().buildShaderReadyMeshModel();
        //ModelMesh mm=loadModelMesh(name);
        //mvao.init(mm);
        //mvao.setMaterial("meshes/1.0/juliette_face.png");
        //stage.add(&mvao);
	    /*
	    cout << "*************** JOINT ***************"<<endl;
	    vector<Joint> joints;
	    for(XMLNode &xn:urdf("robot")["joint"]){
	    	if(xn.hasChild("origin")){
		    	Mat m=getMat(xn);
		    	//not using VAO idx that is the parameter 1
		    	Joint j(1,xn.getAttribute("name"),m);
		    	joints.push_back(j);
		    	printJoint(xn);
	    	}
	    }*/
	    cout << "*************** LINK ***************"<<endl;
	    for(XMLNode &xn:urdf("robot")["link"]) try {
	    	string name=xn.getAttribute("name");
	    	//cout << name << endl;
	    	//for(string& s:getChildJoints(urdf("robot"),name)){
	    		//cout << "    " << s << endl;
	    		//printJoint(urdf("robot")("joint","name",s));
	    	//}
	    	//cout << "    " << xn("visual")("geometry")("mesh").getAttribute("filename") << endl;
	    	if(xn.hasChild("visual") /*&& name[0]=='L'*/){
	    		string fileName=split(xn("visual")("geometry")("mesh").getAttribute("filename"),':')[1];
	    		fileName=ltrim(fileName,"/");
	    		fileName=split(fileName,'/').back();
	    		//cout << "    " << "fileName= "<< fileName <<endl;
	    		SolidVAOPtr pvao=loadSolidVAO(fileName,0.001);
	    		Joint& joint=jointRoot.getJointByName(name);
	    		Mat bt=joint.getBindTransform();
   				bt.convertTo(bt, CV_64F);
	    		pvao->setTransformationMat(bt);
	    		pvao->hazFija();
	    		stage.add(pvao);
	    	}
	    	if(!xn.hasChild("visual")){
	    		AxisPtr paxis=new Axis(Vector3D(0.05,0.05,0.05));
	    		Joint& joint=jointRoot.getJointByName(name);
	    		Mat bt=joint.getBindTransform();
   				bt.convertTo(bt, CV_64F);
	    		paxis->setTransformationMat(bt);
	    		stage.add(paxis);
	    	}
	    }
	    catch(...){}
	}
	void update(double dt){
		if (waitKey(1) == 27) exit(0);
	}
	SolidVAOPtr loadSolidVAO(string fileName,float scale=0.1){
		SolidVAOPtr pvao=new SolidVAO(&shaderProgram);
		SolidVAO& vao=*pvao;
        ModelMesh mm=loadModelMesh(fileName,scale);
        vao.init(mm);
        vao.setMaterial("meshes/1.0/juliette_face.png");
        return pvao;
	}
	ModelMesh loadModelMesh(string fileName,float scale=0.1){
	    Mat_<double> TI = (Mat_<double>(4,4) <<
	               1,    0,    0, 0,
	               0,    0,    1, 0,
	               0,   -1,    0, 0,
	               0,    0,    0, 1);
	    string package=".";
	    string folder="/meshes/1.0/";
	    Loader::model_base_path=package;
	    LoaderDAE ldae(fileName,0,folder);
	    ldae.loadNodeRootFromFileName(package+folder+fileName);
	    ldae.loadGeometry();
        ModelMesh mm=ldae.getModel();
        mm.doScale(scale);
        //mm.transform(TI);
        mm=mm.buildShaderReadyMeshModel();
        return mm;
	}
};

TestURDFloader testURDFloader("cPATestURDFloader");

int main(int argc, char** argv) try{
	srand(10);
	GameEngine::setGame(&testURDFloader);
	GameEngine::gameInit(argc,argv);
	GameEngine::gameMainLoop();
	return 0;
}
catch (exception &e){
	cout << "Error in cPATestURDFloader::main(): "<<e.what() <<endl;
	return 1;
}
