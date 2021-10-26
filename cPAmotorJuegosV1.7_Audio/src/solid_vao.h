/*
 * solid_vao.h
 *
 *  Created on: 2 Apr 2020
 *      Author: Francisco Dominguez
 */
#pragma once
#include "shader.h"
#include "model_mesh.h"
#include "solid.h"
/*
 * The point of this class is to use one instance as master, the owner of the pointer vao
 * and other clones are used to share the vao attribute of the master instance.
 * Then we can render lots of different objects, in different location and orientation, with the same vao.
 *
 */
// Masters SolidVAOs are owner of the vao and should release then in the destructor
// Masters SolidVAOs have to be initiates with a mesh in order to upload data(vertex info) to GPU
// Example: SolidVAO mvao0(&shaderProgram),mvao1(&shaderProgram);
// Not Masters SolidVAOs are not aware of the vao share it with master
// next instance mvao0 is the master, we can use copy constructor to set not master SolidVAOs
// not master vaos do not have to be initiated with a mesh.
// Example: SolidVAO mvaor(mvao0);
class SolidVAO: public Solid {
	GLSLShaderProgram* shaderProgram;
	GLSLVAO* vao;
	bool isMaster;
	//23/04/2020 added one material per VAO
	//it is used only for texturing
	//This attribute has to be set in all kind of SolidVAO, not only in master
	Material material;
public:
	SolidVAO(GLSLShaderProgram* p,GLSLVAO *pvao=nullptr):shaderProgram(p),vao(pvao),isMaster(false){
		if(vao==nullptr){
			//this is the master instance the owner of vao
			vao=new GLSLVAO();
			isMaster=true;
		}
	}
	//This copy constructor is a standard one but with isMaster to false, because is not a master SolidVAO
	SolidVAO(const SolidVAO &s):shaderProgram(s.shaderProgram),vao(s.vao),isMaster(false),material(s.material){}
	virtual ~SolidVAO(){
		if(isMaster) delete vao;
	}
	SolidVAO *clone(){return new SolidVAO(*this);}
	void initAttributes(ModelMesh &m){
		vao->bind();
		vector<GLfloat> vf=m.getFloatFromVertices();
		vao->createAttribute(0,vf,3);
		vector<GLfloat> vt=m.getFloatFromTextures();
		vao->createAttribute(1,vt,2);
		vector<GLfloat> vn=m.getFloatFromNormals();
		vao->createAttribute(2,vn,3);
		vao->unbind();
	}
	// This only have to call on the master instance
	void init(ModelMesh &m){
		vao->init();
		vao->createIndexBuffer(m.getIvertices());
		initAttributes(m);
	}
	//24/04/2020
	//this two methods allow share attribute material
	//in order to render different parts with different materials
	//all parts share the attribute but have their own indexBuffer
	//This method could be mixed with the above one but let it be for clarity purpose
	void initMaterial(ModelMesh &m, string materialName,bool initAttr=true){
		material=m.getMaterial(materialName);
		vao->init();
		vector<GLuint> idxV=m.getIverticesMaterial(materialName);
		vao->createIndexBuffer(idxV);
		if(initAttr) initAttributes(m);
	}
	void shareAttributes(SolidVAO &s){
		vao->bind();
		GLSLVAO* svao=s.getVAOPtr();
		vao->setVBOs(svao->getVBOs());
		vao->unbind();
	}
	void setMaterial(Material &m){material=m;}
	Material &getMaterial(){return material;}
	void render(){
		GLSLShaderProgram &sp=*shaderProgram;
		material.bind();
		sp.start();
	    vao->bindAll();
		sp["T"]=posEulerAnglesToTransformationMatrix<float>(getPos(),getRot());
		glDrawElements(GL_TRIANGLES, vao->getIndexCount(), GL_UNSIGNED_INT,0);
		vao->unbindAll();
		sp.stop();
		material.unbind();
	}
	GLSLVAO* getVAOPtr(){return vao;}
};
//Some model have many textures
//With the next class we solve the problem having many SolidVAO
//one for each texture.
//The first one own all vertex, normals and textcoord
//TODO: differentiate from master and no master in order render
//     many same objects in different locations.
class SolidMultiVAO:Solid{
	GLSLShaderProgram* shaderProgram;
	vector<SolidVAO*> solidVAOs;
public:
	SolidMultiVAO(GLSLShaderProgram* p):shaderProgram(p){}
	void init(ModelMesh &m){
		vector<string> mn=m.getMaterialNames();
		assert(mn.size()>0);
	    SolidVAO* svao=new SolidVAO(shaderProgram);
	    svao->initMaterial(m,mn[0]);
	    solidVAOs.push_back(svao);
	    //all other vaos share with this one attributes
	    SolidVAO &vao=*solidVAOs[0];
		for(unsigned int i=1;i<mn.size();i++){
			svao=new SolidVAO(shaderProgram);
			svao->initMaterial(m,mn[i],false);
			svao->shareAttributes(vao);
			solidVAOs.push_back(svao);
		}
	}
	void setPos(Vector3D p){Solid::setPos(p);for(SolidVAO* &sv:solidVAOs) sv->setPos(p);}
	void setRot(Vector3D r){Solid::setRot(r);for(SolidVAO* &sv:solidVAOs) sv->setRot(r);}
	void render(){for(SolidVAO* &sv:solidVAOs) sv->render();}
};

