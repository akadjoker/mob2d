#include "mob2d_node.h"

#include "SpriteManager.h"
#include "Mob2DRenderer.h"
#include "Mob2D.h"

namespace Mob2D {

mob2d_node::mob2d_node(pSprite sprite)
{
    angle = 0.0f;
	m_sprite = sprite;
	frame = x = y = 0;
    scale_x = scale_y = 1.0f;
    layer = 0.0f;

    animation = shader = "ERROR";

    draw_to_screen = false;
    loop = draw = true;

	SetBlend(255, 255, 255);
}
void mob2d_node::SetAnimation(const string& anim)
{
    if(m_sprite->animations.find(anim) != m_sprite->animations.end())
        animation = anim;
    else
        animation = "ERROR";
}
void mob2d_node::SetBlend(float r, float g, float b)
{
	GLfloat m_r = (GLfloat) r / 255,
			m_g = (GLfloat) g / 255,
			m_b = (GLfloat) b / 255;

	if(m_r > 1.0f) m_r = 1.0f;
	if(m_g > 1.0f) m_g = 1.0f;
	if(m_b > 1.0f) m_b = 1.0f;

	blend_color[0] = blend_color[3] = blend_color[6] = blend_color[9]  = m_r;
	blend_color[1] = blend_color[4] = blend_color[7] = blend_color[10] = m_g;
	blend_color[2] = blend_color[5] = blend_color[8] = blend_color[11] = m_b;
}
void mob2d_node::StepFrame()
{
    if(loop)
        if((frame >= m_sprite->GetMaxFrames(animation)-1))
            frame = 0;
        else
            frame++;
    else
        frame = frame;
}
void mob2d_node::SetFrame(uint frame)
{
    if((frame >= m_sprite->GetMaxFrames(animation)-1))
        frame = 0;
    else
        this->frame = frame;
}
void mob2d_node::ReassignToSprite(pSprite sprite)
{
    m_sprite = sprite;
}
void mob2d_node::SetSprite(const string& sprite_name)
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
void mob2d_node::SetLayer(GLfloat layer)
{
    this->layer = layer;
}
void mob2d_node::SetFlag(M2Denum val)
{
    switch(val)
    {
        case M2D_DRAWTOSCREEN:
            draw_to_screen = !draw_to_screen;
        break;
        case M2D_LOOP:
            loop = !loop;
        break;
        case M2D_DRAW:
            draw = !draw;
        break;
    }
}
bool mob2d_node::GetFlag(M2Denum val)
{
    switch(val)
    {
        case M2D_DRAWTOSCREEN:
            return draw_to_screen;
        case M2D_LOOP:
            return loop;
        case M2D_DRAW:
            return draw;
    }
}
void mob2d_node::SetShader(const string& shader)
{
    this->shader = shader;
}
void mob2d_node::EnableShader()
{
    shader_enabled = true;
}
void mob2d_node::DisableShader()
{
    shader_enabled = false;
}


} // namespace

/*
    GLfloat width = m_sprite->GetAnimation(animation)->max_width,
            height = m_sprite->GetAnimation(animation)->max_height;

    GLfloat varray[12];

    varray[0] = -width; varray[1]  = height;  varray[2]  = 0.0f; // lower left
    varray[3] = width;  varray[4]  = height;  varray[5]  = 0.0f; // lower right
    varray[6] = width;  varray[7]  = -height; varray[8]  = 0.0f; // upper right
    varray[9] = -width; varray[10] = -height; varray[11] = 0.0f; // upper left
// */
// VBO
    // glUseProgram(0); // required

// DONE WITH THE TEXTURE_DRAWING

/*
// vvv NOW DRAW THE BOUNDING VOLUME vvv
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    glPushMatrix();
        glBindBuffer(GL_ARRAY_BUFFER, Mob2DRenderer::Instance()->debug_color);
        glColorPointer(3, GL_FLOAT, 0, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glVertexPointer(3, GL_FLOAT, 0, varray);

        glTranslatef(x, y, layer);
        glDrawArrays(GL_LINE_LOOP, 0, 4);
    glPopMatrix();

    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
// */
