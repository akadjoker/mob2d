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
    protected:
    // These should never be called outside the system. //

		void ReassignToSprite(pSprite sprite);

    /// Int denoting what frame it is on.
		uint frame;

    /// Flag denoting whether or not to animate the node, also whether or not to draw the node. In addition, if the
    /// draw_to_screen flag is set, it will use its coordinates as screen coordinates.
		bool draw;
		bool loop;
		bool draw_to_screen;

    /// Positional data, angular data, and scalar data.
		float x;
		float y;
		float layer; // z
		float angle;
        float scale_x;
        float scale_y;

    /// Current animation being rendered.
		string animation;

    /// This is used to provide direct access to the Sprite resource rather than looking it up through the sprite cache.
		pSprite m_sprite;

	public:
		mob2d_node(pSprite sprite);
		~mob2d_node(){};

    /// Methods to set data for drawing.
		void StepFrame();
		void SetFrame(uint frame);
		void SetAnimation(const string& anim);
		void SetSprite(const string& sprite_handle);
		void SetBlend(float r, float g, float b);
        void SetLayer(GLfloat layer);

    /// Set the specified flag either on or off, depending on what it is currently set to.
		void SetFlag(M2Denum val);
    /// Returns the specified flag's value.
		bool GetFlag(M2Denum val);

		void Rotate(float angle)                  { this->angle = angle;      }
		void Translate(int x, int y)              { this->x = x; this->y = y; }
		void Transform(int x, int y, float angle) { this->x = x; this->y = y; this->angle = angle; }

		Mob2Daabb           GetAABB();
		Mob2DBoundingCircle GetBoundingCircle();

        bool isSpriteError() { return m_sprite->error(); }

        GLfloat blend_color[12];

		friend class SpriteManager;
		friend class Mob2DRenderer;
};

} // namespace
#endif
