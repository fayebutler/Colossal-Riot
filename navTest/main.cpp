#include "mainwindow.h"
#include <QApplication>
#include "sparsegraph.h"
#include "cell.h"

std::vector <cell> * gGraph;


void loadOBJ(const char *fileName, std::vector<ngl::Vec3> &vertices, std::vector<GLushort> &elements )
{

    std::ifstream in(fileName, std::ios::in);
     if (!in) { std::cerr << "Cannot open " << fileName << std::endl; exit(1); }

     std::string line;
     while (getline(in, line)) {
       if (line.substr(0,2) == "v ") {
         std::istringstream s(line.substr(2));
         ngl::Vec3 v; s >> v.x; s >> v.y; s >> v.z;
         vertices.push_back(v);
       }  else if (line.substr(0,2) == "f ") {
         std::istringstream s(line.substr(2));
         GLushort a,b,c;
         s >> a; s >> b; s >> c;
         a--; b--; c--;
         elements.push_back(a); elements.push_back(b); elements.push_back(c);
       }
       else if (line[0] == '#') { /* ignoring this line */ }
       else { /* ignoring this line */ }
     }

     for (int i = 0; i < elements.size(); i+=3) {
       GLushort ia = elements[i];
       GLushort ib = elements[i+1];
       GLushort ic = elements[i+2];

     }

     //std::cout<< " Vert Indexs:  "<<

}

void generateGraph(std::vector<glm::vec4> &verticies)
{

    for ( int i=0; i<verticies.size(); i++)
    {


    }
//    for (int i=0;i<rows; i++)
//    {
//        for (int j=0; j<columns; j++)
//        {
//           int nodeRows = i*10;
//           graphNode newNode(nodeRows+j);
//        }

//    }

}





int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    loadOBJ("plane_plain.obj");



    return a.exec();
}
