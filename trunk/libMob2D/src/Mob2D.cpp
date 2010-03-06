#include "Mob2D.h"

Mob2D*
Mob2D::m_pInstance = NULL;

void Mob2D::Init(uint window_width, uint window_height, uint view_width, uint view_height)
{
	SpriteManager::Instance()->Init();
	Mob2DRenderer::Instance()->Init(window_width, window_height, view_width, view_height);
	Mob2DLog::Instance();
}
void Mob2D::Deinit()
{
	SpriteManager::Instance()->Deinit();
	Mob2DRenderer::Instance()->Deinit();
}

//-----FUNCTIONS THAT REFERENCE Mob2DRenderer-----//
void Mob2D::MoveCamera(int x, int y)
{
    Mob2DRenderer::Instance()->MoveCamera(x, y);
}

void Mob2D::SetViewport(int width, int height)
{
    Mob2DRenderer::Instance()->SetView(width, height);
}

void Mob2D::FinalizeFrame()
{
    Mob2DRenderer::Instance()->Render();
}

//-----FUNCTIONS THAT REFERENCE SpriteManager-----//
void Mob2D::Cleanup()
{
    SpriteManager::Instance()->Cleanup();
}

void Mob2D::AddResource(string file)
{
    SpriteManager::Instance()->AddResource(file);
}

void Mob2D::DeleteSprite(string handle)
{
    SpriteManager::Instance()->DeleteSprite(handle);
}

void Mob2D::ClearNodes(string the_handle)
{
    SpriteManager::Instance()->ClearNodes(the_handle);
}

M2DNode Mob2D::AddNode(string handle)
{
    return SpriteManager::Instance()->AddNode(handle);
}

//-----LOG DUMP FUNCTIONS-----//
void Mob2D::DumpLogConsole()
{
    Mob2DLog::Instance()->ConsoleDump();
}
void Mob2D::DumpLogFile()
{
    Mob2DLog::Instance()->FileDump();
}

//-----UTILITY FUNCTIONS-----//
void Mob2D::SetClearColor(float r, float g, float b)
{
    GLfloat _r = (GLfloat)r / 255,
			_g = (GLfloat)g / 255,
			_b = (GLfloat)b / 255;

	if(_r > 1.0f) _r = 1.0f;
	if(_g > 1.0f) _g = 1.0f;
	if(_b > 1.0f) _b = 1.0f;

    glClearColor(_r, _g, _b, 0.0f);
}
Mob2DCoord Mob2D::GetWorldCoords(int x, int y)
{
    GLint viewport[4];
    GLfloat win_x, win_y, win_z;
    GLdouble pos_z;
    GLdouble modelview_mat[16];
    GLdouble projection_mat[16];

    glGetIntegerv(GL_VIEWPORT, viewport);
    glGetDoublev(GL_PROJECTION_MATRIX, projection_mat);
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview_mat);

    win_x = (float)x;
    win_y = (float)viewport[3] - (float)y;
    glReadPixels(x, int(win_y), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &win_z);

    Mob2DCoord coord;
    gluUnProject(win_x, win_y, win_z, modelview_mat, projection_mat, viewport, &coord.x, &coord.y, &pos_z);
    return coord;
}
