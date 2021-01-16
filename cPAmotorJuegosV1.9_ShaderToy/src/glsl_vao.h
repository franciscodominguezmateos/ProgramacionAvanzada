/*
 * glsl_vao.h
 *
 *  Created on: 14 Jan 2021
 *      Author: Francisco Dominguez
 */
#pragma once
class GLSLVAO;
using GLSLVAOPtr=GLSLVAO*;

class GLSLVAO{
	static const GLuint BYTES_PER_INT=4;
	static const GLuint BYTES_PER_FLOAT=4;
	GLuint id;
	vector<GLSLVBO> VBOs;
	GLSLVBO indexVBO;
	GLuint indexCount;
public:
	GLSLVAO():id(0),indexCount(0){}
	inline void init(){
		glGenVertexArrays(1, &id);
		bind();
	}
	GLuint getIndexCount(){return indexCount;}
	vector<GLSLVBO> &getVBOs(){return VBOs;}
	void setVBOs(vector<GLSLVBO> &vbos){
		VBOs=vbos;
		//this makes the asumption that attributes location
		//are the same that position in the vector
		for(unsigned int i=0;i<vbos.size();i++){
			GLSLVBO &vbo=vbos[i];
			vbo.bind();
			setVertexAttribPointer(i,vbo);
			vbo.unbind();
		}
	}
	inline void bind()  {glBindVertexArray(id);}
	inline void unbind(){glBindVertexArray(0);}
	inline void bind(vector<GLuint> &attributes){bind();for(GLuint i:attributes) glEnableVertexAttribArray(i);}
	inline void bind(GLuint atts...){vector<GLuint> attributes(atts);bind(attributes);}
	inline void bindAll(){bind();for(GLuint i=0;i<VBOs.size();i++) glEnableVertexAttribArray(i);}
	inline void unbind(vector<GLuint> &attributes){for(GLuint i:attributes) glDisableVertexAttribArray(i);unbind();}
	inline void unbind(GLuint atts...){vector<GLuint> attributes(atts);unbind(attributes);}
	inline void unbindAll(){for(GLuint i=0;i<VBOs.size();i++) glDisableVertexAttribArray(i);unbind();}
	void createIndexBuffer(vector<GLint> &indices){
		indexVBO=GLSLVBO(GL_ELEMENT_ARRAY_BUFFER);
		indexVBO.bind();
		indexVBO.storeData(indices);
		indexCount=indices.size();
	}
	void createIndexBuffer(vector<GLuint> &indices){
		indexVBO=GLSLVBO(GL_ELEMENT_ARRAY_BUFFER);
		indexVBO.bind();
		indexVBO.storeData(indices);
		indexCount=indices.size();
	}
	void setVertexAttribPointer(GLuint attribute,GLSLVBO &vbo){
		if(vbo.getDataType()==GL_FLOAT)
			glVertexAttribPointer(attribute,vbo.getAttrSize(),GL_FLOAT,GL_FALSE,0,0);
		else
			glVertexAttribIPointer(attribute,vbo.getAttrSize(),vbo.getDataType(),0,0);
	}
	void createAttribute(GLuint attribute,vector<GLfloat> &data,GLuint attrSize){
		GLSLVBO vbo(GL_ARRAY_BUFFER,GL_FLOAT,attrSize);
		vbo.bind();
		vbo.storeData(data);
		setVertexAttribPointer(attribute,vbo);
		vbo.unbind();
		VBOs.push_back(vbo);
	}
	void createAttribute(GLuint attribute,vector<GLint> &data,GLuint attrSize){
		GLSLVBO vbo(GL_ARRAY_BUFFER,GL_INT,attrSize);
		vbo.bind();
		vbo.storeData(data);
		setVertexAttribPointer(attribute,vbo);
		vbo.unbind();
		VBOs.push_back(vbo);
	}
	void createAttribute(GLuint attribute,vector<GLuint> &data,GLuint attrSize){
		GLSLVBO vbo(GL_ARRAY_BUFFER,GL_UNSIGNED_INT,attrSize);
		vbo.bind();
		vbo.storeData(data);
		setVertexAttribPointer(attribute,vbo);
		vbo.unbind();
		VBOs.push_back(vbo);
	}
	void deleteVBOs(){
		glDeleteVertexArrays(1,&id);
		for(GLSLVBO &vbo:VBOs) vbo.deleleteBuffer();
		indexVBO.deleleteBuffer();
	}
};




