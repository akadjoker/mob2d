#ifndef Mob2DRENDERER_H
#define Mob2DRENDERER_H

#include "Mob2DShader.h" // Includes Sprite.h and Mob2D_node.h
#include "Mob2DCommon.h"
#include "SpriteManager.h"

namespace m2d {
/**
Mob2DRenderer:
Uses OpenGL to render the scene. Its rendering process is described in the comments of VARender.
This only handles texture binding as far as the actual rendering process is concerned. Drawing is handled
by the Mob2D_node. This is a Meyers Singleton class initalized by Mob2D. Only the Mob2D class may call methods
of this class.
*/
class Mob2DRenderer
{
	public:
		static Mob2DRenderer* Instance() { if(!m_pInstance) m_pInstance = new Mob2DRenderer(); return m_pInstance; }

		void Init(uint window_width, uint window_height, uint view_width, uint view_height);
		void Deinit();

		void MoveCamera(int x, int y);
		void SetView(int width, int height);
		void SetWindowData(int width, int height);

		void Render();

	private:
		Mob2DRenderer(){};
		Mob2DRenderer(Mob2DRenderer const&){};
		Mob2DRenderer& operator=(Mob2DRenderer const&){};
		~Mob2DRenderer(){};
		static Mob2DRenderer* m_pInstance;

    /// Looks at the node then determines how best to draw it, delegating the responsibility to other functions.
        void DrawNode(M2DNode node);

		void DrawFixedFunction(M2DNode node);
		void DrawShader(M2DNode node);

    /// Camera position and viewport width/height. By default, the last two values are the same as the window. [x, y, width, height]
		int camera_data[4];
    /// Width and height of the actual window opened by the operating system.
		int window[2];

    /// BoundingCircle used to represent the viewport in the culling routine.
        Mob2DBoundingCircle m_bc;

    /// INTERNAL: Checks to see if a node is inside the viewport.
		bool isInView(M2DNode node);

		GLuint debug_color;

		GLfloat modelview_projection_matrix[16];

		friend class Mob2D;
		friend class mob2d_node;
};

} // namespace
#endif // Mob2DRENDERER_H
