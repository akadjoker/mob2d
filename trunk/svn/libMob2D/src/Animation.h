#ifndef ANIMATION_H
#define ANIMATION_H

#include "Mob2DCommon.h"

struct Frame
{
    GLfloat texture_coords[8];
    GLfloat vertex_array[8];
};
class Animation
{
	public:
		Animation();
		virtual ~Animation() { };

        float GetBoundingCircleRadius() { return bc_rad;        }
        float GetMaxWidth()             { return max_width;     }
        float GetMaxHeight()            { return max_height;    }

        uint GetMaxFrames()             { return frames.size(); }
        Frame GetFrame(uint frame);

        void AddFrame(Frame frame);

    protected:

    // These functions should only ever be called once!
		void SetAABBData();
		void SetBoundingCircleData();

    /// Creates a frame, sets the data, then pushes it into the frame vector.
        void SetFrame(Frame frame, int image_width, int image_height, int tl_x, int tl_y, int br_x, int br_y);

    /// Axis Alligned Bounding Box used when more precise collision detection is required.
        float max_width;
        float max_height;

    /// Bounding circle radius.
        float bc_rad;

        Frame error_frame;

        std::vector<Frame> frames;

        // friend class Mob2D_node;
        friend class Sprite;
};
typedef boost::shared_ptr<Animation> pAnimation;

/*
    Bounding volumes are never stored complete in this rendering engine. They are always stored partially by the
    Animation class and then completed by the calling node, and then returned in their completed form. The idea
    is not to explicitly provide bounding volumes, but to provide data for the derived applications to impliment
    their own.
    As an example, the bounding volume data is used by Mob2DRenderer to impliment automatic culling.
*/

#endif // ANIMATION_H
