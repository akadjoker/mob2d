#ifndef Mob2D_H
#define Mob2D_H

#include "Mob2DCommon.h"
#include "Mob2DRenderer.h"

/**
Mob2DCoord:
A coordinate pair used specifically by GetWorldCoords(int win_x. int win_y);
*/
struct Mob2DCoord
{
    GLdouble x;
    GLdouble y;
};

/**
Mob2D:
This is the main API class for the DAta Rendering System. It provides functions for working with the rendering
engine. The Primary goal of the Mob2D class is to make sure that the user does not need to call on any methods
of any of the engines internal classes. This class does not call on any OpenGL functions. It does that through its
internal classes. I personally would not use this class. I would use the classes themselves instead.
*/

class Mob2D
{
	public:
		static Mob2D* API() { if(!m_pInstance) m_pInstance = new Mob2D(); return m_pInstance; }

    /// Initalize Mob2D.
		void Init(uint window_width, uint window_height, uint view_width, uint view_height);

    /// Deinitalize Mob2D.
		void Deinit();

	//-----FUNCTIONS THAT REFERENCE Mob2DRenderer-----//
    /// Moves the camera to the given position relative to its current position.
		void MoveCamera(int x, int y);

    /// Set the viewport of the renderer.
		void SetViewport(int width, int height);

    /// Set the framerate of the renderer. This is how many times per second it updates the frame data of animated sprites.
		void SetFramerate(uint framerate);

    /// Run the rendering routine. Call on the renderer to run its rendering routine.
    /// Does not swap the buffers.
		void FinalizeFrame();

	//-----FUNCTIONS THAT REFERENCE SpriteManager-----//
    /// Cleans up the cache of nodes that need to be deleted.
		void Cleanup();

    /// Adds a sprite resource.
		void AddResource(string file);

    /// Deletes a sprite and flags all nodes using it as errors.
        void DeleteSprite(string handle);

    /// Clears all the nodes using a particular resource.
		void ClearNodes(string the_handle);

    /// Adds a node to the internal cache of nodes and returns a copy for reference.
		Mob2DNode AddNode(string handle);

    //-----UTILITY FUNCTIONS-----//
    /// Sets the clear color.
        void SetClearColor(float r, float g, float b);

    /// Given the screen coordinates, this function will return the world coordinates useful for screen
    /// rendering and mouse checking.
        Mob2DCoord GetWorldCoords(int x, int y);

    //-----LOG FUNCTIONS-----//
		void DumpLogConsole();
        void DumpLogFile();

	protected:
		Mob2D(){};
		Mob2D(Mob2D const&){};
		Mob2D& operator=(Mob2D const&){};  // assignment operator is private
		~Mob2D(){};

		static Mob2D* m_pInstance;
};
#endif // Mob2D_H
