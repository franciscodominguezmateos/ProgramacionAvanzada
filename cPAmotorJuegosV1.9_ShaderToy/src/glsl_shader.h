/*
 * glsl_shader.h
 *
 *  Created on: 14 Jan 2021
 *      Author: Francisco Dominguez
 */
#pragma once
class GLSLShader{
	GLuint shaderID;
    //30/04/2020
	//TODO: automatic extraction of uniform name variables
	//      just parse lines and extract names the ones that begin with uniform
	vector<string> uniformNames;
public:
	GLSLShader(GLuint shaderType):shaderID(glCreateShader(shaderType)){	}
    vector<string> &getUniformNames(){return uniformNames;}
	void parseUniformNames(string sourceCode){
	    //30/04/2020
		//06/05/2020: automatic extraction of uniform name variables
		//      just parse lines and extract names the ones that begin with uniform
		vector<string> lines;
		sourceCode>>lines;
		for(string line:lines){
			trim(line);
			trim(line,";");
			vector<string> words=split(line);
			if(words.size()>0)
			if(words[0]=="uniform"){
				string uniformName=words[2];
				vector<string> vs=split(uniformName,'[');
				if(vs.size()==1)
					uniformNames.push_back(uniformName);
				else
					uniformNames.push_back(vs[0]);
			}
		}
	}
	GLuint compileFromFileName(string fileName){
		cout << "Compiling shader: "<<fileName<<endl;
		string sourceCode;
		ifstream sf(fileName);
		sf>>sourceCode;
		return compile(sourceCode);
	}
	GLuint compile(string sourceCode){
		GLint Result = GL_FALSE;
		int InfoLogLength;

		// Compile shader
		char const * sourcePointer = sourceCode.c_str();
		glShaderSource(shaderID, 1, &sourcePointer , NULL);
		glCompileShader(shaderID);

		// Check errors
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &Result);
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if ( InfoLogLength > 0 ){
			std::vector<char> shaderErrorMessage(InfoLogLength+1);
			glGetShaderInfoLog(shaderID, InfoLogLength, NULL, &shaderErrorMessage[0]);
			throw runtime_error(&shaderErrorMessage[0]);
		}
		parseUniformNames(sourceCode);
		return shaderID;
	}
};




