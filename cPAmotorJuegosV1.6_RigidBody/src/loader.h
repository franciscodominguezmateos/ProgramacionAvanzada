/*
 * loader.h
 *
 *  Created on: March 28, 2020
 *      Author: Francisco Dominguez
 */

#pragma once
#include <stdio.h>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <GL/glut.h>
#include "vector3d.h"
#include "util.h"
#include "material.h"
#include "triangle.h"
#include "model_mesh.h"
#include "model_mesh_articulated.h"

using namespace std;

class Loader {
	ModelMeshArticulated m;
	string name;
	string path;
public:
	static string model_base_path;

	Loader(string s,string folder=""){
		if(folder=="")
			folder=takeAwayExtension(s)+"/";
		path=model_base_path+folder;
		name=s;
		//load();
	}
	virtual void load()=0;
	inline string &getName(){return name;}
	inline ModelMesh &getModel(){return m;}
	inline ModelMeshArticulated &getModelArticulated(){return m;}
	inline vector<string> &getJointNames(){return m.getJointNames();}
	inline void setJointNames(vector<string> &jn){m.setJointNames(jn);}
	inline int getJointIdx(string name){return m.getJointIdx(name);}
	inline vector<Triangle>& getTriangles(){return m.getTriangles();}
	inline map<string, Material>& getMaterials() {return m.getMaterials();}
	inline vector<Vector3D>& getVertices() {return m.getVertices();}
	inline vector<Vector3D>& getTextures() {return m.getTextures();}
	inline vector<Vector3D>& getNormals()  {return m.getNormals();}
	inline void addVTNindex(unsigned int iv,unsigned int it,unsigned int in){m.addVTNindex(iv,it,in);}
	inline void addVertex (Vector3D &v){m.addVertex(v);}
	inline void addTexture(Vector3D &v){m.addTexture(v);}
	inline void addNormal (Vector3D &v){m.addNormal(v);}
	inline void addMaterial(string name,Material &mat){m.addMaterial(name,mat);}
	inline void addTriangle(Triangle &t){m.addTriangle(t);}
	inline void setVerticesFromFloats(vector<GLfloat> &vf){m.setVerticesFromFloats(vf);}
	inline void setTexturesFromFloats(vector<GLfloat> &vf){m.setTexturesFromFloats(vf);}
	inline void setNormalsFromFloats (vector<GLfloat> &vf){m.setNormalsFromFloats(vf) ;}
	inline void setJointsRoot(Joint j){m.setJointsRoot(j);}
	inline Joint &getJointsRoot(){return m.getJointsRoot();}
	inline void addVertexSkinData(VertexSkinData &vsd){m.addVertexSkinData(vsd);}
};
//must finish on slash '/'
string Loader::model_base_path="modelos/";
