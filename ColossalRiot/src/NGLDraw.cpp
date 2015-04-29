#include "NGLDraw.h"
#include <ngl/ShaderLib.h>
#include <ngl/NGLInit.h>
#include <ngl/Material.h>
#include <ngl/Transformation.h>
#include "Vehicle.h"
#include "GameWorld.h"


const static float INCREMENT=-0.02;
const static float ZOOM=5;
NGLDraw::NGLDraw()
{
  m_rotate=false;
  // mouse rotation values set to 0
  m_spinXFace=0;
  m_spinYFace=0;

  glClearColor(0.4f, 0.4f, 0.4f, 1.0f);			   // Grey Background
  // enable depth testing for drawing
  glEnable(GL_DEPTH_TEST);


   // now to load the shader and set the values
  // grab an instance of shader manager
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  // we are creating a shader called Phong
  shader->createShaderProgram("Phong");
  // now we are going to create empty shaders for Frag and Vert
  shader->attachShader("PhongVertex",ngl::VERTEX);
  shader->attachShader("PhongFragment",ngl::FRAGMENT);
  // attach the source
  shader->loadShaderSource("PhongVertex","shaders/PhongVertex.glsl");
  shader->loadShaderSource("PhongFragment","shaders/PhongFragment.glsl");
  // compile the shaders
  shader->compileShader("PhongVertex");
  shader->compileShader("PhongFragment");
  // add them to the program
  shader->attachShaderToProgram("Phong","PhongVertex");
  shader->attachShaderToProgram("Phong","PhongFragment");
  // now bind the shader attributes for most NGL primitives we use the following
  // layout attribute 0 is the vertex data (x,y,z)
  shader->bindAttribute("Phong",0,"inVert");
  // attribute 1 is the UV data u,v (if present)
  shader->bindAttribute("Phong",1,"inUV");
  // attribute 2 are the normals x,y,z
  shader->bindAttribute("Phong",2,"inNormal");

  // now we have associated this data we can link the shader
  shader->linkProgramObject("Phong");
  // and make it active ready to load values
  (*shader)["Phong"]->use();
  // the shader will use the currently active material and light0 so set them
  ngl::Material m(ngl::POLISHEDSILVER);
  // load our material values to the shader into the structure material (see Vertex shader)
  m.loadToShader("material");





  // Now we will create a basic Camera from the graphics library
  // This is a static camera so it only needs to be set once
  // First create Values for the camera position
  ngl::Vec3 from(0,50,0);
  ngl::Vec3 to(0,0,0);
  ngl::Vec3 up(0,0,-1);

  // now load to our new camera
  m_cam= new ngl::Camera(from,to,up);
  // set the shape using FOV 45 Aspect Ratio based on Width and Height
  // The final two are near and far clipping planes of 0.5 and 10

  m_cam->setShape(45,(float)m_width/m_height,0.05,350);
  shader->setShaderParam3f("viewerPos",m_cam->getEye().m_x,m_cam->getEye().m_y,m_cam->getEye().m_z);
  // now create our light this is done after the camera so we can pass the
  // transpose of the projection matrix to the light to do correct eye space
  // transformations
  ngl::Mat4 iv=m_cam->getViewMatrix();
  iv.transpose();
  m_light = new ngl::Light(ngl::Vec3(-2,5,2),ngl::Colour(1,1,1,1),ngl::Colour(1,1,1,1),ngl::POINTLIGHT );
  m_light->setTransform(iv);
  // load these values to the shader as well
  m_light->loadToShader("light");

  m_gameworld = new GameWorld();

  m_selected = false;
  m_selectedSquad = NULL;

}

NGLDraw::~NGLDraw()
{
  ngl::NGLInit *Init = ngl::NGLInit::instance();
  std::cout<<"Shutting down NGL, removing VAO's and Shaders\n";
  delete m_light;
  delete m_cam;
  Init->NGLQuit();
}

void NGLDraw::resize(int _w, int _h)
{
  glViewport(0,0,_w,_h);

  m_height = _h;
  m_width = _w;

  // now set the camera size values as the screen size has changed
  m_cam->setShape(45,(float)_w/_h,0.05,350);
}

