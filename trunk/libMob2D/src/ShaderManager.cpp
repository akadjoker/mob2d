#include "ShaderManager.h"

namespace Mob2D {

ShaderManager*
ShaderManager::m_pInstance = NULL;

ShaderManager::ShaderManager()
{
    m2dFx error_shader(new M2DFx);
    shaders["ERROR"] = error_shader;
}

ShaderManager::~ShaderManager()
{
}

bool ShaderManager::LoadShader(const string& file)
{
    m2dFx shader(new M2DFx());

    TiXmlDocument doc(file);
	bool loaded = doc.LoadFile();
	TiXmlHandle sheet_dec(&doc);
	TiXmlElement* root = sheet_dec.FirstChildElement("shader").ToElement();

    if(root)
    {
        char* vertex_shader;
        char* fragment_shader;

        string name = root->Attribute("name");

        TiXmlElement* vert_elem = root->FirstChildElement("vertex");
        TiXmlElement* frag_elem = root->FirstChildElement("fragment");

        if(vert_elem && frag_elem)
        {
            vertex_shader = strdup(vert_elem->Attribute("file"));
            fragment_shader = strdup(frag_elem->Attribute("file"));

            if(shader->initialize(vertex_shader, fragment_shader))
            {
                if(name != "ERROR")
                {
                    if(shaders.find(name) == shaders.end())
                    {
                        shaders.insert(std::make_pair(name, shader));
                        Mob2DLog::Instance()->PushString("Shader successfuly loaded!("+name+")\n");
                    }
                    else
                    {
                        shaders.insert(std::make_pair(name, shader));
                        Mob2DLog::Instance()->PushString("Shader data overwritten.\n");
                    }
                    shader->bindAttrib(0, "m2d_vertex"    );
                    shader->bindAttrib(1, "m2d_texcoord"  );
                    shader->bindAttrib(2, "m2d_blendcolor");
                    shader->linkProgram();
                    return true;
                }
                else
                    Mob2DLog::Instance()->PushString("Shader load failed. Tried to overwrite default error resource.");
            }
            else
                return false;
        }
        else
            return false;
    }
    else
        return false;
}
m2dFx ShaderManager::GetShader(const string& shader)
{
    if(shaders.find(shader) != shaders.end())
		return shaders[shader];
	else
		return shaders["ERROR"];
}

} // namespace
