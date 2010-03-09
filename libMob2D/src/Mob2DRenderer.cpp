#include "Mob2DRenderer.h"

namespace Mob2D {

Mob2DRenderer*
Mob2DRenderer::m_pInstance = NULL;

void Mob2DRenderer::Init(uint window_width, uint window_height, uint view_width, uint view_height)
{
	camera_data[0] = camera_data[1] = 0.0f; // left top
	camera_data[2] = view_width; // right
	camera_data[3] = view_height; // bottom

	window[0] = window_width;
	window[1] = window_height;

	if(view_width > view_height) m_bc.radius = view_width;
    else                         m_bc.radius = view_height;

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
	GLuint current_image = 0,
		   previous_image = 9001;

    // Loop through all instances of Mob2D_node (Mob2DNode) in the sprite manager and render them out.
	for(Mob2DNodeIter i = SpriteManager::Instance()->nodes.begin();
		i != SpriteManager::Instance()->nodes.end(); i++)
	{
		// If it's not an error
		if(((*i).second->draw) && (isInView((*i).second)))
		{
            current_image = (*i).second->m_sprite->GetImageHandle();

            glActiveTexture(GL_TEXTURE0);
            if(current_image != previous_image)
                glBindTexture(GL_TEXTURE_2D, (*i).second->m_sprite->GetImageHandle());

            if(!(*i).second->draw_to_screen)
            {
                glMatrixMode(GL_PROJECTION);
                glLoadIdentity();

                glOrtho(camera_data[0], camera_data[0]+camera_data[2],
                        camera_data[1]+camera_data[3], camera_data[1],
                        -9001, 9001);
                glViewport(0, 0, window[0], window[1]);
                glMatrixMode(GL_MODELVIEW);
                glLoadIdentity();
            }
            else
            {
                glMatrixMode(GL_PROJECTION);
                glLoadIdentity();

                glOrtho(0, window[0], 0, window[1], -9001, 9001);
                glViewport(0, 0, window[0], window[1]);

                glMatrixMode(GL_MODELVIEW);
                glLoadIdentity();
                // OH GOD DRAW TO SCREEN IS DRAWING UPSIDE DOWN!
                glTranslatef(0.375f, 0.375f, 0.0f);
                // glScalef(0.0f, -1.0f, 0.0f);
            }

            DrawNode((*i).second);

            previous_image = current_image;
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
    }
}
void Mob2DRenderer::DrawNode(M2DNode node)
{
    if(!node->m_sprite->error())
    {
        // only render it with a shader if it is enabled AND the shader is a valid one.
        if(node->shader_enabled && ShaderManager::Instance()->GetShader(node->shader)->isValid())
            DrawShader(node);
        else
            DrawFixedFunction(node);
    }
}
void Mob2DRenderer::DrawFixedFunction(M2DNode node)
{
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_BUFFER);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glBindBuffer(GL_ARRAY_BUFFER, node->m_sprite->GetFrame(node->animation,node->frame).vertex_buff);
    glVertexPointer(3, GL_FLOAT, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, node->m_sprite->GetFrame(node->animation,node->frame).texture_coords_buff);
    glTexCoordPointer(2, GL_FLOAT, 0, 0);

    glPushMatrix();
        glTranslatef(node->x, node->y, node->layer);
        glRotatef(node->angle, 0.0f, 0.0f, 1.0f);
        glScalef(node->scale_x, node->scale_y, 0.0f);
        glDrawArrays(GL_QUADS, 0, 4);
    glPopMatrix();

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_BUFFER);
}
void Mob2DRenderer::DrawShader(M2DNode node)
{
    // glGetFloatv(GL_MODELVIEW_PROJECTION_NV, mp_mat);
    // m_sprite->shader.sendUniform4x4("m2d_mp_matrix", mp_mat);

    glEnable(GL_DEPTH_BUFFER);
    ShaderManager::Instance()->GetShader(node->shader)->bindShader();

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, node->m_sprite->GetFrame(node->animation,node->frame).vertex_buff);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, node->m_sprite->GetFrame(node->animation,node->frame).texture_coords_buff);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, node->blend_color);

    glPushMatrix();
        glTranslatef(node->x, node->y, node->layer);
        glRotatef(node->angle, 0.0f, 0.0f, 1.0f);
        glScalef(node->scale_x, node->scale_y, 0.0f);

        ShaderManager::Instance()->GetShader(node->shader)->sendUniform("m2d_texture0", 0);

        glDrawArrays(GL_QUADS, 0, 4);
    glPopMatrix();

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);

    glUseProgram(0);
    glDisable(GL_DEPTH_BUFFER);

    // std::cout<<"Shader rendered.\n";
    // 1 -> m2d_vertex
    // 2 -> m2d_texcoord
    // 3 -> m2d_blendcolor
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

}// namespace
