#ifndef ANIMATION_H
#define ANIMATION_H

#include "Mob2DCommon.h"
#include "Frame.h"

namespace m2d {

class Animation
{
	public:
		Animation();
		virtual ~Animation() { }

        float GetBoundingCircleRadius();
        float GetMaxWidth();
        float GetMaxHeight();

        uint GetMaxFrames();
        Frame GetFrame(uint frame);

        Frame AddFrame(int image_width, int image_height, int tl_x, int tl_y, int br_x, int br_y, bool error=false);

    protected:

    /// Axis Alligned Bounding Box used when more precise collision detection is required.
        float max_width;
    /// Also, the larger value is returned as the bounding circle radius.
        float max_height;

        Frame error_frame;

        std::vector<Frame> frames;

        // friend class Mob2D_node;
        friend class Sprite;
        friend class mob2d_node;
};
typedef boost::shared_ptr<Animation> pAnimation;

/*
    Bounding volumes are never stored complete in this rendering engine. They are always stored partially by the
    Animation class and then completed by the calling node, and then returned in their completed form. The idea
    is not to explicitly provide bounding volumes, but to provide data for the derived applications to impliment
    their own.
    As an example, the bounding volume data is used by Mob2DRenderer to impliment automatic culling.
*/

} // namespace
#endif // ANIMATION_H