void NGLDraw::draw()
{
  // clear the screen and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // grab an instance of the shader manager
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  (*shader)["Phong"]->use();


  // Rotation based on the mouse position for our global transform
  ngl::Mat4 rotX;
  ngl::Mat4 rotY;
  // create the rotation matrices
  rotX.rotateX(m_spinXFace);
  rotY.rotateY(m_spinYFace);
  // multiply the rotations
  m_mouseGlobalTX=rotY*rotX;
  // add the translations
  m_mouseGlobalTX.m_m[3][0] = m_modelPos.m_x;
  m_mouseGlobalTX.m_m[3][1] = m_modelPos.m_y;
  m_mouseGlobalTX.m_m[3][2] = m_modelPos.m_z;

  m_gameworld->draw(m_cam, m_mouseGlobalTX);


}

void NGLDraw::update(double timeElapsed, double currentTime)
{
  m_gameworld->Update(timeElapsed, currentTime);
}


void NGLDraw::loadMatricesToShader()
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();

  ngl::Mat4 MV;
  ngl::Mat4 MVP;
  ngl::Mat3 normalMatrix;
  ngl::Mat4 M;
  M=m_trans.getMatrix()*m_mouseGlobalTX;
  MV=  M*m_cam->getViewMatrix();
  MVP= M*m_cam->getVPMatrix();
  normalMatrix=MV;
  normalMatrix.inverse();
  shader->setShaderParamFromMat4("MV",MV);
  shader->setShaderParamFromMat4("MVP",MVP);
  shader->setShaderParamFromMat3("normalMatrix",normalMatrix);
  shader->setShaderParamFromMat4("M",M);
}

//----------------------------------------------------------------------------------------------------------------------
void NGLDraw::mouseMoveEvent (const SDL_MouseMotionEvent &_event)
{

  // right mouse translate code
if(m_translate && _event.state &SDL_BUTTON_MMASK)
  {
    int diffX = (int)(_event.x - m_origXPos);
    int diffZ = (int)(_event.y - m_origYPos);
    m_origXPos=_event.x;
    m_origYPos=_event.y;
//    m_modelPos.m_x += INCREMENT * diffX;
//    m_modelPos.m_y -= INCREMENT * diffY;

    m_cam->move(INCREMENT * diffX,0,INCREMENT * diffZ);

    this->draw();
  }
}


//----------------------------------------------------------------------------------------------------------------------
void NGLDraw::mousePressEvent (const SDL_MouseButtonEvent &_event)
{
  // this method is called when the mouse button is pressed in this case we
  // store the value where the maouse was clicked (x,y) and set the Rotate flag to true

 if(_event.button == SDL_BUTTON_MIDDLE)
  {
    m_origXPos = _event.x;
    m_origYPos = _event.y;
    m_translate=true;
  }
 if(_event.button == SDL_BUTTON_LEFT)
  {
     std::cout<<"MOUSEBUTTON PRESS  "<<_event.x<<"  "<<_event.y<<std::endl;
     if(m_selected == true)
     {
         doMovement(_event.x, _event.y);
     }
     else
     {
        doSelection(_event.x, _event.y);
     }

  }
}

//----------------------------------------------------------------------------------------------------------------------
void NGLDraw::mouseReleaseEvent (const SDL_MouseButtonEvent &_event)
{
  // this event is called when the mouse button is released
  // we then set Rotate to false
  if (_event.button == SDL_BUTTON_LEFT)
  {
    m_rotate=false;
  }
  // right mouse translate mode
  if (_event.button == SDL_BUTTON_MIDDLE)
  {
    m_translate=false;
  }
}

//----------------------------------------------------------------------------------------------------------------------
void NGLDraw::wheelEvent(const SDL_MouseWheelEvent &_event)
{

  // check the diff of the wheel position (0 means no change)
  if(_event.y > 0)
  {
    m_modelPos.m_y+=ZOOM;
    this->draw();
  }
  else if(_event.y <0 )
  {
    m_modelPos.m_y-=ZOOM;
    this->draw();
  }

  // check the diff of the wheel position (0 means no change)
  if(_event.x > 0)
  {
    m_modelPos.m_x-=ZOOM;
    this->draw();
  }
  else if(_event.x <0 )
  {
    m_modelPos.m_x+=ZOOM;
    this->draw();
  }
}
//----------------------------------------------------------------------------------------------------------------------

