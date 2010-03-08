#ifndef Mob2DCOMMON_H
#define Mob2DCOMMON_H

/**
This is the main resource header. This contains the following:
	* Includes for
		- OpenGL.
		- STL containers (including string).
	* Utility functions.
	* Apropriate Licensing Information for open source libraries used in this engine as well as Licensing for this library itself.

Contact me at 0rrm1k@gmail.com if you have any questions or meet me on irc. I can usually be found on:

EFnet    @ #c++, #opengl, #opengl2d
Afternet @ #gamedev
*/

/* PRIORITIZED TODO AND FIXME LIST */
//
// TODO Impliment shaders. Working on it!
//
// TODO Replace the OpenGL matrix management with Kazmath. Whenever I get around to it.
//
// TODO Very Low Priority: Create resource tools for subdividing spritesheets, creating particle systems, etc...
//      CONTACT ME IF YOU WANT TO DO THIS!
// Review skeleton and bones of the skeleton.
//-----STANDARD-----//
#include <map>
#include <list>
#include <vector>
#include <string>
#include <fstream>
#include <string.h> // FINE! WE'LL DO IT YOUR WAY!
#include <iostream>

#include <boost/shared_ptr.hpp>

#include <boost/unordered_map.hpp> // Tested this out. Didn't work well.

//-----OpenGL-----//
#include "GLee.h"
#include <GL/gl.h>
#include <GL/glu.h>

namespace Mob2D {

typedef std::string string;

struct strsrt
{
    bool operator()(const string str1, const string str2) const
    { return strcmp(str1.c_str(), str2.c_str()) < 0; }
};


/**
Mob2DCoord:
A coordinate pair used specifically by GetWorldCoords(int win_x, int win_y);
*/
struct Mob2DCoord
{
    GLdouble x;
    GLdouble y;
};


#ifdef _cplusplus
extern "C" {
#endif

// Used for draw to screen functionality among other things.
Mob2DCoord GetWorldCoords(int, int);

#ifdef _cplusplus
}
#endif

// General enum for engine.
enum M2Denum
{
    M2D_DRAWTOSCREEN,
    M2D_LOOP,
    M2D_DRAW
};


/**
Mob2DLog:
This class stores a list of std::strings that are then used by a separate console class. The two classes work together
to provide valuable debugging information.
*/
class Mob2DLog
{
	public:
		static Mob2DLog* Instance() { if(!m_pInstance) m_pInstance = new Mob2DLog; return m_pInstance; }

    /// Pushes a message string into the internal list of messages.
		void PushString(const string& error_string);

    /// Dumps the message list into the console.
		void ConsoleDump();

    /// Dumps the message list into a file. The file name is "Mob2DLog_" and then the date it was generated.
		void FileDump();

	protected:
		Mob2DLog(){};
		Mob2DLog& operator=(Mob2DLog const&){};
		virtual ~Mob2DLog();
		static Mob2DLog* m_pInstance;

		std::list<string> error_list;
};

} // namespace
#endif // Mob2DCOMMON_H
