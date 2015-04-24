#include <SDL.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include "NGLDraw.h"
#include <ngl/NGLInit.h>
#include "MovingEntity.h"

#include "Timer.h"

#include "Vehicle.h"
#include "GameWorld.h"
#include <ngl/Transformation.h>

#include "Rioter.h"
#include "Police.h"


/// @brief function to quit SDL with error message
/// @param[in] _msg the error message to send
void SDLErrorExit(const std::string &_msg);

/// @brief initialize SDL OpenGL context
SDL_GLContext createOpenGLContext( SDL_Window *window);



int main()
{
  srand(static_cast <unsigned> (time(0)));


  // Initialize SDL's Video subsystem
  if (SDL_Init(SDL_INIT_VIDEO) < 0 )
  {
    // Or die on error
    SDLErrorExit("Unable to initialize SDL");
  }

  // now get the size of the display and create a window we need to init the video
  SDL_Rect rect;
  SDL_GetDisplayBounds(0,&rect);
  // now create our window
  SDL_Window *window=SDL_CreateWindow("Colossal-Riot",
                                      SDL_WINDOWPOS_CENTERED,
                                      SDL_WINDOWPOS_CENTERED,
                                      1280,
                                      720,
                                      SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
                                     );
  // check to see if that worked or exit
  if (!window)
  {
    SDLErrorExit("Unable to create window");
  }

  // Create our opengl context and attach it to our window

   SDL_GLContext glContext=createOpenGLContext(window);
   if(!glContext)
   {
     SDLErrorExit("Problem creating OpenGL context");
   }
   // make this our current GL context (we can have more than one window but in this case not)
   SDL_GL_MakeCurrent(window, glContext);
  /* This makes our buffer swap syncronized with the monitor's vertical refresh */
  SDL_GL_SetSwapInterval(1);
  // we need to initialise the NGL lib which will load all of the OpenGL functions, this must
  // be done once we have a valid GL context but before we call any GL commands. If we dont do
  // this everything will crash
  ngl::NGLInit::instance();
  // now clear the screen and swap whilst NGL inits (which may take time)
  glClear(GL_COLOR_BUFFER_BIT);
  SDL_GL_SwapWindow(window);
  // flag to indicate if we need to exit
  bool quit=false;
  // sdl event processing data structure
  SDL_Event event;
  // now we create an instance of our ngl class, this will init NGL and setup basic
  // opengl stuff ext. When this falls out of scope the dtor will be called and cleanup
  // our gl stuff
  NGLDraw ngldraw;
  // resize the ngl to set the screen size and camera stuff
  ngldraw.resize(rect.w,rect.h);

  bool paused = 0;

  Timer gameTimer;
  double timeElapsed = 0.0;
  double currentTime = 0.0;
//  GameWorld* world;
//  Vehicle* veh = new Vehicle(world, ngl::Vec3(5,0,5), ngl::Vec3(1,1,1), 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);


//  veh->Steering()->SeekOn();
//  veh->Steering()->ArriveOff();
//  veh->Steering()->FleeOff();
//-------MAIN LOOP----------------------------------------------------------------------

//  GameWorld *MyWorld;
//  Rioter test_rioter(MyWorld);
//  Police test_police(MyWorld);

  while(!quit)
  {

    std::cout<<"------------- TICK -------------"<<std::endl;

    std::cout<<"Current Time  = "<<gameTimer.getCurrentTime()<<std::endl;
    std::cout<<"Time Elapsed  = "<<timeElapsed<<std::endl;



    ngldraw.update(timeElapsed,currentTime);


    while ( SDL_PollEvent(&event) )
    {
      switch (event.type)
      {
        // this is the window x being clicked.
        case SDL_QUIT : quit = true; break;

        // process the mouse data by passing it to ngl class

        case SDL_MOUSEMOTION : ngldraw.mouseMoveEvent(event.motion);break;
        case SDL_MOUSEBUTTONDOWN : ngldraw.mousePressEvent(event.button); break;
        case SDL_MOUSEBUTTONUP : ngldraw.mouseReleaseEvent(event.button); break;
        case SDL_MOUSEWHEEL : ngldraw.wheelEvent(event.wheel); break;

        // if the window is re-sized pass it to the ngl class to change gl viewport
        // note this is slow as the context is re-create by SDL each time
        case SDL_WINDOWEVENT :
          int w,h;
          // get the new window size
          SDL_GetWindowSize(window,&w,&h);
          ngldraw.resize(w,h);
        break;


        // now we look for a keydown event
        case SDL_KEYDOWN:
        {
          switch( event.key.keysym.sym )
          {
            // if it's the escape key quit
            case SDLK_ESCAPE :  quit = true; break;
            case SDLK_w : glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); break;
            case SDLK_s : glPolygonMode(GL_FRONT_AND_BACK,GL_FILL); break;
            case SDLK_f :
            SDL_SetWindowFullscreen(window,SDL_TRUE);
            glViewport(0,0,rect.w,rect.h);
            break;


            case SDLK_p : paused = !paused; gameTimer.resetTimer(); break;
            case SDLK_t : std::cout<<gameTimer.getCurrentTime()<<std::endl; break;
            case SDLK_r : std::cout<<"reset"<<std::endl; gameTimer.resetTimer(); break;

            case SDLK_g : SDL_SetWindowFullscreen(window,SDL_FALSE); break;
            default : break;
          } // end of key process
        } // end of keydown

        default : break;
      } // end of event switch
    } // end of poll events

    if(paused == 0)
    {
    timeElapsed=gameTimer.timeElapsed();
    currentTime=gameTimer.getCurrentTime();

    std::cout<<"------------- TICK -------------"<<std::endl;
    ngldraw.update(timeElapsed,currentTime);

    }
    // now we draw ngl
    ngldraw.draw();
    // swap the buffers
    SDL_GL_SwapWindow(window);

//--------------------------------------------------------------------------------------------------------------------

  }
  // now tidy up and exit SDL
 SDL_Quit();
}


SDL_GLContext createOpenGLContext(SDL_Window *window)
{
  // Request an opengl 3.2 context first we setup our attributes, if you need any
  // more just add them here before the call to create the context
  // SDL doesn't have the ability to choose which profile at this time of writing,
  // but it should default to the core profile
  // for some reason we need this for mac but linux crashes on the latest nvidia drivers
  // under centos
  #ifdef DARWIN
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
  #else

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
  #endif
  // set multi sampling else we get really bad graphics that alias
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,4);
  // Turn on double buffering with a 24bit Z buffer.
  // You may need to change this to 16 or 32 for your system
  // on mac up to 32 will work but under linux centos build only 16
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
  // enable double buffering (should be on by default)
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  //
  return SDL_GL_CreateContext(window);

}

void SDLErrorExit(const std::string &_msg)
{
  std::cerr<<_msg<<"\n";
  std::cerr<<SDL_GetError()<<"\n";
  SDL_Quit();
  exit(EXIT_FAILURE);
}
