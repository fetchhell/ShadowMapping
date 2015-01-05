#pragma once

#include <include/includeAll.h>
#include <file/my_file.h>
#include <object/my_object.h>

using namespace std;

//-----------------------
class Shader {
	public:
    
    //----------------------------------------------
	bool loadShader(const char *, const char *);
	bool compileShader(string, unsigned int);
	bool linkProgram();

    //----------------------------------------------
    template<int N> void setAttribute(const char *name, const float (&attribute)[N], int perAttrib);

    template<int M, int N> void setUniform(const char* name, const float (&uniform)[M][N]);
	template<int N> void setUniform(const char* name, const float (&uniform)[N]);
	void setUniform(const char* name, const float uniform);

    void drawScene(object& obj, GLuint *vbo_Ids);

	int getProgramHandle() { return program_handle; }

    private:

	int program_handle;
	vector<int> shaderHandles;
};

//-----------------------------------------------------------
template<int N>
void Shader::setAttribute(const char *name, const float (&attribute)[N], int perAttrib) {

	glUseProgram(program_handle);

	int attribute_handle = glGetAttribLocation(program_handle, name);
	glEnableVertexAttribArray(attribute_handle);

	glVertexAttribPointer(attribute_handle, perAttrib, GL_FLOAT, false, 0, 0); 
}

//-----------------------------------------------------------
template<int M, int N> 
void Shader::setUniform(const char* name, const float (&uniform)[M][N]) {

	glUseProgram(program_handle);

	int uniform_handle = glGetUniformLocation(program_handle, name);
	if(N == 4) glUniformMatrix4fv(uniform_handle, 1, false, uniform[0]);
	else if(N == 3) glUniformMatrix3fv(uniform_handle, 1, false, uniform[0]);
}

//-----------------------------------------------------------
template<int N> 
void Shader::setUniform(const char* name, const float (&uniform)[N]) {

	glUseProgram(program_handle);

	int uniform_handle = glGetUniformLocation(program_handle, name);
	if(N == 4) glUniform4fv(uniform_handle, 1, uniform);
	else if(N == 3) glUniform3fv(uniform_handle, 1, uniform);
	else if(N == 2) glUniform2fv(uniform_handle, 1, uniform);
}

