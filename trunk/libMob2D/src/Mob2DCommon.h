#ifndef Mob2DCOMMON_H
#define Mob2DCOMMON_H

/**
This is the main resource header. This contains the following:
	* Includes for
		- OpenGL.
		- STL containers (including string).
	* Utility functions.
	* Typedefs for numerous default data types as well as some STL objects.
	* Apropriate Licensing Information for open source libraries used in this engine as well as Licensing for this library itself.
*/

/* PRIORITIZED TODO AND FIXME LIST */
//
// TODO Impliment shaders.
//
// TODO Replace the OpenGL matrix management with Kazmath.
//
// TODO Very Low Priority: Create resource tools for subdividing spritesheets, creating particle systems, etc...
//      Have someone else do that.

#define Mob2D_USE_VERT_ARRAYS
// #define Mob2D_USE_VBO

//-----STANDARD-----//
#include <map>
#include <list>
#include <ctime>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

#include <boost/shared_ptr.hpp>
#include <boost/ptr_container/ptr_map.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

// Drew a weird flashing box at the bottom of the screen, but otherwise rendered everything fine.
#include <boost/unordered_map.hpp> // Tested this out. Didn't work well.

//-----OpenGL-----//
#include "GLee.h"
#include <GL/gl.h>
#include <GL/glu.h>

typedef std::string string;

struct strsrt
{
    bool operator()(const string str1, const string str2) const
    { return strcmp(str1.c_str(), str2.c_str()) < 0; }
};

#ifndef Mob2DLOG_H
#define Mob2DLOG_H
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
		void PushString(string _string);

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
#endif // ENGINELOG_H
#endif // Mob2DCOMMON_H

/*

*/
