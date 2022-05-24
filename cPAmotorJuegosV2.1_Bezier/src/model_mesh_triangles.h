/*
 * model_mesh_triangles.h
 *
 *  Created on: 22 Oct 2021
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

/**
 * This is a generic model of a triangle mesh
 */
class ModelMeshTriangles{
protected:
	vector<Triangle> triangles;
	vector<string>   triangleMaterialNames;
	map<string,Material> materials;
	float maxX,maxY,maxZ;
	float minX,minY,minZ;
public:
	void clear(){
		triangles.clear();
		triangleMaterialNames.clear();
		materials.clear();
		maxX=maxY=maxZ=1e-12;
		minX=minY=minZ=1e+12;
	}
	inline void addMaterial(string name,Material &m){materials[name]=m;}
	inline void addTriangle(Triangle &t){triangles.push_back(t);}
	inline void addTriangleMaterialName(string s){triangleMaterialNames.push_back(s);}
	inline vector<Triangle>& getTriangles()     {return triangles;}
	inline map<string, Material>& getMaterials(){return materials;}
	inline Material              &getMaterial(string n){return materials[n];}
	inline vector<string>         getMaterialNames(){
		vector<string> vs;
		for(pair<string,Material> pm:materials)
			vs.push_back(pm.first);
		return vs;
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




