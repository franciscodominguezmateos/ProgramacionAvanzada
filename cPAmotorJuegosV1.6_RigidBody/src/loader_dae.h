/*
 * loader_dae.h
 *
 *  Created on: 27 Mar 2020
 *      Author: Francisco Dominguez
 */
#pragma once
#include "loader.h"
#include "xml_parser.h"
#include "model_mesh_articulated.h"

class LoaderDAE: public Loader {
	int idxGeo;
public:
	LoaderDAE(string name,int idx=0):Loader(name),idxGeo(idx){}
	void load(){
		//ifstream ifdae("model.dae");
		string fileName=getName();
		cout << "fileName="<<fileName <<endl;
		ifstream ifdae(fileName);
	  XMLNode root;
	  ifdae >> root;

	  // COLLADA FILE FORMAT
	  //Geometry loader
	  XMLNode &collada=root("COLLADA");
	  XMLNode &library_geometries   =collada("library_geometries");
	  XMLNode &library_controllers  =collada("library_controllers");
	  XMLNode &library_visual_scenes=collada("library_visual_scenes");

	  //for the moment this only load the idxGeo geometry
	  XMLNode &geometry=library_geometries["geometry"][idxGeo];
	  XMLNode &mesh=geometry("mesh");
	  loadVertices(mesh);
	  loadNormals (mesh);
	  loadTextures(mesh);
	  loadIndixes (mesh);

	  XMLNode &skin=library_controllers("controller")("skin");
	  loadSkin(skin);

	  XMLNode &armature=library_visual_scenes("visual_scene")("node","id","Armature");
	  XMLNode &torso=armature("node","id","Torso");
	  //cout << torso <<endl;
	  Joint jointsRoot=loadJoints(torso);
	  cout <<"********************************************** DAE *********************"<< endl;
	  jointsRoot.calcInverseBindTransform(Mat::eye(4,4,CV_32F));
	  setJointsRoot(jointsRoot);

	  // SkeletonLoader
	  for(auto &pair:collada.getChildren()){
		  cout << pair.first << endl;
	  }
	  cout << "END"<<endl;
	}
	void loadSkin(XMLNode &skin){
		  XMLNode &vertex_weights=skin("vertex_weights");

		  XMLNode &iJoint=vertex_weights("input","semantic","JOINT");
		  vector<string>jointNames=colladaSourceStrings(iJoint,skin,"Name_array");
		  setJointNames(jointNames);

		  XMLNode &iWeight=vertex_weights("input","semantic","WEIGHT");
	      vector<GLfloat> weights=colladaSourceNumbers<GLfloat>(iWeight,skin,"float_array");

	      vector<GLint> counts=split_numbers<GLint>(vertex_weights("vcount").getText());
	      vector<GLint> data  =split_numbers<GLint>(vertex_weights("v").getText());
	      unsigned int idx=0;
	      for(int &c:counts)
	    	  for(unsigned i=0;i<c;i++){
	    		  GLint jointID =data[idx++];
	    		  GLint weightId=data[idx++];
	    		  GLfloat weight=weights[weightId];
	    		  addJoint(jointID);
	    		  addWeight(weight);
	    	  }
	}
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
		Mat m=Mat(4,4,CV_32F, vf.data());
		cout << "--------->"<<jointNode.getAttribute("id") <<endl;
		cout << m <<endl;
		return Joint(idx,name,m);
	}
	void loadVertices(XMLNode &mesh){
		XMLNode &vertices=mesh("vertices");
		XMLNode &iPosition=vertices("input","semantic","POSITION");
		vector<GLfloat> vf=colladaSourceNumbers<GLfloat>(iPosition,mesh,"float_array");
		setVerticesFromFloats(vf);
	}
	void loadNormals(XMLNode &mesh){
        XMLNode &polylist=mesh("polylist");
		XMLNode &iNormal=polylist("input","semantic","NORMAL");
		vector<GLfloat> vn=colladaSourceNumbers<GLfloat>(iNormal,mesh,"float_array");
		setNormalsFromFloats(vn);
	}
	void loadTextures(XMLNode &mesh){
		XMLNode &polylist=mesh("polylist");
		XMLNode &iTextcoord=polylist("input","semantic","TEXCOORD");
		vector<GLfloat> vt=colladaSourceNumbers<GLfloat>(iTextcoord,mesh,"float_array");
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
			  vector<string> vs=split(text);
			  if(sourceNode.hasAttribute("count")){
				  int count=sourceNode.getAttributeInt("count");
				  if(count!=vs.size())
					  runtime_error("Error LoaderDAE::colladaSourceStrings() vector size doesn't seem to be correct");
			  }
			  return vs;
		}
	template<class T>
	vector<T> colladaSourceNumbers(XMLNode &nodeID,XMLNode &nodeData,string source){
		// find in nodeID a source id
		  string dataSourceID=nodeID.getAttribute("source");
		  ltrim(dataSourceID,"#");
		  // in nodeData there is a source node with id dataSourceID then get the source node
		  XMLNode &sourceNode=nodeData("source","id",dataSourceID)(source);
		  string &text=sourceNode.getText();
		  vector<T> vf=split_numbers<T>(text);
		  if(sourceNode.hasAttribute("count")){
			  int count=sourceNode.getAttributeInt("count");
			  if(count!=vf.size())
				  runtime_error("Error LoaderDAE::colladaSourceNumbers() vector size doesn't seem to be correct");
		  }
		  return vf;
	}
};

