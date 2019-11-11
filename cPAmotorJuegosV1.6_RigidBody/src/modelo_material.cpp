/*
 * modelo.cpp
 *
 *  Created on: Nov 12, 2015
 *      Author: francisco
 */

#include "modelo_material.h"

//must fihish on slash '/'
string ModeloMaterial::model_base_path="modelos/";

ModeloMaterial::~ModeloMaterial() {
	// TODO Auto-generated destructor stub
}

Triangulo *ModeloMaterial::centrar(Triangulo *t){
	Vector3D centro(minX+getAncho()/2.0,minY+getAlto()/2.0,minZ+getProfundo()/2);
	Vector3D p0=t->getP0();
	Vector3D p1=t->getP1();
	Vector3D p2=t->getP2();
	/// NNOOOOOOOOO
	return(new Triangulo(p0-centro,p1-centro,p2-centro));
}

Vector3D *ModeloMaterial::parseVector3D(string &linea){
	float x,y,z;

	istringstream ss(linea);
	string s;
    ss>>s;//line type
	ss>>x;
	ss>>y;
	ss>>z;
	return new Vector3D(x,y,z);
}
Vector3D *ModeloMaterial::parseVector2D(string &linea){
	float x,y;

	istringstream ss(linea);
	string s;
    ss>>s;//line type
	ss>>x;
	ss>>y;
	return new Vector3D(x,y,0);
}

Triangulo *ModeloMaterial::parseTriangulo(string &linea){
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
	Triangulo *t=new Triangulo(p0,p1,p2);
	return t;
}
Triangulo *ModeloMaterial::parseTriangulos(string &linea){
	vector<string> vs=split(linea);
	if (vs.size()!=4){
		//throw runtime_error("Not triangle detected in parseTriangulos from model.cpp");
		//cout << "No triangle detected in "<<name<<" NO PROCESSED. May be a "<<vs.size()-1<<" faces poligon"<<endl;
		return nullptr;
	}
	if (vs[0]!="f")
		throw runtime_error("Not face line in parseTriangulos from model.cpp");
	vector<string> vs0=split(vs[1],'/');
	vector<int> vid0;
	for(string s:vs0){
		int i=stoi(s);
		vid0.push_back(i-1);
	}
	vector<string> vs1=split(vs[2],'/');
	vector<int> vid1;
	for(string s:vs1){
		int i=stoi(s);
		vid1.push_back(i-1);
	}
	vector<string> vs2=split(vs[3],'/');
	vector<int> vid2;
	for(string s:vs2){
		int i=stoi(s);
		vid2.push_back(i-1);
	}
	Vector3D p0=*vertices[vid0[0]];
	Vector3D p1=*vertices[vid1[0]];
	Vector3D p2=*vertices[vid2[0]];
	Triangulo *t=new Triangulo(p0,p1,p2);
	if(vid0.size()>1){
		Vector3D t0=*textures[vid0[1]];
		Vector3D t1=*textures[vid1[1]];
		Vector3D t2=*textures[vid2[1]];
		t->setT0(t0);
		t->setT1(t1);
		t->setT2(t2);
	}
	if(vid0.size()==3){
		Vector3D n0=*normals[vid0[2]];
		Vector3D n1=*normals[vid1[2]];
		Vector3D n2=*normals[vid2[2]];
		t->setN0(n0);
		t->setN1(n1);
		t->setN2(n2);
	}
return t;
}
void ModeloMaterial::calculaExtremos(Vector3D *v){
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

// model as to be in a floder named 'name' in model_base_path
// in that folder should be a model file with name 'name'.obj
void ModeloMaterial::cargar(){
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
				Triangulo *t=parseTriangulos(linea);
				if(t){//t!=nullptr
					Textura* tex=materiales[currentMaterial].getMapKdTex();
					t->setTextura(tex);
					triangulos.push_back(t);
				}
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
void ModeloMaterial::cargarMateriales(string nombre){
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
		cout << "Fichero "+nombreFichero+" no existe."<<endl;
	}
}

/*
inline Contorno *Modelo::getContorno(){
	return new Contorno(getP(),getAlto()/2.0);
}
*/
