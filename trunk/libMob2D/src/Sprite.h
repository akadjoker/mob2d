#ifndef SPRITE_H
#define SPRITE_H

#include "Mob2DCommon.h"
#include "Animation.h"
#include "Mob2DShader.h"
#include "xml/tinyxml.h"
#include "soil/SOIL.h"

class Sprite;

typedef boost::shared_ptr<Sprite> pSprite;

/**
@class Sprite
The Sprite class is a structure that holds all the data being rendered. The structure of the Sprite class and its
associated classes Animation and the Frame struct lead to a very flexible overall structure that allow for pretty
much any kind of spritesheet layout to be loaded.

Also transient, if you're reading this, you and your XML abstraction idea can jump off a cliff.
*/
class Sprite
{
	public:
		Sprite();
		Sprite(string file);
		~Sprite();

		int GetImageHeight();
		int GetImageWidth();
		string GetName();

		bool error();

		GLuint GetImageHandle();

        uint GetMaxFrames(string anim);
		pAnimation GetAnimation(string anim);
        Frame GetFrame(string anim, uint frame);

        Frame SetFrameData(int tl_x, int tl_y, int br_x, int br_y);

    protected:
    /// Tells the SpriteManager what its name is when it is created.
		string name;

		int image_width;
		int image_height;

        bool error_flag;
        bool shader_enabled; // Not all sprites have to have a shader. Those that do will, use them.

    /// Handle to the image stored in memory.
		GLuint image_handle;

    /// Shader that the sprite runs.
		M2DFx shader;

        // hash map implimentation.
        // boost::unordered_map<string, pAnimation> animations;
        std::map<string, pAnimation> animations;

        //GLfloat alpha_color[3];

        void CreateDefaultAnimation();

        bool LoadImageProperties(TiXmlElement* root);
        bool LoadAnimations(TiXmlElement* root);
        bool LoadImageData(TiXmlElement* root);
        bool LoadShaderProgram(TiXmlElement* root); // Loads the shader strings from the XML file. Will need to make sure newlines are preserved.
        void LoadAnimationFrame(TiXmlElement* frame_element, pAnimation anim);

        friend class mob2d_node;
        friend class Mob2DRenderer;
};
#endif
