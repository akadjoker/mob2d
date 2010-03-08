#include "Mob2D.h"
/*
Mob2D*
Mob2D::m_pInstance = NULL;
*/

namespace Mob2D {

#ifdef _cplusplus
extern "C" {
#endif

Mob2DCoord m2dGetWorldCoords(int x, int y)
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
void m2dInit(uint window_width, uint window_height, uint view_width, uint view_height)
{
    SpriteManager::Instance()->Init();
	Mob2DRenderer::Instance()->Init(window_width, window_height, view_width, view_height);
	Mob2DLog::Instance();
}
void m2dDeinit()
{
    SpriteManager::Instance()->Deinit();
	Mob2DRenderer::Instance()->Deinit();
}
void m2dMoveCamera(int x, int y)
{
    Mob2DRenderer::Instance()->MoveCamera(x, y);
}
void m2dSetViewport(int width, int height)
{
    Mob2DRenderer::Instance()->SetView(width, height);
}

void m2dFinalizeFrame()
{
    Mob2DRenderer::Instance()->Render();
}

void m2dCleanup()
{
    SpriteManager::Instance()->Cleanup();
}

void m2dAddResource(string file)
{
    SpriteManager::Instance()->AddResource(file);
}

void m2dDeleteSprite(string handle)
{
    SpriteManager::Instance()->DeleteSprite(handle);
}

void m2dClearNodes(string handle)
{
    SpriteManager::Instance()->ClearNodes(handle);
}

M2DNode m2dAddNode(string handle)
{
    return SpriteManager::Instance()->AddNode(handle);
}

void m2dSetClearColor(float r, float g, float b)
{
    GLfloat _r = (GLfloat)r / 255,
			_g = (GLfloat)g / 255,
			_b = (GLfloat)b / 255;

	if(_r > 1.0f) _r = 1.0f;
	if(_g > 1.0f) _g = 1.0f;
	if(_b > 1.0f) _b = 1.0f;

    glClearColor(_r, _g, _b, 0.0f);
}
void m2dDumpLogConsole()
{
    Mob2DLog::Instance()->ConsoleDump();
}
void m2dDumpLogFile()
{
    Mob2DLog::Instance()->FileDump();
}

#ifdef _cplusplus
}
#endif

} // m2d namespace
