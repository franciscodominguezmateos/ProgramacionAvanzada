/*
 * modelo_mesh.h
 *
 *  Created on: March 27, 2020
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

using namespace std;

/**
 * This is a generic model of a triangle mesh
 */
class ModelMesh {
	vector<Vector3D> vertices;
	vector<Vector3D> textures;
	//it should be Vector2D but....
	vector<Vector3D> normals;
	//theses three vector should have same size
	vector<unsigned int> ivertices;
	vector<unsigned int> itextures;
	vector<unsigned int> inormals;
	//this vector size is above ones divided by 3 since are triangles
	vector<string>       iMaterialNames;
	vector<Triangle> triangles;
	vector<string>   triangleMaterialNames;
	map<string,Material> materials;
	float maxX,maxY,maxZ;
	float minX,minY,minZ;
public:
	inline void addVTNindex(unsigned int iv,unsigned int it,unsigned int in){
		ivertices.push_back(iv);
		itextures.push_back(it);
		inormals.push_back(in);
	}
	inline void scale(GLfloat s){for(unsigned int i=0;i<vertices.size();i++) vertices[i]*=s;}
	inline void transform(Mat &T){
		for(Vector3D &v:vertices) v=T*v.asH ();
		for(Vector3D &n:normals ) n=T*n.asH0();
	}
	inline void addVertex (Vector3D &v){vertices.push_back(v);}
	inline void addTexture(Vector3D &v){textures.push_back(v);}
	inline void addNormal (Vector3D &v){normals.push_back(v);}
	inline void addMaterial(string name,Material &m){materials[name]=m;}
	inline void addTriangle(Triangle &t){triangles.push_back(t);}
	inline void addTriangleMaterialName(string s){triangleMaterialNames.push_back(s);}
	inline void addIdxMaterialName(string s){iMaterialNames.push_back(s);}
	inline void setVector(vector<Vector3D> &vv,vector<GLfloat> &vf,unsigned int stride=3){
		vector<double> va(stride);
		for(unsigned int i=0;i<vf.size()/stride;i++){
			for(unsigned int j=0;j<stride;j++)
				va[j]=vf[i*stride+j];
			vv.push_back(Vector3D(va));
		}
	}
	inline void setVerticesFromFloats(vector<GLfloat> &vf){setVector(vertices,vf  );}
	inline void setTexturesFromFloats(vector<GLfloat> &vf){setVector(textures,vf,2);}
	inline void setNormalsFromFloats (vector<GLfloat> &vf){setVector(normals ,vf  );}
	template<class T>
	inline vector<T> getVector(vector<Vector3D> &vv,int stride=3){
		vector<T> vf;
		for(Vector3D &v:vv){
			vf.push_back((T)v.getX());
			vf.push_back((T)v.getY());
			if(stride==3) vf.push_back((T)v.getZ());
		}
		return vf;
	}
	inline vector<GLfloat> getFloatFromVertices(){return getVector<GLfloat>(vertices  );}
	inline vector<GLfloat> getFloatFromTextures(){return getVector<GLfloat>(textures,2);}
	inline vector<GLfloat> getFloatFromNormals (){return getVector<GLfloat>(normals   );}
	inline vector<Vector3D>& getVertices() {return vertices;}
	inline vector<Vector3D>& getTextures() {return textures;}
	inline vector<Vector3D>& getNormals()  {return normals; }
	inline vector<unsigned int>& getIvertices() {return ivertices;}
	inline vector<unsigned int>& getInormals()  {return inormals; }
	inline vector<unsigned int>& getItextures() {return itextures;}
	inline vector<Triangle>& getTriangles()     {return triangles;}
	inline map<string, Material>& getMaterials(){return materials;}
	inline Material              &getMaterial(string n){return materials[n];}
	inline vector<string>         getMaterialNames(){
		vector<string> vs;
		for(pair<string,Material> pm:materials)
			vs.push_back(pm.first);
		return vs;
	}
	void buildTriangles(){
		//it is supose that all data is ready: vertices,textures,normals and indixes
		unsigned int stride=3; //since we are building triangles
		for(unsigned int i=0;i<ivertices.size()/stride;i+=stride){
			vector<unsigned int> vid0={ivertices[i*stride+0],itextures[i*stride+0],inormals[i*stride+0]};
			vector<unsigned int> vid1={ivertices[i*stride+1],itextures[i*stride+1],inormals[i*stride+1]};
			vector<unsigned int> vid2={ivertices[i*stride+2],itextures[i*stride+2],inormals[i*stride+2]};
			Vector3D &p0=getVertices()[vid0[0]];
			Vector3D &p1=getVertices()[vid1[0]];
			Vector3D &p2=getVertices()[vid2[0]];
			Triangle t(p0,p1,p2);
			Vector3D &t0=getTextures()[vid0[1]];
			Vector3D &t1=getTextures()[vid1[1]];
			Vector3D &t2=getTextures()[vid2[1]];
			t.setT0(t0);
			t.setT1(t1);
			t.setT2(t2);
			Vector3D &n0=getNormals()[vid0[2]];
			Vector3D &n1=getNormals()[vid1[2]];
			Vector3D &n2=getNormals()[vid2[2]];
			t.setN0(n0);
			t.setN1(n1);
			t.setN2(n2);
			string &currentMaterial=triangleMaterialNames[i];
			Texture* tex=getMaterial(currentMaterial).getMapKdTex();
			t.setTextura(tex);
			addTriangle(t);
		}
	}
	ModelMesh buildShaderReadyMeshModel(){
		ModelMesh m;
		m.ivertices=ivertices;
		m.materials=materials;
		m.iMaterialNames=iMaterialNames;
		m.vertices=vertices;
		m.textures=vector<Vector3D>(vertices.size());
		m.normals =vector<Vector3D>(vertices.size());
		for(unsigned int i=0;i<ivertices.size();i++){
			unsigned int iv=ivertices[i];
			unsigned int it=itextures[i];
			unsigned int in=inormals [i];
			m.textures[iv]=textures[it];
			m.normals [iv]=normals [in];
		}
		return m;
	}
	vector<unsigned int> getIverticesMaterial(string materialName){
		vector<unsigned int> idxV;
		for(unsigned int i=0;i<ivertices.size();i++){
			int j=i/3;
			if(iMaterialNames[j]==materialName){
				unsigned int iv=ivertices[i];
				idxV.push_back(iv);
			}
		}
		return idxV;
	}
	void calculaExtremos(Vector3D &v){
		float x=v.getX();
		float y=v.getY();
		float z=v.getZ();
		maxX=fmax(maxX,x);
		maxY=fmax(maxY,y);
		maxZ=fmax(maxZ,z);
		minX=fmin(minX,x);
		minY=fmin(minY,y);
		minZ=fmin(minZ,z);
	}
	Triangle centrar(Triangle &t){
		Vector3D centro(minX+getAncho()/2.0,minY+getAlto()/2.0,minZ+getProfundo()/2);
		Vector3D p0=t.getP0();
		Vector3D p1=t.getP1();
		Vector3D p2=t.getP2();
		/// NNOOOOOOOOO
		return(Triangle(p0-centro,p1-centro,p2-centro));
	}
	inline float getAncho()    {return maxX-minX;}
	inline float getAlto()     {return maxY-minY;}
	inline float getProfundo() {return maxZ-minZ;}
	inline Vector3D getCentro(){return Vector3D(minX+getAncho()/2.0,minY+getAlto()/2.0,minZ+getProfundo()/2);}
	void computeMinMax(){
		for(Triangle &t:triangles){
			calculaExtremos(t.getP0());
			calculaExtremos(t.getP1());
			calculaExtremos(t.getP2());
		}
	}
	void doCentrar(){
		for(Triangle &t:triangles){
			t=centrar(t);
		}
	}
	void doScale(double s){
		for(Vector3D &v:vertices)
			v*=s;
		vector<Triangle> vt(triangles);
		triangles.clear();
		for(Triangle &t:vt) try{
			t.doScale(s);
			triangles.push_back(t);
		}
		catch(runtime_error &e){
			//cout << "triangulo muy pequeño en ModeloMaterial::doScale :"+string(e.what())<<endl;
		}
		minX*=s;
		minY*=s;
		minZ*=s;
		maxX*=s;
		maxY*=s;
		maxZ*=s;
	}
};
