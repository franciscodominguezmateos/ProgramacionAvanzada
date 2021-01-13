/*
 * modelo_material.h
 *
 *  Created on: Nov 12, 2015
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
//#include "contorno.h"
#include "util.h"
#include "material.h"
#include "triangle.h"

using namespace std;

class ModeloMaterial: public Solid {
	vector<Vector3D *> vertices;
	vector<Vector3D *> textures;
	vector<Vector3D *> normals;
	vector<Triangle *> triangulos;
	map<string,Material> materiales;
	string currentMaterial;
	float maxX,maxY,maxZ;
	float minX,minY,minZ;
	Vector3D scale;
	string name;
	string path;
public:
	static string model_base_path;
	ModeloMaterial(){}

	ModeloMaterial(string s,string folder=""){
		if(folder=="")
			folder=takeAwayExtension(s)+"/";
		path=model_base_path+folder;
		name=s;
		cargar();
		scale=Vector3D(1,1,1);
	}
	//ModeloMaterial(const ModeloMaterial &m);
	ModeloMaterial *clone(){return new ModeloMaterial(*this);}

	Triangle *centrar(Triangle *t){
		Vector3D centro(minX+getAncho()/2.0,minY+getAlto()/2.0,minZ+getProfundo()/2);
		Vector3D p0=t->getP0();
		Vector3D p1=t->getP1();
		Vector3D p2=t->getP2();
		/// NNOOOOOOOOO
		return(new Triangle(p0-centro,p1-centro,p2-centro));
	}
	inline float getAncho()    {return maxX-minX;}
	inline float getAlto()     {return maxY-minY;}
	inline float getProfundo() {return maxZ-minZ;}
	inline Vector3D getCentro(){return Vector3D(minX+getAncho()/2.0,minY+getAlto()/2.0,minZ+getProfundo()/2);}
	inline void setScale(Vector3D s){scale=s;}
	inline void setScale(double d){scale=Vector3D(d,d,d);}
	inline Vector3D getScale(){return scale;}
	inline float getMinY(){return minY;}
	void setTexture(Texture* tex){
		for(Triangle* t:triangulos)
			t->setTextura(tex);
	}
	inline void addTriangulo(Triangle *t){
		triangulos.push_back(t);
	}
	inline void setColor(Vector3D c){
		for(Triangle *t:triangulos){
			t->setCol(c);
		}
	}
	inline void setDrawNormals(bool b){
		for(Triangle* &t:triangulos)
			t->setDrawNormals(b);
	}
	inline void doScale(double s){
		vector<Triangle*> vt(triangulos);
		triangulos.clear();
		for(Triangle* &t:vt) try{
			t->doScale(s);
			triangulos.push_back(t);
		}
		catch(runtime_error &e){
			//cout << "triangulo muy pequeño en ModeloMaterial::doScale :"+string(e.what())<<endl;
			delete t;
		}
		minX*=s;
		minY*=s;
		minZ*=s;
		maxX*=s;
		maxY*=s;
		maxZ*=s;
	}
	inline void doTranslate(Vector3D v){
		for(Triangle* &t:triangulos)
			t->doTranslate(v);
		minX+=v.getX();
		minY+=v.getY();
		minZ+=v.getZ();
		maxX+=v.getX();
		maxY+=v.getY();
		maxZ+=v.getZ();
	}
	inline void doRotate(Vector3D av){
		for(Triangle* &t:triangulos)
			t->doRotate(av);
	}
	inline void doCenter(){doTranslate(-getCentro());}
	inline void render(){
		Vector3D p=this->getPos();
		glPushMatrix();
//		if(name=="mario_course.obj")
//			cout << "maroi _course"<<endl;
		glTranslatef(p.getX(),p.getY(),p.getZ());
		glRotatef(getRot().getX(), 1, 0, 0);
		glRotatef(getRot().getY(), 0, 1, 0);
		glRotatef(getRot().getZ(), 0, 0, 1);
		glScalef(scale.getX(),scale.getY(),scale.getZ());
/*
		glPushMatrix();
		  glTranslatef(0,getAlto()/2.0,0);
		  glColor3f(1,1,0);
		  glutWireSphere(getAlto()/2.0,10,10);
		glPopMatrix();
*/
		for(Triangle *t:triangulos)
			t->render();
		glPopMatrix();
	}

	void cargarMateriales(string nombre){
		string lineaUntrimmed;
		string matName;
		string nombreFichero=path+nombre;

		ifstream ficheroModeloObj(nombreFichero.c_str());
		if (ficheroModeloObj.is_open()){
			while (getline (ficheroModeloObj,lineaUntrimmed)){
				string linea=trim(lineaUntrimmed);
				if(linea[0]=='#' || linea.size()==0)
					continue;
				if(linea.find("newmtl")!=string::npos){
					vector<string> vs=split(linea);
					matName=vs[1];
					materiales[matName]=Material(path);
				}
				else{
					materiales[matName].parseLine(linea);
				}
			}
			ficheroModeloObj.close();
		}
		else{
			cout << "Fichero de material: "+nombreFichero+" no existe."<<endl;
		}
	}
	// model has to be in a folder named 'name' in model_base_path
	// in that folder should have a model file with name 'name.obj'
	void cargar(){
		string nombreFichero=path+name;
		maxX=maxY=maxZ=-1e40;
		minX=minY=minZ= 1e40;

		std::string lineaUntrimmed;

		ifstream ficheroModeloObj(nombreFichero.c_str());
		if (ficheroModeloObj.is_open()){
			while (getline (ficheroModeloObj,lineaUntrimmed)){
				//left trim linea TODO
				string linea=trim(lineaUntrimmed);
				if(linea[0]=='#' || linea.size()==0)
					continue;
				if (linea[0] == 'v')	{
					if(linea[1]=='t'){
						//Texture detected
						Vector3D *v=parseVector2D(linea);
						textures.push_back(v);
					}
					else if(linea[1]=='n'){
						//Normal detected
						Vector3D *v=parseVector3D(linea);
						normals.push_back(v);
					}
					else if(linea[1]==' '){
						//Vertex detected
						Vector3D *v=parseVector3D(linea);
						calculaExtremos(v);
						vertices.push_back(v);
					}
					else{
						runtime_error("Tipo de vertice no reconocido en ModeloMaterial::cargar()");
					}
				}
				if (linea[0] == 'f'){
					Triangle* t=parseTriangulos(linea);
					if(t!=nullptr){
						Texture* tex=materiales[currentMaterial].getMapKdTex();
						t->setTextura(tex);
						triangulos.push_back(t);
					}
					//else
					//	cout << "null ptr"<<endl;
				}
				if(linea.find("mtllib")!=string::npos){
					vector<string> vs=split(linea);
					cargarMateriales(vs[1]);
				}
				if(linea.find("usemtl")!=string::npos){
					vector<string> vs=split(linea);
					currentMaterial=vs[1];
				}
			}
			ficheroModeloObj.close();
		}
		else{
			cout << "Fichero "+nombreFichero+" no existe."<<endl;
		}
		cout<< "El modelo "<< name << " tiene "<< triangulos.size() << " triangulos."<<endl;
	}
 	Vector3D  *parseVector3D(string &linea){
 		float x,y,z;
 		istringstream ss(linea);
 		string s;
 	    ss>>s;//line type
 		ss>>x;
 		ss>>y;
 		ss>>z;
 		return new Vector3D(x,y,z);
 	}
 	Vector3D  *parseVector2D(string &linea){
 		float x,y;
 		istringstream ss(linea);
 		string s;
 	    ss>>s;//line type
 		ss>>x;
 		ss>>y;
 		return new Vector3D(x,y,0);
 	}
