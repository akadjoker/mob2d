#include "Mob2D.h"
/*
Mob2D*
Mob2D::m_pInstance = NULL;
*/

namespace m2d {

#ifdef _cplusplus
extern "C" {
#endif

Mob2DCoord GetWorldCoords(int x, int y)
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
void Init(uint window_width, uint window_height, uint view_width, uint view_height)
{
    SpriteManager::Instance()->Init();
	Mob2DRenderer::Instance()->Init(window_width, window_height, view_width, view_height);
	Mob2DLog::Instance();
}
void Deinit()
{
    SpriteManager::Instance()->Deinit();
	Mob2DRenderer::Instance()->Deinit();
}
void MoveCamera(int x, int y)
{
    Mob2DRenderer::Instance()->MoveCamera(x, y);
}
void SetViewport(int width, int height)
{
    Mob2DRenderer::Instance()->SetView(width, height);
}

void FinalizeFrame()
{
    Mob2DRenderer::Instance()->Render();
}

void Cleanup()
{
    SpriteManager::Instance()->Cleanup();
}

void AddResource(string file)
{
    SpriteManager::Instance()->AddResource(file);
}

void DeleteSprite(string handle)
{
    SpriteManager::Instance()->DeleteSprite(handle);
}

void ClearNodes(string handle)
{
    SpriteManager::Instance()->ClearNodes(handle);
}

M2DNode AddNode(string handle)
{
    return SpriteManager::Instance()->AddNode(handle);
}

void SetClearColor(float r, float g, float b)
{
    GLfloat _r = (GLfloat)r / 255,
			_g = (GLfloat)g / 255,
			_b = (GLfloat)b / 255;

	if(_r > 1.0f) _r = 1.0f;
	if(_g > 1.0f) _g = 1.0f;
	if(_b > 1.0f) _b = 1.0f;

    glClearColor(_r, _g, _b, 0.0f);
}
void DumpLogConsole()
{
    Mob2DLog::Instance()->ConsoleDump();
}
void DumpLogFile()
{
    Mob2DLog::Instance()->FileDump();
}

#ifdef _cplusplus
}
#endif

} // m2d namespace
