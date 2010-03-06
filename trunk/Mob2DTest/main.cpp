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

#ifdef LINUX
uint32_t GetTickCount(void)
{
    tms tm;
    return times(&tm)*10;
}
#endif

const int TICKS_PER_SECOND = 10; // This is how many times per second DARSNode::StepFrame() will be called.
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
	Mob2D::API()->Init(800, 600, 800, 600); // 800x600 window

	// Add a sprite markup to the internal cache.
	Mob2D::API()->AddResource("resources/image_def.xml");
    Mob2D::API()->AddResource("resources/zero_def.xml");

	Mob2D::API()->SetClearColor(0.0f, 0.0f, 0.0f);

    // Create app nodes and assign it to the image resources stored by the system.
    // This is just a quick and dirty demonstration. I need to make a nicer one.
	M2DNode app_node  = Mob2D::API()->AddNode("Test_Image");
	M2DNode app_node1 = Mob2D::API()->AddNode("Zero");
	M2DNode app_node2 = Mob2D::API()->AddNode("Test_Image");
	M2DNode app_node3 = Mob2D::API()->AddNode("Zero");
    M2DNode app_node4 = Mob2D::API()->AddNode("Test_Image");

    // Example of using the error flag through the node.
    if(!app_node->isSpriteError()) { std::cout<<"No error.\n"; }
	else { std::cout<<"Error.\n"; }

    // Move the sprite relative to the top left.
	app_node->Translate(100, 300); // left side
    app_node1->Translate(400, 100); // top side
    app_node2->Translate(700, 300); // right side
    app_node3->Translate(400, 500); // bottom side
    app_node4->Translate(9001, 9001); // center*/

	// If an animation is not set for a node, then it will not display.
	// Unset nodes default to the "ERROR" animation. Same thing occurs when an animation string is invalid.
	app_node->SetAnimation("letters");
	app_node1->SetAnimation("Transport_Zero");
	app_node2->SetAnimation("letters");
	app_node3->SetAnimation("Transport_Zero");
	app_node4->SetAnimation("letters");

    // Dump the log to the console so we can see if anything went wrong.
	Mob2D::API()->DumpLogConsole();

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
		Mob2D::API()->FinalizeFrame(/*Interpolation will be implimented soon!*/);

        glfwSwapBuffers();

    // exit if ESC was pressed or window was closed
        running = (!glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED));
    }
    // Mob2D::API()->DumpLogFile();
    Mob2D::API()->Deinit();

    glfwTerminate();

    return 0;
}
