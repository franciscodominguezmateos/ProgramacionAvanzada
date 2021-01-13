/*
 * glsl_shader_program.h
 *
 *  Created on: 14 Jan 2021
 *      Author: Francisco Dominguez
 */
#pragma once
class GLSLShaderProgram {
	GLuint programID;
	GLuint vertexShaderID;
	GLuint fragmentShaderID;
	map<string,Uniform> uniforms;
public:
	GLSLShaderProgram(){}
	void compileFromFileNames(string vertexFileName,string fragmentFileName){
		// Compiling vertes shader
		GLSLShader vs(GL_VERTEX_SHADER);
	    vertexShaderID=vs.compileFromFileName(vertexFileName);
	    addUniformsNames(vs.getUniformNames());
	    // Compiling frame shader
		GLSLShader fs(GL_FRAGMENT_SHADER);
		fragmentShaderID=fs.compileFromFileName(fragmentFileName);
	    addUniformsNames(fs.getUniformNames());
		link();
	}
	void compileFromStrings(string vertexSourceCode,string fragmentSourceCode){
		// Compiling vertes shader
		GLSLShader vs(GL_VERTEX_SHADER);
	    vertexShaderID=vs.compile(vertexSourceCode);
	    addUniformsNames(vs.getUniformNames());
	    // Compiling frame shader
		GLSLShader fs(GL_FRAGMENT_SHADER);
		fragmentShaderID=fs.compile(fragmentSourceCode);
	    addUniformsNames(fs.getUniformNames());
		link();
	}
	void link(){
		GLint Result = GL_FALSE;
		int InfoLogLength;

		// Vincular el programa por medio del ID
		//printf("Linking program\n");
		programID = glCreateProgram();
		glAttachShader(programID, vertexShaderID);
		glAttachShader(programID, fragmentShaderID);
		glLinkProgram(programID);

		// Revisar el programa
		glGetProgramiv(programID, GL_LINK_STATUS, &Result);
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if ( InfoLogLength > 0 ){
			std::vector<char> ProgramErrorMessage(InfoLogLength+1);
			glGetProgramInfoLog(programID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
			throw runtime_error(&ProgramErrorMessage[0]);
		}
		glDetachShader(programID, vertexShaderID);
		glDetachShader(programID, fragmentShaderID);

		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);
		setUniformLocations();
	}
	~GLSLShaderProgram(){stop();glDeleteProgram(programID);}
	void setUniformsLocation(vector<Uniform> &uniforms){for(Uniform &u:uniforms)u.setLocation(programID);}
	// I don't know how this work jet. Any help out there? ->
	//           Old->       https://docs.microsoft.com/en-us/cpp/cpp/functions-with-variable-argument-lists-cpp?view=vs-2019
	//           C++11       https://en.cppreference.com/w/cpp/utility/initializer_list
	//           C++11       https://en.cppreference.com/w/cpp/language/parameter_pack
	//                       https://thispointer.com/c11-variadic-template-function-tutorial-examples/
	//void setUniformsLocation(Uniform uniforms...){for(Uniform &u:uniforms)u.setLocation(programID);}
    //void setUniforms(initializer_list<string> strings){
    void setUniforms(vector<string> uniformNames){
     	for(auto s:uniformNames){
    		Uniform u(s);
    		u.setLocation(programID);
    		uniforms[s]=u;
    	}
    }
    void setUniformLocations(){
    	for(auto pair:uniforms){
    		uniforms[pair.first].setLocation(programID);
    	}
    }
    void addUniformsNames(vector<string> uniformNames){
         	for(auto s:uniformNames){
        		Uniform u(s);
        		//u.setLocation(programID);
        		uniforms[s]=u;
        	}
    }
    inline bool hasUniformName(string s){return !(uniforms.count(s)==0);}
    Uniform &getUniform(string s){return uniforms[s];}
	Uniform &operator[](string s){
		if(uniforms.count(s)==0)
			throw runtime_error(" in GLSLShaderProgram::operataor[] uniform name='"+s+ "' doesn't exist.");
		return uniforms[s];
	}
	void bindAttribute(GLuint attribID,string s){
		glBindAttribLocation(programID,attribID,s.c_str());
	}
	void start(){glUseProgram(programID);}
	void stop() {glUseProgram(0);}
	inline GLuint id(){return programID;}
};