void NGLDraw::doSelection(const int _x, const int _y)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



//    glReadPixels(_x, _y , 1, 1, GL_RGB, GL_FLOAT, &pixel);
//    std::cout<<"PIXEL COLOUR  "<< pixel[0]<<"  "<<pixel[1]<<"  "<<pixel[2]<<std::endl;

    for(int i=0; i < m_gameworld->getSquads().size(); i++)
    {
        Squad* currentSquad = m_gameworld->getSquads()[i];

        currentSquad->selectionDraw(m_cam, m_mouseGlobalTX);

    }
//    unsigned char pixel[3];
    ngl::Vec3 pixel;
    //pixel = ngl::Vec3(0.f,0.f,0.f);
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    glReadPixels(_x, viewport[3] - _y , 1, 1, GL_RGB, GL_FLOAT, &pixel);
//    pixel.m_x = round(pixel.m_x);
    std::cout<<"PIXEL COLOUR  "<< pixel[0]<<"  "<<pixel[1]<<"  "<<pixel[2]<<std::endl;

    if (m_selectedSquad!= NULL)
    {
        m_selectedSquad->setSquadColour(ngl::Colour(0.0f,1.0f,0.0f,1.0f));
    }

    for(int i=0; i < m_gameworld->getSquads().size(); i++)
    {
        Squad* currentSquad = m_gameworld->getSquads()[i];


        if(currentSquad->checkSelectionColour(pixel) == true)
        {
            currentSquad->setSquadColour(ngl::Colour(0.0f,0.5f,0.5f,1.0f));
            m_selected = true;
            m_selectedSquad = currentSquad;
            break;
        }
    }
}

void NGLDraw::doMovement(const int _x, const int _y)
{
    m_clickPosition = getWorldSpace(_x, _y);

    std::cout<<" MOVE TO :  "<<m_clickPosition.m_x<<"  "<<m_clickPosition.m_y<<"  "<<m_clickPosition.m_z<<std::endl;
    std::cout<<"SQUAD POSITION BEFORE MOVE : "<<m_selectedSquad->getPos().m_x<<"  "<<m_selectedSquad->getPos().m_y<<"  "<<m_selectedSquad->getPos().m_z<<std::endl;
    m_selectedSquad->setPos(m_clickPosition);
    std::cout<<"SQUAD POSITION AFTER MOVE : "<<m_selectedSquad->getPos().m_x<<"  "<<m_selectedSquad->getPos().m_y<<"  "<<m_selectedSquad->getPos().m_z<<std::endl;
    m_selected = false;
}

ngl::Vec3 NGLDraw::getWorldSpace(int _x, int _y)
{
  std::cout<<"Mouse pos "<<_x<<" "<<_y<<" ";

  ngl::Mat4 t=m_cam->getProjectionMatrix();
  ngl::Mat4 v=m_cam->getViewMatrix()*m_mouseGlobalTX;

  // as ngl:: and OpenGL use different formats need to transpose the matrix.
  t.transpose();
  v.transpose();
  ngl::Mat4 inverse=(t*v).inverse();

  std::cout<<"WIDTH "<<m_width<<"HEIGHT "<<m_height<<std::endl;

  ngl::Vec4 tmp(0.0f,0.0f,1.0f,1.0f);
  // convert into NDC
  tmp.m_x=(2.0f * _x) / m_width - 1.0f;
  tmp.m_y=1.0f - (2.0f * _y) / m_height;
  // scale by inverse MV * Project transform
  ngl::Vec4 obj=inverse*tmp;
  // Scale by w
  obj/=obj.m_w*75.0;

  std::cout<<obj.m_w*75.0<<std::endl;
    std::cout<<obj.m_w<<std::endl;


  obj.m_y = 0.0;



  return obj.toVec3();


}





