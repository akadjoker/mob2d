#include "Mob2DRenderer.h"

Mob2DRenderer*
Mob2DRenderer::m_pInstance = NULL;

void Mob2DRenderer::Init(uint window_width, uint window_height, uint view_width, uint view_height)
{
	camera_data[0] = camera_data[1] = 0.0f; // left top
	camera_data[2] = view_width; // right
	camera_data[3] = view_height; // bottom

	window[0] = window_width;
	window[1] = window_height;

	if(view_width > view_height)
        m_bc.radius = view_width;
    else
        m_bc.radius = view_height;

    m_bc.x = m_bc.y = camera_data[0];

    GLfloat debug_color_arr[3];
    debug_color_arr[0] = 100/256;
    debug_color_arr[1] = 245/256;
    debug_color_arr[2] = 25/256;

    glGenBuffers(1, &debug_color);
    glBindBuffer(GL_ARRAY_BUFFER, debug_color);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*3, debug_color_arr, GL_DYNAMIC_DRAW);
}
void Mob2DRenderer::Render()
{

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(camera_data[0], camera_data[0]+camera_data[2], camera_data[1]+camera_data[3], camera_data[1], -9001, 9001);
    glViewport(0, 0, window[0], window[1]);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	GLuint current_image = 0,
		   previous_image = 9001;

// Loop through all instances of Mob2D_node (Mob2DNode) in the sprite manager and render them out.
// Handle the binding of textures here, but let the nodes translate and rotate themselves.
	for(Mob2DNodeIter i = SpriteManager::Instance()->nodes.begin();
		i != SpriteManager::Instance()->nodes.end(); i++)
	{
		// If it's not an error
		if(((*i).second->draw) && (isInView((*i).second)))
		{
            current_image = (*i).second->m_sprite->GetImageHandle();

            // if the texture is already bound, there's no need to rebind it. I'm taking advantage of the
            // stl::multimap's native sorting capabilities here.
            glActiveTexture(GL_TEXTURE0);
            if(current_image != previous_image)
                glBindTexture(GL_TEXTURE_2D, (*i).second->m_sprite->GetImageHandle());

            // Take it to the node. The node is what holds the draw routine. Class Mob2D_node.
            // It also automatically uses its internal parameters
            (*i).second->Draw();

            previous_image = current_image;
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
    }
}
void Mob2DRenderer::MoveCamera(int x, int y)
{
    m_bc.x = x;
    m_bc.y = y;

    camera_data[0] = x;
    camera_data[1] = y;
}
void Mob2DRenderer::SetView(int width, int height)
{
    camera_data[2] = width;
    camera_data[3] = height;

    if(width >= height)
        m_bc.radius = width;
    else
        m_bc.radius = height;
}
void Mob2DRenderer::SetWindowData(int width, int height)
{
    window[0] = width; window[1] = height;
}
// Culling routine using bounding circles.
bool Mob2DRenderer::isInView(M2DNode node)
{
    float rel_x = node->GetBoundingCircle().x - m_bc.x,
          rel_y = node->GetBoundingCircle().y - m_bc.y;

    float distance = rel_x * rel_x + rel_y * rel_y;

    float min_dist = node->GetBoundingCircle().radius + m_bc.radius;
    return distance <= min_dist * min_dist;
}
void Mob2DRenderer::Deinit()
{
}
