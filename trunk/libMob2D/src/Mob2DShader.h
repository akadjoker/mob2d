#ifndef GLSL_SHADER_H_INCLUDED
#define GLSL_SHADER_H_INCLUDED

/**
Mob2DFx:
This class is the class that processes GLSL programs for the rendering engine, since it seems to be the only way
to get the bastard image drawing to work.
*/

#ifdef _WIN32
    #include <windows.h>
#endif

/*
#include <map>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
*/

#include "Mob2DCommon.h"
// #include "Mob2DShaderStrings.h"

using std::string;
// using std::ifstream;
using std::map;
using std::vector;

namespace m2d {

class M2DFx
{
public:
    struct M2DShader
    {
        unsigned int id;
        string source;
    };
    M2DFx();
    virtual ~M2DFx();

    void unload();
    bool initialize(char* vert, char* frag);// string vert, string frag)
	void linkProgram();

    GLuint getUniformLocation(const string& name);
    GLuint getAttribLocation(const string& name);

    void validateShader(GLuint shader);

    void sendUniform(const string& name, const int id);
    void sendUniform4x4(const string& name, const float* matrix, bool transpose=false);
    void sendUniform3x3(const string& name, const float* matrix, bool transpose=false);
    void sendUniform(const string& name, const float red, const float green, const float blue, const float alpha);
    void sendUniform(const string& name, const float x, const float y, const float z);
    void sendUniform(const string& name, const float scalar);

    void bindAttrib(unsigned int index, const string& attribName);
    void bindShader();

private:
    bool compileShader(const M2DShader& shader);
    void outputShaderLog(unsigned int shaderID);

    char* textFileRead(char* fn);

    bool loadShader(char* vertex_shader, char* fragment_shader);//string vertex_shader, string fragment_shader)

    M2DShader m_vertexShader;
    M2DShader m_fragmentShader;

    unsigned int m_programID;

    map<string, GLuint> m_uniformMap;
    map<string, GLuint> m_attribMap;
};

} // anmespace
#endif // GLSL_SHADER_H_INCLUDED
