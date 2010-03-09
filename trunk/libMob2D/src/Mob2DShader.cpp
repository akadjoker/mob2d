#include "Mob2DShader.h"

namespace Mob2D {

M2DFx::M2DFx()
{
    valid = false;
}

M2DFx::~M2DFx()
{
    unload();
}

void M2DFx::unload()
{
    glDetachShader(m_programID, m_vertexShader.id);
    glDetachShader(m_programID, m_fragmentShader.id);
    glDeleteShader(m_vertexShader.id);
    glDeleteShader(m_fragmentShader.id);
    glDeleteShader(m_programID);
}
bool M2DFx::initialize(char* vert, char* frag)// string vert, string frag)
{
    m_programID = glCreateProgram();
    m_vertexShader.id = glCreateShader(GL_VERTEX_SHADER);
    m_fragmentShader.id = glCreateShader(GL_FRAGMENT_SHADER);

    loadShader(vert, frag);

    // empty() is a string function
    if (m_vertexShader.source.empty() || m_fragmentShader.source.empty())
    {
        std::cerr<<"Empty shader string.\n";
        valid = false;
        return valid;
    }

    const GLchar* tmp = static_cast<const GLchar*>(m_vertexShader.source.c_str());
    glShaderSource(m_vertexShader.id, 1, (const GLchar**)&tmp, NULL);

    tmp = static_cast<const GLchar*>(m_fragmentShader.source.c_str());
    glShaderSource(m_fragmentShader.id, 1, (const GLchar**)&tmp, NULL);

    if (!compileShader(m_vertexShader) || !compileShader(m_fragmentShader))
    {
        glValidateProgram(m_programID);
        GLint status;
        glGetProgramiv(m_programID, GL_VALIDATE_STATUS, &status);

        std::cerr << "Could not compile the shaders, they are invalid.\n";
        valid = false;
        return valid;
    }
    glAttachShader(m_programID, m_vertexShader.id);
    glAttachShader(m_programID, m_fragmentShader.id);

    glLinkProgram(m_programID);

    valid = true;

    return valid;
}

bool M2DFx::loadShader(char* vertex_shader, char* fragment_shader)//string vertex_shader, string fragment_shader)
{
    char *vert = textFileRead(vertex_shader  );
    char *frag = textFileRead(fragment_shader);

    m_vertexShader.source = vert;
    m_fragmentShader.source = frag;

    // std::cout<<"VERTEX SHADER: \n\n"<<vert<<"\nFRAGMENT SHADER: \n\n"<<frag<<"\n";
}
char* M2DFx::textFileRead(char *fn)
{
    FILE *fp;
    char *content = NULL;

    int count=0;

    if (fn != NULL)
    {
        fp = fopen(fn,"rt");

        if (fp != NULL)
        {
            fseek(fp, 0, SEEK_END);
            count = ftell(fp);
            rewind(fp);

            if (count > 0)
            {
                content = (char *)malloc(sizeof(char) * (count+1));
                count = fread(content,sizeof(char),count,fp);
                content[count] = '\0';
            }
            fclose(fp);
        }
    }
    return content;
}
void M2DFx::linkProgram()
{
    glLinkProgram(m_programID);
}

GLuint M2DFx::getUniformLocation(const string& name)
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

GLuint M2DFx::getAttribLocation(const string& name)
{
    map<string, GLuint>::iterator i = m_attribMap.find(name);
    if(i == m_attribMap.end())
    {
        GLuint location = glGetAttribLocation(m_programID, name.c_str());
        m_attribMap.insert(std::make_pair(name, location));
        return location;
    }
    return (*i).second;
}
void M2DFx::sendUniform(const string& name, const int id)
{
    GLuint location = getUniformLocation(name);
    glUniform1i(location, id);
}
void M2DFx::sendUniform4x4(const string& name, const float* matrix, bool transpose)
{
    GLuint location = getUniformLocation(name);
    glUniformMatrix4fv(location, 1, transpose, matrix);
}
void M2DFx::sendUniform3x3(const string& name, const float* matrix, bool transpose)
{
    GLuint location = getUniformLocation(name);
    glUniformMatrix3fv(location, 1, transpose, matrix);
}
void M2DFx::sendUniform(const string& name, const float red, const float green,
                        const float blue, const float alpha)
{
    GLuint location = getUniformLocation(name);
    glUniform4f(location, red, green, blue, alpha);
}
void M2DFx::sendUniform(const string& name, const float x, const float y,
                        const float z)
{
    GLuint location = getUniformLocation(name);
    glUniform3f(location, x, y, z);
}
void M2DFx::sendUniform(const string& name, const float scalar)
{
    GLuint location = getUniformLocation(name);
    glUniform1f(location, scalar);
}
void M2DFx::bindAttrib(unsigned int index, const string& attribName)
{
    glBindAttribLocation(m_programID, index, attribName.c_str());
}
void M2DFx::bindShader()
{
    glUseProgram(m_programID);
}
bool M2DFx::compileShader(const M2DShader& shader)
{
    glCompileShader(shader.id);
    GLint result = 0xDEADBEEF;
    glGetShaderiv(shader.id, GL_COMPILE_STATUS, &result);

    if(!result)
    {
        std::cout << "Could not compile shader: " << shader.id << std::endl;
        outputShaderLog(shader.id);
        return false;
    }
    return true;
}
void M2DFx::outputShaderLog(unsigned int shaderID)
{
    vector<char> infoLog;
    GLint infoLen;
    glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLen);
    infoLog.resize(infoLen);

    std::cerr << "GLSL Shader: Shader contains errors, please validate this shader!" << std::endl;
    if(infoLen > 0)
    {
        glGetShaderInfoLog(shaderID, sizeof(infoLog), &infoLen, &infoLog[0]);
        std::cerr <<"GLSL LOG: "<< string(infoLog.begin(), infoLog.end()) << std::endl;
    #ifdef _WIN32
        MessageBox(NULL, string(infoLog.begin(), infoLog.end()).c_str(), "Error", MB_OK);
    #endif
    }
    else
        std::cerr <<"GLSL LOG EMPTY\n";
}

} // namespace
