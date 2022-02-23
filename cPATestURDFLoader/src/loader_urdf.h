/*
 * loader_urdf.h
 *
 *  Created on: 30 Jan 2022
 *      Author: Francisco Dominguez
 */

#pragma once
#include <vector>
#include <string>

#include "xml_parser.h"
#include "loader_dae.h"
#include "solid_vao.h"

using namespace std;

class JointURDFdata{
	Vector3D axis;
	float limit_lower;
	float limit_upper;
public:

};
class SolidURDF: public Solid{
public:
	Joint joints;
	map<string,SolidPtr> solids;
	map<string,JointURDFdata> urdfData;

};
class LoaderURDF: public Loader{
    XMLNode urdf;
    //one solid for each joint
    //map<string,SolidPtr> solids;
    SolidURDF solidURDF;
public:
    // NONONONONO BUT....
    GLSLShaderProgram shaderProgram;
    LoaderURDF():Loader("",""){}
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
			}
    		catch(exception& e){
    			cout << "In LoaderURDF::getParentJoints: "<< e.what()<<endl;
    		}
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
			}
    		catch(exception& e){
    			cout << "In LoaderURDF::getChildJoints: "<< e.what()<<endl;
    		}
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
    		Vector3D posZ(pos.getX(),pos.getZ(),pos.getY());
    		Vector3D thetaZ(theta.getX(),theta.getZ(),theta.getY());
    		Mat m=posEulerAnglesToTransformationMatrix<float>(posZ,thetaZ*RAD2DEG);
    		return m;
    	}
    	else throw runtime_error("Joint "+joint.getAttribute("name")+" doesn't have attribute 'origin' in getMat()");
    }
	SolidVAOPtr loadSolidVAO(string fileName,float scale=0.1){
		SolidVAOPtr pvao=new SolidVAO(&shaderProgram);
		SolidVAO& vao=*pvao;
        ModelMesh mm=loadModelMesh(fileName,scale);
        vao.init(mm);
        vao.setMaterial("/opt/ros/kinetic/share/pepper_meshes/meshes/1.0/juliette_face.png");
        return pvao;
	}
	ModelMesh loadModelMesh(string fileName,float scale=0.1){
	    string package="/opt/ros/kinetic/share/pepper_meshes";
	    string folder="/meshes/1.0/";
	    Loader::model_base_path=package;
	    LoaderDAE ldae(fileName,0,folder);
	    ldae.loadNodeRootFromFileName(package+folder+fileName);
	    ldae.loadGeometry();
        ModelMesh mm=ldae.getModel();
        Mat_<double> T=(Mat_<double>(4,4) <<
                1, 0, 0, 0,
                0, 0, 1, 0,
                0, 1, 0, 0,
                0, 0, 0, 1);
        mm.transform(T);
        mm.doScale(scale);
        mm=mm.buildShaderReadyMeshModel();
        return mm;
	}
	void load(){}
    void init(Stage& stage){
	    ifstream ifdae("pepper.urdf");
	    ifdae >> urdf;

	    shaderProgram.compileFromFileNames("src/SimpleMeshVertex.glsl",
				                           "src/SimpleMeshFragment.glsl");
		shaderProgram.start();
	    shaderProgram["lightPosition"]=Vector3D(100,100,100);
	    shaderProgram["lightColor"]   =Vector3D(1,1,1);
	    shaderProgram.stop();

	    string jointRootLinkName="torso";
	    unsigned int counter=0;
	    solidURDF.joints=createJoint(urdf("robot"),jointRootLinkName,counter);
	    Joint&  jointRoot=solidURDF.joints;
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
	    	if(xn.hasChild("visual") /*&& name[0]=='L'*/ ){
	    		XMLNode& vgm=xn("visual")("geometry")("mesh");
	    		string fileName=split(vgm.getAttribute("filename"),':')[1];
	    		//Apply scale to the mesh only the X axis
	    		float scale=1;
	    		if(vgm.hasAttribute("scale")){
	    			string sscale=vgm.getAttribute("scale");
	    			Vector3D vscale=split_Vector3D(sscale);
	    			scale=vscale.getX();
	    		}
	    		fileName=ltrim(fileName,"/");
	    		fileName=split(fileName,'/').back();
	    		//cout << "    " << "fileName= "<< fileName <<endl;
	    		SolidVAOPtr pvao=loadSolidVAO(fileName,0.01*scale);
	    		Joint& joint=jointRoot.getJointByName(name);
	    		Mat bt=joint.getBindTransform();
   				bt.convertTo(bt, CV_64F);
	    		pvao->setTransformationMat(bt);
	    		pvao->hazFija();
	    		stage.add(pvao);
	    		solidURDF.solids[name]=pvao;
	    		AxisPtr paxis=new Axis(Vector3D(0.05,0.05,0.05));
	    		paxis->setName(name);
	    		paxis->setTransformationMat(bt);
	    		stage.add(paxis);
	    	}
	    	if(!xn.hasChild("visual")){
	    		AxisPtr paxis=new Axis(Vector3D(0.05,0.05,0.05));
	    		paxis->setName(name);
	    		Joint& joint=jointRoot.getJointByName(name);
	    		Mat bt=joint.getBindTransform();
   				bt.convertTo(bt, CV_64F);
	    		paxis->setTransformationMat(bt);
	    		stage.add(paxis);
	    	}
	    }
	    catch(exception& e){
	    	cout <<"In LoaderURDF::init: "<<  e.what()<<endl;
	    }
    }
};



