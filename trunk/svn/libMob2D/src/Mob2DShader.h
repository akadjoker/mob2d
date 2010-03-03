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

#include <map>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

// Already gives us TinyXML
#include "Mob2DCommon.h"
// #include "Mob2DShaderStrings.h"

using std::string;
using std::ifstream;
using std::map;
using std::vector;

class Mob2DFx
{
public:
    struct Mob2DShader
    {
        unsigned int id;
        // string filename;
        string source;
    };

	// For now, this class will just hold the shader program required to draw the image and discard alpha
	// fragments.
    Mob2DFx()
    {
		// m_vertexShader.source = vertexShader;
        // m_fragmentShader.source = fragmentShader;
    }

    virtual ~Mob2DFx() { };

    void unload()
    {
        glDetachShader(m_programID, m_vertexShader.id);
        glDetachShader(m_programID, m_fragmentShader.id);
        glDeleteShader(m_vertexShader.id);
        glDeleteShader(m_fragmentShader.id);
        glDeleteShader(m_programID);
    }

    bool initialize()
    {
        m_programID = glCreateProgram();
        m_vertexShader.id = glCreateShader(GL_VERTEX_SHADER);
        m_fragmentShader.id = glCreateShader(GL_FRAGMENT_SHADER);

		// The variables are defined in Mob2DShaderstd::strings.h
        // m_vertexShader.source = Mob2D_vertex_shader; // readXML(m_vertexShader.filename);
        // m_fragmentShader.source = Mob2D_fragment_shader; // readXML(m_fragmentShader.filename);

        // empty() is a string function
        if (m_vertexShader.source.empty() || m_fragmentShader.source.empty())
			{ return false; }

        const GLchar* tmp = static_cast<const GLchar*>(m_vertexShader.source.c_str());
        glShaderSource(m_vertexShader.id, 1, (const GLchar**)&tmp, NULL);

        tmp = static_cast<const GLchar*>(m_fragmentShader.source.c_str());
        glShaderSource(m_fragmentShader.id, 1, (const GLchar**)&tmp, NULL);

        if (!compileShader(m_vertexShader) || !compileShader(m_fragmentShader))
        {
			std::cerr << "Could not compile the shaders, they are invalid" << std::endl;
            return false;
        }

        glAttachShader(m_programID, m_vertexShader.id);
        glAttachShader(m_programID, m_fragmentShader.id);

        glLinkProgram(m_programID);
        return true;
    }

	void linkProgram()
		{ glLinkProgram(m_programID); }

    GLuint getUniformLocation(const string& name)
    {
        map<string, GLuint>::iterator i = m_uniformMap.find(name);
        if (i == m_uniformMap.end())
        {
            GLuint location = glGetUniformLocation(m_programID, name.c_str());
            m_uniformMap.insert(std::make_pair(name, location));
            return location;
        }
        return (*i).second;
    }

    GLuint getAttribLocation(const string& name)
    {
        map<string, GLuint>::iterator i = m_attribMap.find(name);
        if (i == m_attribMap.end())
        {
            GLuint location = glGetAttribLocation(m_programID, name.c_str());
            m_attribMap.insert(std::make_pair(name, location));
            return location;
        }

        return (*i).second;
    }

    void sendUniform(const string& name, const int id)
    {
        GLuint location = getUniformLocation(name);
        glUniform1i(location, id);
    }

    void sendUniform4x4(const string& name, const float* matrix, bool transpose=false)
    {
        GLuint location = getUniformLocation(name);
        glUniformMatrix4fv(location, 1, transpose, matrix);
    }

    void sendUniform3x3(const string& name, const float* matrix, bool transpose=false)
    {
        GLuint location = getUniformLocation(name);
        glUniformMatrix3fv(location, 1, transpose, matrix);
    }

    void sendUniform(const string& name, const float red, const float green,
                     const float blue, const float alpha)
    {
        GLuint location = getUniformLocation(name);
        glUniform4f(location, red, green, blue, alpha);
    }

    void sendUniform(const string& name, const float x, const float y,
                     const float z)
    {
        GLuint location = getUniformLocation(name);
        glUniform3f(location, x, y, z);
    }

    void sendUniform(const string& name, const float scalar)
    {
        GLuint location = getUniformLocation(name);
        glUniform1f(location, scalar);
    }

    void bindAttrib(unsigned int index, const string& attribName)
    {
        glBindAttribLocation(m_programID, index, attribName.c_str());
    }

    void bindShader()
    {
        glUseProgram(m_programID);
    }

private:

	// Repurpose this to read the XML from the SpriteSheets define file.
	/*
    string readFile(const string& filename)
    {
        ifstream fileIn(filename.c_str());

        if (!fileIn.good())
        {
            std::cerr << "Could not load shader: " << filename << std::endl;
            return string();
        }
        string std::stringBuffer(std::istreambuf_iterator<char>(fileIn), (std::istreambuf_iterator<char>()));
        return std::stringBuffer;
    }
    */
    bool compileShader(const Mob2DShader& shader)
    {
        glCompileShader(shader.id);
        GLint result = 0xDEADBEEF;
        glGetShaderiv(shader.id, GL_COMPILE_STATUS, &result);

        if (!result)
        {
            std::cout << "Could not compile shader: " << shader.id << std::endl;
            outputShaderLog(shader.id);
            return false;
        }
        return true;
    }

    void outputShaderLog(unsigned int shaderID)
    {
        vector<char> infoLog;
        GLint infoLen;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLen);
		infoLog.resize(infoLen);

        std::cerr << "GLSL Shader: Shader contains errors, please validate this shader!" << std::endl;
        glGetShaderInfoLog(shaderID, sizeof(infoLog), &infoLen, &infoLog[0]);

        std::cerr << string(infoLog.begin(), infoLog.end()) << std::endl;
#ifdef _WIN32
        MessageBox(NULL, string(infoLog.begin(), infoLog.end()).c_str(), "Error", MB_OK);
#endif

    }

    Mob2DShader m_vertexShader;
    Mob2DShader m_fragmentShader;
    unsigned int m_programID;

    map<string, GLuint> m_uniformMap;
    map<string, GLuint> m_attribMap;
};

#endif // GLSL_SHADER_H_INCLUDED
