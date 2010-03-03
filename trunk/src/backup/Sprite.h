#ifndef SPRITE_H
#define SPRITE_H

#include "DARSCommon.h"
#include "Animation.h"
#include "DARSShader.h"

class Sprite;

typedef boost::shared_ptr<Sprite> pSprite;

/**
@class Sprite
The Sprite class handles the loading of the actual spritesheet data and stores that data in memory. Rendering
the quad is handled by an array stored in the renderer. All this really does is store data, and does not
provide any internal methods for modifying that data. Stores a map that holds references to sections (animations)
of the sheet.
*/
class Sprite
{
    struct strsrt
    {
        bool operator()(const std::string str1, const std::string str2) const
            { return strcmp(str1.c_str(), str2.c_str()) < 0; }
    };
    struct SprAlpha
    {
        GLfloat red;
        GLfloat green;
        GLfloat blue;
    };

	public:
    /// Used for creating the error sprite.
		Sprite();

    /// Name is set in the definition file.
		Sprite(string file);
		~Sprite()
		{
			glDeleteTextures(1, &image_handle);
		};

		int GetImageHeight()    { return image_height; }
		int GetImageWidth()     { return image_width; }

		string GetName()        { return name; }

		GLuint GetImageHandle() { return image_handle;}

		bool IsError()          { return error_flag; }

    protected:
    /// Tells the SpriteManager what its name is when it is created.
		string name;

		int image_width, image_height;

    /// Handle to the image stored in memory.
		GLuint image_handle;

		std::map<string, pAnimation, strsrt> animations; // boost::unordered_map<string, pAnimation>

        bool error_flag;

		SprAlpha alpha_color;

		// color that should not be rendered of the spritesheet. used in a shader.

		void LoadAnimations(TiXmlElement* root);
		void LoadAnimationFrame(TiXmlElement* frame_element, pAnimation anim);

		Frame SetFrameData(int tl_x, int tl_y, int br_x, int br_y);

        friend class dars_node;
        friend class DARSRenderer;
        friend class SpriteManager;
};
#endif
