/*
 * material.h
 *
 *  Created on: Dic 11, 2018
 *      Author: Francisco Dominguez
 *  TODO: it is .obj dependent
 */
#pragma once
#include "util.h"
#include "texture.h"
#include "vector3d.h"

class Material{
	double Ns;
	double Ni;
	double d;
	double Tr;
	Vector3D Tf;
	double illum;
	Vector3D Ka;
	Vector3D Kd;
	Vector3D Ks;
	Vector3D Ke;
	string map_Ka;
	Texture* map_Ka_tex;
	string map_Kd;
	Texture* map_Kd_tex;
	string path;
	string name;
public:
	Material(string path="",string name=""):map_Ka_tex(nullptr),map_Kd_tex(nullptr),path(path),name(name){}
	void parseLine(string linea){
		vector<string> vs=split(linea,' ');
		if(vs[0]=="Ns")
			Ns=stod(vs[1]);
		if(vs[0]=="Ni")
			Ni=stod(vs[1]);
		if(vs[0]=="d")
			d=stod(vs[1]);
		if(vs[0]=="Tr")
			Tr=stod(vs[1]);
		if(vs[0]=="Tf")
			Tf=Vector3D(stod(vs[1]),stod(vs[2]),stod(vs[3]));
		if(vs[0]=="illum")
			illum=stod(vs[1]);
		if(vs[0]=="Ka")
			Ka=Vector3D(stod(vs[1]),stod(vs[2]),stod(vs[3]));
		if(vs[0]=="Kd")
			Kd=Vector3D(stod(vs[1]),stod(vs[2]),stod(vs[3]));
		if(vs[0]=="Ks")
			Ks=Vector3D(stod(vs[1]),stod(vs[2]),stod(vs[3]));
		if(vs[0]=="Ke")
			Ke=Vector3D(stod(vs[1]),stod(vs[2]),stod(vs[3]));
		if(vs[0]=="map_Ka"){
			//string &su=vs[1];
			//transform(su.begin(), su.end(), su.begin(), ::tolower);
			map_Ka=vs[1];
			map_Ka_tex=new Texture();
			map_Ka_tex->init();
			Mat img=imread(path+map_Ka,IMREAD_UNCHANGED);
			map_Ka_tex->setImage(img);
		}
		if(vs[0]=="map_Kd"){
			//string &su=vs[1];
			//transform(su.begin(), su.end(), su.begin(), ::tolower);
			//vs[1]=toLower(vs[1]);
			//cout <<"kkk="<< vs[1] << endl;
			map_Kd=vs[1];
			map_Kd_tex=new Texture();
			map_Kd_tex->init();
			Mat img=imread(path+map_Kd,IMREAD_UNCHANGED);
			map_Kd_tex->setImage(img);
		}
	}
	void bind(){
		if(map_Kd_tex) map_Kd_tex->bind();
		if(map_Ka_tex) map_Ka_tex->bind();
	}
	void unbind(){
		if(map_Kd_tex) map_Kd_tex->unbind();
		if(map_Ka_tex) map_Ka_tex->unbind();
	}
	double getD() const {return d;}
	void setD(double d) {this->d = d;}
	double getIllum() const {return illum;}
	void setIllum(double illum) {this->illum = illum;}
	const Vector3D& getKa() const {return Ka;}
	void setKa(const Vector3D& ka) {Ka = ka;}
	const Vector3D& getKd() const {return Kd;}
	void setKd(const Vector3D& kd) {Kd = kd;}
	const Vector3D& getKe() const {return Ke;}
	void setKe(const Vector3D& ke) {Ke = ke;}
	const Vector3D& getKs() const {return Ks;}
	void setKs(const Vector3D& ks) {Ks = ks;}
	const string& getMapKa() const {return map_Ka;}
	void setMapKa(const string& mapKa) {map_Ka = mapKa;}
	Texture* getMapKaTex() const {return map_Ka_tex;}
	void setMapKaTex(Texture* mapKaTex) {map_Ka_tex = mapKaTex;}
	const string& getMapKd() const {return map_Kd;}
	void setMapKd(const string& mapKd) {map_Kd = mapKd;}
	Texture* getMapKdTex() const {return map_Kd_tex;}
	void setMapKdTex(Texture* mapKdTex) {map_Kd_tex = mapKdTex;	}
	double getNi() const {return Ni;}
	void setNi(double ni) {Ni = ni;}
	double getNs() const {return Ns;}
	void setNs(double ns) {Ns = ns;	}
	const Vector3D& getTf() const {return Tf;}
	void setTf(const Vector3D& tf) {Tf = tf;}
	double getTr() const {return Tr;}
	void setTr(double tr) {Tr = tr;}
};
