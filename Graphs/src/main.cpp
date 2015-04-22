#include <SDL.h>
#include <cstdlib>
#include "NGLDraw.h"
#include <ngl/NGLInit.h>
#include "cellGraph.h"

#include <fstream>
#include <sstream>
#include <glm/ext.hpp>
#include <string>


#include <ngl/Transformation.h>


/// @brief function to quit SDL with error message
/// @param[in] _msg the error message to send
void SDLErrorExit(const std::string &_msg);

/// @brief initialize SDL OpenGL context
SDL_GLContext createOpenGLContext( SDL_Window *window);




int main()
{
    cellGraph benGraph("plane_Test.obj");
    benGraph.printCellGraph();

}
