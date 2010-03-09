#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include "Mob2DCommon.h"
#include "Mob2DShader.h"
#include "xml/tinyxml.h"

namespace Mob2D {

class ShaderManager
{
    public:
        static ShaderManager* Instance()
        { if(!m_pInstance) m_pInstance = new ShaderManager; return m_pInstance; }

    /// Loads a shader resource.
        bool LoadShader(const string& file);

        m2dFx GetShader(const string& name);

    protected:
        ShaderManager();
        virtual ~ShaderManager();

        static ShaderManager* m_pInstance;

        std::map<string, m2dFx> shaders;
};

}

#endif // SHADERMANAGER_H

/*
// Load a shader from XML.

    TiXmlElement* shader_element = root->FirstChildElement("shader");
    char* vertex_shader;
    char* fragment_shader;

    if(shader_element)
    {
        TiXmlElement* vert_elem = shader_element->FirstChildElement("vertex");
        TiXmlElement* frag_elem = shader_element->FirstChildElement("fragment");

        vertex_shader = strdup(vert_elem->Attribute("file"));
        fragment_shader = strdup(frag_elem->Attribute("file"));

        if(!shader.initialize(vertex_shader, fragment_shader))
            return false;

        else
        {
            shader.bindAttrib(0, "m2d_vertex"    );
            shader.bindAttrib(1, "m2d_texcoord"  );
            shader.bindAttrib(2, "m2d_blendcolor");
            shader.linkProgram();
            return true;
        }
    }
    else return false;
*/