/*	Triangle *parseTriangulo(string &linea){
		istringstream ss(linea);
		int iv0,iv1,iv2;
		ss>>iv0;
		ss>>iv1;
		ss>>iv2;
		iv0--;
		iv1--;
		iv2--;
		Vector3D p0=*vertices[iv0];
		Vector3D p1=*vertices[iv1];
		Vector3D p2=*vertices[iv2];
		Triangle *t=new Triangle(p0,p1,p2);
		return t;
	}*/
	Triangle *parseTriangulos(string &linea){
		vector<string> vs=split(linea);
		if (vs.size()!=4){
			//throw runtime_error("Not triangle detected in parseTriangulos from model.cpp");
			//cout << "No triangle detected in "<<name<<" NO PROCESSED. May be a "<<vs.size()-1<<" faces poligon"<<endl;
			return nullptr;
		}
		if (vs[0]!="f")
			throw runtime_error("Not face line in parseTriangulos from model_material.cpp");
		vector<string> vs0=split(vs[1],'/');
		vector<int> vid0;
		for(string s:vs0){
			int i=0;
			if(trim(s)!="")
				i=stoi(s);
			vid0.push_back(i-1);
		}
		vector<string> vs1=split(vs[2],'/');
		vector<int> vid1;
		for(string s:vs1){
			int i=0;
			if(trim(s)!="")
				i=stoi(s);
			vid1.push_back(i-1);
		}
		vector<string> vs2=split(vs[3],'/');
		vector<int> vid2;
		for(string s:vs2){
			int i=0;
			if(trim(s)!="")
				i=stoi(s);
			vid2.push_back(i-1);
		}
		Vector3D p0=*vertices[vid0[0]];
		Vector3D p1=*vertices[vid1[0]];
		Vector3D p2=*vertices[vid2[0]];
		Triangle* t=nullptr;
		try{
			t=new Triangle(p0,p1,p2);
			if(vid0.size()>1){
				if(vid0[1]!=-1){
					Vector3D t0=*textures[vid0[1]];
					Vector3D t1=*textures[vid1[1]];
					Vector3D t2=*textures[vid2[1]];
					t->setT0(t0);
					t->setT1(t1);
					t->setT2(t2);
				}
			}
			if(vid0.size()==3){
				Vector3D n0=*normals[vid0[2]];
				Vector3D n1=*normals[vid1[2]];
				Vector3D n2=*normals[vid2[2]];
				t->setN0(n0);
				t->setN1(n1);
				t->setN2(n2);
			}
		}
		catch(runtime_error &e){
			t=nullptr;
			//cout<<"coño pasa"<<endl;
			return nullptr;
		}
	return t;
	}
	void calculaExtremos(Vector3D *v){
		float x=v->getX();
		float y=v->getY();
		float z=v->getZ();
		maxX=fmax(maxX,x);
		maxY=fmax(maxY,y);
		maxZ=fmax(maxZ,z);
		minX=fmin(minX,x);
		minY=fmin(minY,y);
		minZ=fmin(minZ,z);
	}
	//inline Contorno *getContorno();
	vector<Triangle*> &getTriangulos(){return triangulos;}
};
//must finish on slash '/'
string ModeloMaterial::model_base_path="modelos/";
