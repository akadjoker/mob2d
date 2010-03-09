#ifndef Mob2D_NODE_H
#define Mob2D_NODE_H

#include "Mob2DCommon.h"
#include "Sprite.h"

namespace Mob2D {

class mob2d_node;
typedef boost::shared_ptr<mob2d_node> M2DNode;

struct Mob2Daabb
{
    float x;
    float y;
    float width;
    float height;
};
struct Mob2DBoundingCircle
{
    float x;
    float y;
    float radius;
};

/**
Mob2D_node:
This is basically a smart pointer to a stored Sprite resource with extra methods thrown in. It is used to provide
access to the Sprite data. A game object holding onto an initialized Mob2DNode is GUARANTEED to point to a valid
instance of this until that said game instance is deallocated.
*/
class mob2d_node
{
	public:
		mob2d_node(pSprite sprite);
		~mob2d_node(){};

    /// Methods to set data for drawing.
		void StepFrame();
		void SetFrame(uint frame);
		void SetLayer(GLfloat layer);
		void SetBlend(float r, float g, float b);

        void SetShader(const string& shader);
		void SetAnimation(const string& anim);
		void SetSprite(const string& sprite_handle);

    /// Set the specified flag either on or off, depending on what it is currently set to.
		void ToggleOption(M2Denum val);
    /// Returns the specified flag's value.
		bool GetFlag(M2Denum val);

        void EnableShader();
		void ToggleShader();
		void DisableShader();

		void Rotate   (float angle )              { this->angle = angle;      }
		void Translate(int x, int y)              { this->x = x; this->y = y; }
		void Transform(int x, int y, float angle) { this->x = x; this->y = y; this->angle = angle; }

        bool isSpriteError() { return m_sprite->error(); }

        Mob2Daabb           GetAABB();
		Mob2DBoundingCircle GetBoundingCircle();

    protected:
		void ReassignToSprite(pSprite sprite);

		uint frame;

		bool draw;
		bool loop;
		bool draw_to_screen;
		bool shader_enabled;

		float x;
		float y;
		float layer; // z
		float angle;
        float scale_x;
        float scale_y;

		string shader;
		string animation;

		GLfloat blend_color[12];

		pSprite m_sprite;

		friend class SpriteManager;
		friend class Mob2DRenderer;
};

} // namespace
#endif
