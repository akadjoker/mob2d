// #define WINDOWS // uncomment this to build on windows.
#define LINUX // comment this out to build on windows.


// The only include you need for the system
#include "../libMob2D/src/Mob2D.h"
#include <GL/glfw.h>

#ifdef LINUX
    #include <sys/times.h>
#endif
#ifdef WINDOWS
    #include <windows.h>
#endif

using namespace Mob2D;

#ifdef LINUX
uint32_t GetTickCount(void)
{
    tms tm;
    return times(&tm)*10;
}
#endif

const int TICKS_PER_SECOND = 30; // This is how many times per second DARSNode::StepFrame() will be called.
const int SKIP_TICKS = 1000 / TICKS_PER_SECOND;
const int MAX_FRAMESKIP = 5;

#ifdef LINUX
    uint32_t next_game_tick = GetTickCount();
#endif
#ifdef WINDOWS
    DWORD next_game_tick = GetTickCount();
#endif

int loops;

int main()
{
    int     _width, _height;
    int     frame = 0;
    bool    running = true;

    glfwInit();

    if(!glfwOpenWindow(800, 600, 0, 0, 0, 0, 0, 0, GLFW_WINDOW))
    {
        glfwTerminate();
        return 0;
    }
    glfwSetWindowTitle("Mob2D Test Application");

    //-----INITALIZE Mob2D AND LOAD A RESOURCE-----//
	m2dInit(800, 600, 700, 500); // 800x600 window

	// Add a sprite markup to the internal cache.
	m2dAddResource("resources/image_def.xml");
    m2dAddResource("resources/zero_def.xml");

	m2dSetClearColor(0.0f, 0.0f, 0.0f);

    // Create app nodes and assign it to the image resources stored by the system.
    // This is just a quick and dirty demonstration. I need to make a nicer one.
	M2DNode app_node  = m2dAddNode("Test_Image");
	M2DNode app_node1 = m2dAddNode("Zero");
	M2DNode app_node2 = m2dAddNode("Test_Image");
	M2DNode app_node3 = m2dAddNode("Zero");
    M2DNode app_node4 = m2dAddNode("Test_Image");

    // The lower the layer number, the more things it will be drawn over.
    // The value can be anywhere between -9001 and 9001
    // Nodes default to layer 0.0f
    //app_node3->SetLayer(-20.0f);

    // Move the sprite relative to the top left.
	app_node ->Translate(100, 300); // left side
    app_node1->Translate(400, 100); // top side
    app_node2->Translate(700, 300); // right side
    app_node3->Translate(400, 500); // bottom side
    app_node4->Translate(400, 300); // bottom. this should be bellow the zero image.*/

    // The lower the layer, the more it is guaranteed to be rendered on top of other objects.
    // app_node4->SetLayer(-1.0f);
    app_node3->SetLayer(-1000.0f);

	// If an animation is not set for a node, then it will not display.
	// Unset nodes default to the "ERROR" animation. Same thing occurs when an animation string is invalid.
	app_node->SetAnimation("letters");
	app_node1->SetAnimation("Transport_Zero");
	app_node2->SetAnimation("letters");
	app_node3->SetAnimation("Transport_Zero");
	app_node4->SetAnimation("letters");

	m2dLoadShader("resources/zero_shader.xml");

	app_node1->SetShader("Zero_Shader");
	app_node1->EnableShader();
	// app_node1->SetBlend(255, 0, 0);

    // Dump the log to the console so we can see if anything went wrong.
	m2dDumpLogConsole();

	app_node4->SetFlag(M2D_DRAWTOSCREEN);

    while(running)
    {
        glClear(GL_COLOR_BUFFER_BIT);

        frame++;

        glfwGetWindowSize(&_width, &_height);

        _height = _height > 0 ? _height : 1;

        // Basic framerate limitation.
        loops = 0;
        while((GetTickCount() > next_game_tick) && (loops < MAX_FRAMESKIP))
        {
            app_node->StepFrame();
            app_node1->StepFrame();
            app_node2->StepFrame();
            app_node3->StepFrame();
            app_node4->StepFrame();

            next_game_tick += SKIP_TICKS;
            loops++;
        }

        //-----INSERT Mob2D RENDERING CODE HERE-----//
		m2dFinalizeFrame(/*Interpolation will be implimented soon!*/);

        glfwSwapBuffers();

    // exit if ESC was pressed or window was closed
        running = (!glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED));
    }
    // Mob2D::API()->DumpLogFile();
    m2dDeinit();

    glfwTerminate();

    return 0;
}
