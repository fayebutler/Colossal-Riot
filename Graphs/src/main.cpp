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



std::vector <cell> * gGraph;


void makeCells(const char *fileName)
{
    std::vector<ngl::Vec3> vertices;
    std::vector<ngl::Vec4> faces;

    std::vector <cell> graph;

    std::ifstream in(fileName, std::ios::in);
     if (!in) { std::cerr << "Cannot open " << fileName << std::endl; exit(1); }

///For each loop of while loop we make a cell from the faces of the .obj (must be quads)
     std::string line;
     while (getline(in, line))
     {
////Reading vertices list used for cells:
       if (line.substr(0,2) == "v ")
       {

         std::istringstream s(line.substr(2));
         ngl::Vec3 v; s >> v.m_x; s >> v.m_y; s >> v.m_z;
         vertices.push_back(v);
       }

////Reading indicies for corners of cells:
       else if (line.substr(0,2) == "f ")
       {
          std::istringstream s(line.substr(2));
          std::string rawVertId1, rawVertId2,rawVertId3,rawVertId4;

          s >> rawVertId1;
          s >> rawVertId2;
          s >> rawVertId3;
          s >> rawVertId4;

          std::string vertId1,vertId2,vertId3,vertId4;

          //Get rid of slashes in .obj files:
          for(int i =0; i<rawVertId1.length(); i++)
          {
            if (std::ispunct(rawVertId1[i]))
            {
                break;
            }
            vertId1 = vertId1+rawVertId1[i];
          }


          for(int i =0; i<rawVertId2.length(); i++)
          {
            if (std::ispunct(rawVertId2[i]))
            {
                break;
            }
            vertId2 = vertId2+rawVertId2[i];
          }


          for(int i =0; i<rawVertId3.length(); i++)
          {
            if (std::ispunct(rawVertId3[i]))
            {
                break;
            }
            vertId3 = vertId3+rawVertId3[i];
          }


          for(int i =0; i<rawVertId4.length(); i++)
          {
            if (std::ispunct(rawVertId4[i]))
            {
                break;
            }
            vertId4 = vertId4+rawVertId4[i];
          }


         //Append each vertId to a vector, making them into a vector of integers:
         int a = std::atoi(vertId1.c_str());
         int b =std::atoi(vertId2.c_str());
                 int c =std::atoi(vertId3.c_str());
                 int d = std::atoi(vertId4.c_str());


///Match indicies to vertices to make the cells:
        ngl::Vec4 newFace;
        newFace.m_x=a;
        newFace.m_y=b;
        newFace.m_z=c;
        newFace.m_w=d;


    faces.push_back(newFace);





        }


       else if (line[0] == '#') { /* ignoring this line */ }
       else { /* ignoring this line */ }
     }


///NOW ACTUALLY MAKE THE CELLS

     for (int j = 0;j<faces.size();j++)
     {
         std::vector<ngl::Vec3> fourCorners;

         fourCorners.push_back(vertices[(faces[j].m_x-1)]);
         fourCorners.push_back(vertices[(faces[j].m_y-1)]);
         fourCorners.push_back(vertices[(faces[j].m_z-1)]);
         fourCorners.push_back(vertices[(faces[j].m_w-1)]);


        //FIND NEIGHBOURS

        std::vector<int> neighbourIDs;

         for (int i=0 ;i<faces.size();i++)
         {
             // j is the cell (face) we are creating. i is cell we are checking against.
            if (faces[j].m_x == faces[i].m_x || faces[j].m_y == faces[i].m_x ||
                    faces[j].m_z == faces[i].m_x || faces[j].m_w == faces[i].m_x||

                    faces[j].m_x == faces[i].m_y || faces[j].m_y == faces[i].m_y ||
                                        faces[j].m_z == faces[i].m_y || faces[j].m_w == faces[i].m_y||

                    faces[j].m_x == faces[i].m_z || faces[j].m_y == faces[i].m_z ||
                                        faces[j].m_z == faces[i].m_z || faces[j].m_w == faces[i].m_z||

                    faces[j].m_x == faces[i].m_w || faces[j].m_y == faces[i].m_w ||
                                        faces[j].m_z == faces[i].m_w || faces[j].m_w == faces[i].m_w)
            {
                if( i != j)
                {

                neighbourIDs.push_back(i);
                }
            }




         }


         cell newCell(j,fourCorners,neighbourIDs);
         graph.push_back(newCell);

     }

    for ( int j =0; j<graph.size();j++)
    {
        graph[j].printCellInfo();
    }

//    graph[0].getNeighbourIDs

}



int main()
{

    cellGraph benGraph("plane_Test.obj");
    benGraph.printCellGraph();


}
