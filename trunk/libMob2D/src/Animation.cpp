#include "Animation.h"
Animation::Animation()
{
    max_width = 0.0f;
    max_height = 0.0f;
    bc_rad = 0.0f;

    SetFrame(error_frame,0,0,0,0,0,0);
}
void Animation::SetAABBData()
{
    float w, h;
    for(std::vector<Frame>::iterator i = frames.begin(); i != frames.end(); i++)
    {
        w = (float)((*i).vertex_array[0]*2.0f);
        h = (float)((*i).vertex_array[1]*2.0f);

        if(w > max_width)
            max_width = w;
        if(h > max_height)
            max_height = h;
    }
}
void Animation::SetBoundingCircleData()
{
    if(max_width >= max_height)
        bc_rad = max_width;
    else
        bc_rad = max_height;
}
void Animation::AddFrame(Frame frame)
{
    frames.push_back(frame);
    SetAABBData();
    SetBoundingCircleData();
}
Frame Animation::GetFrame(uint frame)
{
    if(frame >= frames.size())
        return error_frame;
    else
        return frames[frame];
}
void Animation::SetFrame(Frame frame, int image_width, int image_height, int tl_x, int tl_y, int br_x, int br_y)
{
    GLfloat width, height;
	width = (GLfloat)((br_x - tl_x) / 2);
	height = (GLfloat)((br_y - tl_y) / 2);

	frame.vertex_array[0] = -width; frame.vertex_array[1] = height; // lower left
	frame.vertex_array[2] = width; frame.vertex_array[3] = height; // lower right
	frame.vertex_array[4] = width; frame.vertex_array[5] = -height; // upper right
	frame.vertex_array[6] = -width; frame.vertex_array[7] = -height; // upper left

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

    // return frame;
    // frames.push_back(frame);
}
