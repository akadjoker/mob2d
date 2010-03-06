#include "Animation.h"

Animation::Animation()
{
    max_width = 0.0f;
    max_height = 0.0f;
    bc_rad = 0.0f;

    error_frame = AddFrame(0,0,0,0,0,0, true);
}
Frame Animation::AddFrame(int image_width, int image_height, int tl_x, int tl_y, int br_x, int br_y, bool error)
{
    Frame frame;
// VBO
    GLfloat texture_coords[8];
    GLfloat vertex_array[12];
// */

// ALL
    GLfloat width, height;
	width = (GLfloat)((br_x - tl_x)/2);
	height = (GLfloat)((br_y - tl_y)/2);

// VBO
    vertex_array[0] = -width; vertex_array[1] = height; vertex_array[2] = 0.0f; // lower left
	vertex_array[3] = width; vertex_array[4] = height; vertex_array[5] = 0.0f; // lower right
	vertex_array[6] = width; vertex_array[7] = -height; vertex_array[8] = 0.0f; // upper right
	vertex_array[9] = -width; vertex_array[10] = -height; vertex_array[11] = 0.0f; // upper left
// */

// VBO
	texture_coords[0] = (GLfloat)((GLfloat)tl_x/(GLfloat)image_width); // bottom left
	texture_coords[1] = (GLfloat)(1.0f-((GLfloat)br_y/(GLfloat)image_height));
	texture_coords[2] = (GLfloat)((GLfloat)br_x/(GLfloat)image_width); // bottom right
	texture_coords[3] = (GLfloat)(1.0f-((GLfloat)br_y/(GLfloat)image_height));
    texture_coords[4] = (GLfloat)((GLfloat)br_x/(GLfloat)image_width); // top right
	texture_coords[5] = (GLfloat)(1.0f-((GLfloat)tl_y/(GLfloat)image_height));
	texture_coords[6] = (GLfloat)((GLfloat)tl_x/(GLfloat)image_width); // top left
	texture_coords[7] = (GLfloat)(1.0f-((GLfloat)tl_y/(GLfloat)image_height));
// */
// VBO
	glGenBuffers(1, &frame.texture_coords_buff);
	glGenBuffers(1, &frame.vertex_buff);

	glBindBuffer(GL_ARRAY_BUFFER, frame.texture_coords_buff);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*8, texture_coords, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, frame.vertex_buff);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*12, vertex_array, GL_DYNAMIC_DRAW);
// */

    if(!error)
    {
        frames.push_back(frame);
    }
    float aabb_width  = width  * 2.0f,
          aabb_height = height * 2.0f;

    if(aabb_width > max_width)
        max_width = aabb_width;

    if(aabb_height > max_height)
        max_height = aabb_height;

    std::cerr<<max_width<<" "<<max_height<<"\n";

    return frame;
}
Frame Animation::GetFrame(uint frame)
{
    if(frame >= frames.size()) return error_frame;
    else                       return frames[frame];
}
float Animation::GetBoundingCircleRadius()
{
    if(max_width >= max_height)
        return max_width;
    else
        return max_height;
}
float Animation::GetMaxWidth()
{
    return max_width;
}
float Animation::GetMaxHeight()
{
    return max_height;
}
uint Animation::GetMaxFrames()
{
    return frames.size();
}
