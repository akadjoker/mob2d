#ifndef FRAME_H
#define FRAME_H

#include "Mob2DCommon.h"

namespace Mob2D {
class Frame
{
    public:
        Frame()
        {
            glGenBuffers(1, &texture_coords_buff);
            glGenBuffers(1, &vertex_buff);
        }
        ~Frame()
        {
        //  glDeleteBuffers(GL_ARRAY_BUFFER, &texture_coords_buff);
        //  glDeleteBuffers(GL_ARRAY_BUFFER, &vertex_buff);
        }

        GLuint texture_coords_buff;
        GLuint vertex_buff;
};

} // namespace

#endif
