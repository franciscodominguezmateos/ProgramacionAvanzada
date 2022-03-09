/*
 * glsl_vbo.h
 *
 *  Created on: 14 Jan 2021
 *      Author: Francisco Dominguez
 */
#pragma once
class GLSLVBO;
using GLSLVBOPtr=GLSLVBO*;

class GLSLVBO{
	static const GLuint BYTES_PER_INT=4;
	static const GLuint BYTES_PER_FLOAT=4;
	GLuint id;
	GLuint type;
	GLuint data_type;
	GLuint attrSize;
public:
	GLSLVBO():id(0),type(0){}
	GLSLVBO(GLuint type,GLuint dt=GL_FLOAT,GLuint as=3):type(type),data_type(dt),attrSize(as){glGenBuffers(1,&id);}
	inline GLuint getDataType(){return data_type;}
	inline GLuint getAttrSize(){return attrSize;}
	inline void bind(){glBindBuffer(type,id);}
	inline void unbind(){glBindBuffer(type,0);}
	inline void storeData(vector<GLfloat> &data){glBufferData(type,data.size()*BYTES_PER_FLOAT,data.data(),GL_STATIC_DRAW);}
	inline void storeData(vector<GLint>   &data){glBufferData(type,data.size()*BYTES_PER_INT  ,data.data(),GL_STATIC_DRAW);}
	inline void storeData(vector<GLuint>  &data){glBufferData(type,data.size()*BYTES_PER_INT  ,data.data(),GL_STATIC_DRAW);}
	inline void deleleteBuffer(){glDeleteBuffers(1,&id);}
};




