#ifndef Mob2D_H
#define Mob2D_H

#include "Mob2DCommon.h"
#include "Mob2DRenderer.h"

/**
Mob2D:
This is the main API class for the DAta Rendering System. It provides functions for working with the rendering
engine. The Primary goal of the Mob2D class is to make sure that the user does not need to call on any methods
of any of the engines internal classes. This class does not call on any OpenGL functions. It does that through its
internal classes.
*/

namespace m2d {

#ifdef _cplusplus
extern "C" {
#endif


void Init(uint, uint, uint, uint);

/// Deinitalize Mob2D.
void Deinit();

//-----FUNCTIONS THAT REFERENCE Mob2DRenderer-----//
/// Moves the camera to the given position relative to its current position.
void MoveCamera(int, int);

/// Set the viewport of the renderer.
void SetViewport(int, int);

/// Run the rendering routine. Call on the renderer to run its rendering routine. Does not swap the buffers.
void FinalizeFrame();

//-----FUNCTIONS THAT REFERENCE SpriteManager-----//
/// Cleans up the cache of nodes that need to be deleted.
void Cleanup();

/// Adds a sprite resource.
void AddResource(string);

/// Deletes a sprite and flags all nodes using it as errors.
void DeleteSprite(string);

/// Clears all the nodes using a particular resource.
void ClearNodes(string);

/// Adds a node to the internal cache of nodes and returns a copy for reference.
M2DNode AddNode(string);

//-----UTILITY FUNCTIONS-----//
/// Sets the clear color.
void SetClearColor(float, float, float);

// Given the screen coordinates, this function will return the world coordinates useful for screen
// rendering and mouse checking no matter what the viewport zoom is like.
// Mob2DCoord GetWorldCoords(int x, int y);

//-----LOG FUNCTIONS-----//
void DumpLogConsole();
void DumpLogFile();


#ifdef _cplusplus
}
#endif

} // m2d namespace
#endif // Mob2D_H
