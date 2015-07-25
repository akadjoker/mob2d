# Simplicity #

Here, I will show you just how easy it is to create a working program with Mob2D. I will go over the main concepts behind the engine and how you can best utilize its powerful, yet simple interface.


# Tutorial #

Mob2D was designed to provide a minimal footprint. As a result, it relies on a simplistic precaching system to handle its resources. It also provides a powerful and flexible method of creating animations through XML. The best way to explain is to jump right into it. Here, we are going to recreate the example program that can be found on the SVN. You will need either the GLFW library to create an OpenGL window or you will need to do that yourself through your operating system's API. I will not go over that here, as that is beyond this tutorial's scope.

## The Code ##

Once we have a window with an OpenGL context, we need to enable Mob2D. We do that with one simple function call:

```
m2dInit();
```

See? That wasn't so hard. The engine automatically sets up it's default "error" resources and everything is ready for you. Now, let's get some resources loaded!

```
// This is taken straight out of the "main" in the example application!
m2dAddResource("resources/image_def.xml");
```

In Mob2D, the only resources you need to concern yourself with are the sprite definition files that provide all the data the engine needs. I will go over sprite definitions in another tutorial, or you can look at the ones in the example application found in the SVN trunk.

When you add a resource, the engine automagically loads the spritesheet image and defines the animations, ready for you to exploit! The problem is that this is only a precaching function. No rendering is done until you add what are called "M2DNodes" or simply "Nodes". Nodes are the objects that are actually rendered by the engine, and enable you to translate and rotate them. If you are more advanced in C++, think of them as pointers to an internally cached node. How do we add nodes? Simple, one small function does this for you.

```
// We always want to store a copy of the node for our own use. Otherwise we won't be able to do anything with it!
M2DNode app_node = m2dAddNode("Test_Image");
```

IT'S SO SIMPLE I CAN HARDLY STAND IT! That is literally all there is to it! The string we passed to m2dAddNode() was the name of the sprite we want the node to work with. This string was defined in image\_def.xml. Take a look at it for more information. Again, I will be providing a tutorial on more advanced resource management later on.

But wait! We have a problem! When we add a node, the node will not be able to render anything right off the bat. When we add a node, the node automatically sets itself to a blank animation. This is called the nodes "default state", or the state that prevents it from pointing to any null values that could potentially bring the whole engine down. So lets set the animation! How easy is that? Here, I'll show you.

```
// This is why you want to store a copy! Nodes carry with them a set of functions that allow you to manipulate them. m2ob2d_node::SetAnimation() allows you to specify which animation is played.
app_node->SetAnimation("letters");
```

Now that we have our node and we have set the animation, we are ready to enter our main program loop. Here I will provide the rest of the example program.

```
    while(running)
    {
        glClear(GL_COLOR_BUFFER_BIT);

        frame++;

        glfwGetWindowSize(&_width, &_height);

        _height = _height > 0 ? _height : 1;

        // Basic framerate limitation. Do not concern yourself with this for now.
        loops = 0;
        // Custom implementation of GetTickCount() for linux will be provided.
        while((GetTickCount() > next_game_tick) && (loops < MAX_FRAMESKIP))
        {
            // This node function steps through the frames, automatically looping when the last frame is reached (as long as the loop flag is set. More on that later).
            app_node->StepFrame();
            app_node1->StepFrame();
            app_node2->StepFrame();
            app_node3->StepFrame();
            app_node4->StepFrame();

            next_game_tick += SKIP_TICKS;
            loops++;
        }

        //-----INSERT Mob2D RENDERING CODE HERE-----//
                // The magic function! This function must be called EVERY FRAME otherwise nothing will be drawn. This function renders all the active nodes.
		m2dFinalizeFrame(/*Interpolation will be implemented soon!*/);

        glfwSwapBuffers();

    // exit if ESC was pressed or window was closed
        running = (!glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED));
    }
```

When we are done, we simply call one function and everything will be cleaned up for you.

```
// Goodbye
m2dDeinit();
```
All resources will be cleaned up for you!

Mob2D was designed to be lightweight and simple to use. With this, you can spend less time worrying about OpenGL and more time creating great games.

# More Advanced Concepts #
Nothing here yet. I will provide more tutorials later.