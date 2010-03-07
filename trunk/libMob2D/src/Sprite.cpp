#include "Sprite.h"

Sprite::Sprite()
{
	error_flag = true;
	shader_enabled = true;
	name = "ERROR";
	CreateDefaultAnimation();
}
Sprite::~Sprite()
{
    glDeleteTextures(1, &image_handle);
}
Sprite::Sprite(string file)
{
    CreateDefaultAnimation();

	error_flag = false;

	TiXmlDocument doc(file);
	bool loaded = doc.LoadFile();

	TiXmlHandle sheet_dec(&doc);

	TiXmlElement* root;

	shader_enabled = true;

	if(!doc.Error() && loaded)
	{
        root = sheet_dec.FirstChildElement("spritesheet").ToElement();

        if(root)
        {
            name = root->Attribute("name"); // The name parameter.

            if(!LoadImageProperties(root))
                Mob2DLog::Instance()->PushString("Sprite sheet declaration invalid. No <image_properties>.("+file+")\n");

            if(!LoadAnimations(root))
                Mob2DLog::Instance()->PushString("Animations could not be loaded. Check your frame declarations!("+file+")\n");

            if(!LoadShaderProgram(root))
            {
                Mob2DLog::Instance()->PushString("No shader program specified or the shader source is invalid.\nShaders will be disabled for this sprite.("+file+")\n");
                shader_enabled = false;
            }

            if(!LoadImageData(root))
                Mob2DLog::Instance()->PushString("Image data could not be loaded. Is the sheet_source attribute in <spritesheet> valid?("+file+")\n");
        }
        else
        {
            Mob2DLog::Instance()->PushString("Sprite sheet declaration invalid ("+file+"). No <spritesheet>.\n");
            error_flag = true;
        }
	}
	else
	{
	    if(doc.Error())
            Mob2DLog::Instance()->PushString("The document has errors in it. Fix the errors and check your tags!("+file+")\n");
        if(!loaded)
            Mob2DLog::Instance()->PushString("Could not load definition file for sprite.("+file+")\n");
		error_flag = true;
	}
	if(!error_flag)
        Mob2DLog::Instance()->PushString("Spritesheet successfuly loaded.("+file+")\n");
    else
        Mob2DLog::Instance()->PushString("Spritesheet loading failed somehow. Check the log for details.("+file+")\n");
}
bool Sprite::LoadImageProperties(TiXmlElement* root)
{
    TiXmlElement* props_element = root->FirstChildElement("image_properties");
    if(props_element)
    {
        props_element->QueryIntAttribute("width", &image_width);
        props_element->QueryIntAttribute("height", &image_height);
    }
    else
    {
        error_flag = true;
        return false;
    }
    return true;
}
// if a spritesheet definition does not have at least one animation element with at least one frame element, then it is invalid.
bool Sprite::LoadAnimations(TiXmlElement* root)
{
	// Scan for the first animation element.
	TiXmlElement* anim_element = root->FirstChildElement("animation");
	if(anim_element)
	{
		// outer loop. load the animation only after the frames for the animation are loaded.
		for(anim_element; anim_element; anim_element = anim_element->NextSiblingElement("animation"))
		{
			// Init an animation instance
			pAnimation anim(new Animation());
			string anim_name;

			TiXmlElement* frame_element = anim_element->FirstChildElement("frame");

			// inner loop. loop through all of the frame elements and push them into the animation.
			if(frame_element)
				for(frame_element; frame_element; frame_element = frame_element->NextSiblingElement("frame"))
					LoadAnimationFrame(frame_element, anim);
			else
			{
			    // create a zeroed frame instead of screwing the whole sheet over it.
				Mob2DLog::Instance()->PushString("Frame element invalid. Error frame created for the animation.\n");
				anim->AddFrame(image_width,image_height,0,0,0,0);
			}
            anim_name = anim_element->Attribute("name");
            animations.insert(std::pair<string, pAnimation>(anim_name, anim));
		}
	}
	else
	{
		Mob2DLog::Instance()->PushString("No animations for this spritesheet.\n");
		error_flag = true;
		return false;
	}
	return true;
}
bool Sprite::LoadShaderProgram(TiXmlElement* root)
{
    TiXmlElement* shader_element = root->FirstChildElement("shader");
    char* vertex_shader;
    char* fragment_shader;

    if(shader_element)
    {
        TiXmlElement* vert_elem = shader_element->FirstChildElement("vertex");
        TiXmlElement* frag_elem = shader_element->FirstChildElement("fragment");

        vertex_shader = strdup(vert_elem->Attribute("file"));
        fragment_shader = strdup(frag_elem->Attribute("file"));

        if(!shader.initialize(vertex_shader, fragment_shader)) return false;

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
}

void Sprite::LoadAnimationFrame(TiXmlElement* frame_element, pAnimation anim)
{
    TiXmlElement* tl_elem = frame_element->FirstChildElement("top_left");
    TiXmlElement* br_elem = frame_element->FirstChildElement("bottom_right");

    int top_left_x;     int top_left_y;
    int bottom_right_x; int bottom_right_y;
    if(tl_elem && br_elem)
    {
        tl_elem->QueryIntAttribute("x", &top_left_x);
        tl_elem->QueryIntAttribute("y", &top_left_y);
        br_elem->QueryIntAttribute("x", &bottom_right_x);
        br_elem->QueryIntAttribute("y", &bottom_right_y);
    }
    else
    {
        top_left_x = top_left_y = bottom_right_x = bottom_right_y = 0;
    }
    anim->AddFrame(image_width, image_height, top_left_x, top_left_y, bottom_right_x, bottom_right_y);
}
bool Sprite::LoadImageData(TiXmlElement* root)
{
    string file_to_load = root->Attribute("sheet_source");

    image_handle = SOIL_load_OGL_texture
        (file_to_load.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y );

    if(0 == image_handle)
    {
        error_flag = true;
        Mob2DLog::Instance()->PushString("Image file could not be loaded for "+name+".\n");
        return false;
    }
    else
    {
        error_flag = false;
        return true;
    }
}
pAnimation Sprite::GetAnimation(string anim)
{
    if(animations.find(anim) != animations.end())
        return animations[anim];
    else
        return animations["ERROR"];
}
/*
Frame Sprite::SetFrameData(int tl_x, int tl_y, int br_x, int br_y)
{
    Frame frame;

    GLfloat width, height;
	width = (GLfloat)((br_x - tl_x) / 2);
	height = (GLfloat)((br_y - tl_y) / 2);

	frame.vertex_array[0] = -width; frame.vertex_array[1] = height; frame.vertex_array[2] = 0.0f; // lower left
	frame.vertex_array[3] = width; frame.vertex_array[4] = height; frame.vertex_array[5] = 0.0f; // lower right
	frame.vertex_array[6] = width; frame.vertex_array[7] = -height; frame.vertex_array[8] = 0.0f; // upper right
	frame.vertex_array[9] = -width; frame.vertex_array[10] = -height; frame.vertex_array[11] = 0.0f; // upper left

	// bottom left
	frame.texture_coords[0] = (GLfloat)((GLfloat)tl_x/(GLfloat)image_width);
	frame.texture_coords[1] = (GLfloat)(1.0f-((GLfloat)br_y/(GLfloat)image_height));

	// bottom right
	frame.texture_coords[2] = (GLfloat)((GLfloat)br_x/(GLfloat)image_width);
	frame.texture_coords[3] = (GLfloat)(1.0f-((GLfloat)br_y/(GLfloat)image_height));

	// top right
    frame.texture_coords[4] = (GLfloat)((GLfloat)br_x/(GLfloat)image_width);
	frame.texture_coords[5] = (GLfloat)(1.0f-((GLfloat)tl_y/(GLfloat)image_height));

	// top left
	frame.texture_coords[6] = (GLfloat)((GLfloat)tl_x/(GLfloat)image_width);
	frame.texture_coords[7] = (GLfloat)(1.0f-((GLfloat)tl_y/(GLfloat)image_height));

    // DERRRRRRRRRRRRRRRRRRRRRRRRRRP! I NEVER RETURNED IT! A testimate to my occasional short-sightendess.
    return frame;
}
*/
void Sprite::CreateDefaultAnimation()
{
    pAnimation anim(new Animation());
    anim->AddFrame(0,0,0,0,0,0);
    animations["ERROR"] = anim;
}
int Sprite::GetImageHeight()
{
    return image_height;
}
int Sprite::GetImageWidth()
{
    return image_width;
}
string Sprite::GetName()
{
    return name;
}
bool Sprite::error()
{
    return error_flag;
}
GLuint Sprite::GetImageHandle()
{
    return image_handle;
}
uint Sprite::GetMaxFrames(string anim)
{
    return GetAnimation(anim)->GetMaxFrames();
}
Frame Sprite::GetFrame(string anim, uint frame)
{
    return GetAnimation(anim)->GetFrame(frame);
}
// upper left - -
// upper right + -
// lower right + +
// lower left - +
