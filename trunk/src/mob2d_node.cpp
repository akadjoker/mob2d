#include "mob2d_node.h"

#include "SpriteManager.h"
#include "Mob2DRenderer.h"
#include "Mob2D.h"

mob2d_node::mob2d_node(pSprite sprite)
{
    angle = 0.0f;
	m_sprite = sprite;
	frame = x = y = 0;
    scale_x = scale_y = 1.0f;

    animation = "ERROR";

    draw_to_screen = false;
    loop = draw = true;

	blend_color.red = blend_color.green = blend_color.blue = 1.0f;
}
void mob2d_node::SetAnimation(string anim)
{
    if(m_sprite->animations.find(anim) != m_sprite->animations.end())
        animation = anim;
    else
        animation = "ERROR";
}
// The source image has already been bound by the Mob2DRenderer. Now translate, rotate, draw the quad, and apply
// the texture using the correctly converted texture coordinates.
void mob2d_node::Draw()
{
    glTexCoordPointer(2, GL_FLOAT, 0, &m_sprite->GetFrame(animation, frame).texture_coords[0]);
    glVertexPointer(2, GL_FLOAT, 0, &m_sprite->GetFrame(animation, frame).vertex_array[0]);
    glPushMatrix();
        glTranslatef(x, y, 1.0f);
        glRotatef(angle, 0.0f, 0.0f, 1.0f);
        glScalef(scale_x, scale_y, 0.0f);
    glDrawArrays(GL_QUADS, 0, 4);
    glPopMatrix();

    //std::cout<<animation<<".\n";
}
void mob2d_node::DrawToScreen()
{
    Mob2DCoord coord = Mob2D::API()->GetWorldCoords(x, y);
    glTexCoordPointer(2, GL_FLOAT, 0, &m_sprite->GetFrame(animation, frame).texture_coords[0]);
    glVertexPointer(2, GL_FLOAT, 0, &m_sprite->GetFrame(animation, frame).vertex_array[0]);
    glPushMatrix();
        glTranslatef(coord.x, coord.y, 1.0f);
        glRotatef(angle, 0.0f, 0.0f, 1.0f);
    // Scale value = camera_view_size / window_size
        glScalef(Mob2DRenderer::Instance()->camera_data[2]/Mob2DRenderer::Instance()->window[0],
                 Mob2DRenderer::Instance()->camera_data[3]/Mob2DRenderer::Instance()->window[1], 0.0f);
    glDrawArrays(GL_QUADS, 0, 4);
    glPopMatrix();
}
void mob2d_node::DrawError()
{
    glVertexPointer(3, GL_FLOAT, 0, &m_sprite->GetFrame(animation, frame).vertex_array[0]);
    glColorPointer(3, GL_FLOAT, 0, &SpriteManager::Instance()->error_color_array[0]);
    glPushMatrix();
        glTranslatef(x, y, 0.0f);
        glRotatef(angle, 0.0f, 0.0f, 1.0f);
        glScalef(scale_x, scale_y, 0.0f);
    glDrawArrays(GL_QUADS, 0, 4);
    glPopMatrix();
}
void mob2d_node::SetBlend(float r, float g, float b)
{
	GLfloat m_r = (GLfloat) r / 255,
			m_g = (GLfloat) g / 255,
			m_b = (GLfloat) b / 255;

	if(m_r > 1.0f) m_r = 1.0f;
	if(m_g > 1.0f) m_g = 1.0f;
	if(m_b > 1.0f) m_b = 1.0f;

	blend_color.red   = m_r;
	blend_color.green = m_g;
	blend_color.blue  = m_b;
}
void mob2d_node::StepFrame()
{
    if((frame >= m_sprite->GetMaxFrames(animation)-1) && loop)
        frame = 0;
    else
        frame++;
}
void mob2d_node::SetFrame(uint frame)
{
    if((frame >= m_sprite->GetMaxFrames(animation)-1) && loop)
        frame = 0;
    else
        this->frame = frame;
}
void mob2d_node::ReassignToSprite(pSprite sprite)
{
    m_sprite = sprite;
}
void mob2d_node::SetSprite(string sprite_name)
{
    m_sprite = SpriteManager::Instance()->GetSprite(sprite_name);
}
Mob2DBoundingCircle mob2d_node::GetBoundingCircle()
{
    Mob2DBoundingCircle bc;

    bc.radius = m_sprite->GetAnimation(animation)->GetBoundingCircleRadius();
    bc.x = x;
    bc.y = y;

    return bc;
}
Mob2Daabb mob2d_node::GetAABB()
{
    Mob2Daabb aabb;

    aabb.x = x;
    aabb.y = y;

    aabb.width  = m_sprite->GetAnimation(animation)->GetMaxWidth();
    aabb.height = m_sprite->GetAnimation(animation)->GetMaxHeight();

    return aabb;
}
