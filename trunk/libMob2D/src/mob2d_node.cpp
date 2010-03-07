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
    layer = 0.0f;

    animation = "ERROR";

    draw_to_screen = false;
    loop = draw = true;

	SetBlend(111, 111, 111);
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
    // For now, get the fixed function drawing working again then impliment the shader usage in another draw function.
    if(!m_sprite->error())
    {
        if(!m_sprite->shader_enabled)
            DrawFixedFunction();
        else
            DrawShader();
    }
    // Else the sprite is an error. It can't be rendered.
}
void mob2d_node::DrawFixedFunction()
{
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

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_BUFFER);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glBindBuffer(GL_ARRAY_BUFFER, m_sprite->GetFrame(animation,frame).vertex_buff);
    glVertexPointer(3, GL_FLOAT, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_sprite->GetFrame(animation,frame).texture_coords_buff);
    glTexCoordPointer(2, GL_FLOAT, 0, 0);

    glPushMatrix();

        glTranslatef(x, y, layer);
        glRotatef(angle, 0.0f, 0.0f, 1.0f);
        glScalef(scale_x, scale_y, 0.0f);

        glDrawArrays(GL_QUADS, 0, 4);

    glPopMatrix();

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_BUFFER);
}
void mob2d_node::DrawShader()
{
    // 1 -> m2d_vertex
    // 2 -> m2d_texcoord
    // 3 -> m2d_blendcolor

    //GLfloat mp_mat[16];

    glEnable(GL_DEPTH_BUFFER);

    m_sprite->shader.bindShader();

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, m_sprite->GetFrame(animation,frame).vertex_buff);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_sprite->GetFrame(animation,frame).texture_coords_buff);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, blend_color);

    glPushMatrix();

        glTranslatef(x, y, layer);
        glRotatef(angle, 0.0f, 0.0f, 1.0f);
        glScalef(scale_x, scale_y, 0.0f);

        // glGetFloatv(GL_MODELVIEW_PROJECTION_NV, mp_mat);
        // m_sprite->shader.sendUniform4x4("m2d_mp_matrix", mp_mat);

        m_sprite->shader.sendUniform("texture0", 0);

        glDrawArrays(GL_QUADS, 0, 4);

    glPopMatrix();

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);

//    glUseProgram(0);

    glDisable(GL_DEPTH_BUFFER);
}
void mob2d_node::DrawToScreen()
{/*
    if(!m_sprite->error())
    {
        Mob2DCoord coord = Mob2D::API()->GetWorldCoords(x, y);

        glTexCoordPointer(2, GL_FLOAT, 0, m_sprite->GetFrame(animation, frame).texture_coords);
        glVertexPointer(2, GL_FLOAT, 0, m_sprite->GetFrame(animation, frame).vertex_array);

        glPushMatrix();

            glTranslatef(coord.x, coord.y, 1.0f);
            glRotatef(angle, 0.0f, 0.0f, 1.0f);
        // Scale value = camera_view_size / window_size
            glScalef(Mob2DRenderer::Instance()->camera_data[2]/Mob2DRenderer::Instance()->window[0],
                     Mob2DRenderer::Instance()->camera_data[3]/Mob2DRenderer::Instance()->window[1], 0.0f);
            glDrawArrays(GL_QUADS, 0, 4);

        glPopMatrix();
    }
    */
}

void mob2d_node::SetBlend(float r, float g, float b)
{
	GLfloat m_r = (GLfloat) r / 255,
			m_g = (GLfloat) g / 255,
			m_b = (GLfloat) b / 255;

	if(m_r > 1.0f) m_r = 1.0f;
	if(m_g > 1.0f) m_g = 1.0f;
	if(m_b > 1.0f) m_b = 1.0f;

	blend_color[0] = m_r;
	blend_color[1] = m_g;
	blend_color[2] = m_b;
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
void mob2d_node::SetLayer(GLfloat layer)
{
    this->layer = layer;
}
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
