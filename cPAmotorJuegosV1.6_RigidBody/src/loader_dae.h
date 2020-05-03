/*
 * loader_dae.h
 *
 *  Created on: 27 Mar 2020
 *      Author: Francisco Dominguez
 */
#pragma once
#include "loader.h"
#include "xml_parser.h"
#include "model_joint.h"
#include "model_mesh_articulated.h"
#include "animation_skeleton.h"

class LoaderDAE: public Loader {
	int idxGeo;
	AnimationSkeleton animSkeleton;
public:
	LoaderDAE(string name,int idx=0):Loader(name),idxGeo(idx){}
	AnimationSkeleton &getAnimationSkeleton(){return animSkeleton;}
	void load(){

		//ifstream ifdae("model.dae");
		string fileName=getName();
		cout << "fileName="<<fileName <<endl;
		ifstream ifdae(fileName.c_str());
	    XMLNode root;
	    ifdae >> root;

	  // COLLADA FILE FORMAT
	  //Geometry loader
	  XMLNode &collada=root("COLLADA");
	  XMLNode &library_geometries   =collada("library_geometries");
	  XMLNode &library_controllers  =collada("library_controllers");
	  XMLNode &library_animations   =collada("library_animations");
	  XMLNode &library_visual_scenes=collada("library_visual_scenes");

	  //for the moment this only load the idxGeo geometry
	  XMLNode &geometry=library_geometries["geometry"][idxGeo];
	  XMLNode &mesh=geometry("mesh");
	  loadVertices(mesh);
	  loadNormals (mesh);
	  loadTextures(mesh);
	  loadIndixes (mesh);

	  //Load skin
	  XMLNode &skin=library_controllers("controller")("skin");
	  cout << "skin"<<endl;
	  loadSkin(skin);

	  //Load joints
	  Joint jointsRoot;
	  //TODO: fix this trick, if dae it mean from blender and z-up, if xml mean y-up
	  if(getExtension(getName())=="dae"){
		  XMLNode &armature=library_visual_scenes("visual_scene")("node","id","Armature");
		  XMLNode &torso=armature("node","id","Torso");
		  //cout << torso <<endl;
		  jointsRoot=loadJoints(torso);
	  }
	  else{
		  XMLNode &hips=library_visual_scenes("visual_scene")("node","id","Hips");
		  jointsRoot=loadJoints(hips);
	  }
	  cout <<"********************************************** DAE *********************"<< endl;
	  //Mat I=Mat::eye(4,4,CV_32F);
	  //jointsRoot.calcInverseBindTransform(I);
	  setJointsRoot(jointsRoot);

	  //Load Animations
	  loadJointAnimations(library_animations);

	  // SkeletonLoader
	  for(auto &pair:collada.getChildren()){
		  cout << pair.first << endl;
	  }
	  cout << "END COLLADA"<<endl;
	}
	/*                          A N I M A T I O N S                    */
	void loadJointAnimations(XMLNode &animations){
		animSkeleton.setJointNames(getJointNames());
		for(XMLNode &n:animations["animation"])
			loadJointAnimation(n);
	}
	void loadJointAnimation(XMLNode &animation){
		XMLNode &sampler=animation("sampler");
		XMLNode &channel=animation("channel");
		string jointName=getJointName(channel);
		XMLNode &iInput=sampler("input","semantic","INPUT");
		vector<GLfloat> times=colladaSourceNumbers(iInput,animation);
		cout << "time.size()="<<times.size()<<endl;
		XMLNode &iOutput=sampler("input","semantic","OUTPUT");
		vector<Mat> localTransformations=colladaSourceMatrices4x4(iOutput,animation);
		assert(times.size()==localTransformations.size());
		AnimationJoint kfJoint(jointName);
		for(unsigned int i=0;i<times.size();i++)
			kfJoint.addKeyFrameJoint(KeyFrameJoint(times[i],localTransformations[i]));
		animSkeleton.addKeyFramesJoint(kfJoint);
	}
	string getJointName(XMLNode &channel){
		string target=channel.getAttribute("target");
		vector<string> vs=split(target,'/');
		assert(vs.size()==2);
		return vs[0];
	}
	/*                         S K I N                */
	void loadSkin(XMLNode &skin){
		cout<<"loadSkin"<<endl;
		XMLNode &vertex_weights=skin("vertex_weights");
		XMLNode &iJoint=vertex_weights("input","semantic","JOINT");
		vector<string>jointNames=colladaSourceStrings(iJoint,skin,"Name_array");
		setJointNames(jointNames);

		XMLNode &joints=skin("joints");
		XMLNode &iInvBindMat=joints("input","semantic","INV_BIND_MATRIX");
		vector<Mat> invBindMats=colladaSourceMatrices4x4(iInvBindMat,skin);
		//cout<<"ibm 65="<<invBindMats.size()<<endl;
		setInverseBindTransforms(invBindMats);

		XMLNode &iWeight=vertex_weights("input","semantic","WEIGHT");
	    vector<GLfloat> weights=colladaSourceNumbers(iWeight,skin);

	    vector<GLint> counts=split_numbers<GLint>(vertex_weights("vcount").getText());
	    vector<GLint> data  =split_numbers<GLint>(vertex_weights("v").getText());
	    unsigned int idx=0;
	    for(int &c:counts){
	    	VertexSkinData vskin;
	    	for(int j=0;j<c;j++){
	    		GLint jointID =data[idx++];
	    		GLint weightId=data[idx++];
	    		GLfloat weight=weights[weightId];
	    		vskin.addJointEffect(jointID,weight);
	    	}
	    	//cout << vskin;
	        addVertexSkinData(vskin);
	    }
	}
	/*                          J O I N T S                      */
	Joint loadJoints(XMLNode &jointNode){
		Joint joint=getJoint(jointNode);
		if(jointNode.hasChild("node")){
			vector<XMLNode> &vn=jointNode["node"];
			for(XMLNode &n:vn)
				joint.addChild(loadJoints(n));
		}
		return joint;
	}
	Joint getJoint(XMLNode &jointNode){
		string name=jointNode.getAttribute("id");
		int idx=getJointIdx(name);
		string matrixTxt=jointNode("matrix").getText();
		vector<GLfloat> vf=split_numbers<GLfloat>(matrixTxt);
		assert(vf.size()==16);
		//this constructor doesn't copy data from vf.data()
		Mat m=Mat(4,4,CV_32F, vf.data());
		//cout << "--------->"<<jointNode.getAttribute("id") <<endl;
		//cout << m <<endl;
		return Joint(idx,name,m);
	}
	/*                       M E S H                             */
	void loadVertices(XMLNode &mesh){
		XMLNode &vertices=mesh("vertices");
		XMLNode &iPosition=vertices("input","semantic","POSITION");
		vector<GLfloat> vf=colladaSourceNumbers(iPosition,mesh);
		setVerticesFromFloats(vf);
	}
	void loadNormals(XMLNode &mesh){
        XMLNode &polylist=mesh("polylist");
		XMLNode &iNormal=polylist("input","semantic","NORMAL");
		vector<GLfloat> vn=colladaSourceNumbers(iNormal,mesh);
		setNormalsFromFloats(vn);
	}
	void loadTextures(XMLNode &mesh){
		XMLNode &polylist=mesh("polylist");
		XMLNode &iTextcoord=polylist("input","semantic","TEXCOORD");
		vector<GLfloat> vt=colladaSourceNumbers(iTextcoord,mesh);
		setTexturesFromFloats(vt);
	}
	void loadIndixes(XMLNode &mesh){
		  XMLNode &polylist=mesh("polylist");
		  int typeCount=polylist["input"].size();
		  XMLNode &p=polylist("p");
		  vector<GLuint> vi=split_numbers<GLuint>(p.getText());
		  for(unsigned int i=0;i<vi.size()/typeCount;i++){
			  GLuint iv=vi[i*typeCount+0];
			  GLuint in=vi[i*typeCount+1];
			  GLuint it=vi[i*typeCount+2];
			  addVTNindex(iv,it,in);
		  }
		  //TODO: check count
	}
	/*                   C O L A D A   U T I L                   */
	string &colladaSourceData(XMLNode &nodeID,XMLNode &nodeData,string source){
		// find in nodeID a source id
		  string dataSourceID=nodeID.getAttribute("source");
		  ltrim(dataSourceID,"#");
		  // in nodeData there is a source node with id dataSourceID then get the source node
		  XMLNode &sourceNode=nodeData("source","id",dataSourceID)(source);
		  string &text=sourceNode.getText();
		  return text;
	}
	vector<string> colladaSourceStrings(XMLNode &nodeID,XMLNode &nodeData,string source){
			// find in nodeID a source id
			  string dataSourceID=nodeID.getAttribute("source");
			  ltrim(dataSourceID,"#");
			  // in nodeData there is a source node with id dataSourceID then get the source node
			  XMLNode &sourceNode=nodeData("source","id",dataSourceID)(source);
			  string &text=sourceNode.getText();
			  replaceChars(text);
			  vector<string> vs=split(text);
			  if(sourceNode.hasAttribute("count")){
				  unsigned int count=sourceNode.getAttributeInt("count");
				  if(count!=vs.size())
					  runtime_error("Error LoaderDAE::colladaSourceStrings() vector size doesn't seem to be correct");
			  }
			  return vs;
		}
	template<class T=GLfloat>
	vector<T> colladaSourceNumbers(XMLNode &nodeID,XMLNode &nodeData,string source="float_array"){
		// find in nodeID a source id
		  string dataSourceID=nodeID.getAttribute("source");
		  ltrim(dataSourceID,"#");
		  // in nodeData there is a source node with id dataSourceID then get the source node
		  XMLNode &sourceNode=nodeData("source","id",dataSourceID)(source);
		  string &text=sourceNode.getText();
		  replaceChars(text);
		  vector<T> vf=split_numbers<T>(text);
		  if(sourceNode.hasAttribute("count")){
			  unsigned int count=sourceNode.getAttributeInt("count");
			  if(count!=vf.size())
				  runtime_error("Error LoaderDAE::colladaSourceNumbers() vector size doesn't seem to be correct");
		  }
		  return vf;
	}
	vector<Mat> colladaSourceMatrices4x4(XMLNode &nodeID,XMLNode &nodeData,string source="float_array"){
		vector<Mat> vm;
		vector<GLfloat> vf=colladaSourceNumbers<GLfloat>(nodeID,nodeData,source);
		for(unsigned int i=0;i<vf.size();i+=16){
			//this constructor doesn't copy data from vf.data()
			//that is the reason we call m.clone()
			Mat m=Mat(4,4,CV_32F, vf.data()+i);
			vm.push_back(m.clone());
		}
		return vm;
	}
};

