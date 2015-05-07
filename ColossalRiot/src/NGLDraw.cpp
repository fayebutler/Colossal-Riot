#include "NGLDraw.h"
#include <ngl/ShaderLib.h>
#include <ngl/NGLInit.h>
#include <ngl/Material.h>
#include <ngl/Transformation.h>
#include "GameWorld.h"

const static float INCREMENT=-0.02;
const static float ZOOM=5;
float cameraHeight =125 ;

NGLDraw::NGLDraw(int _width, int _height)
{
  m_rotate=false;
  // mouse rotation values set to 0
  m_spinXFace=0;
  m_spinYFace=0;

  // Grey Background
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
  ngl::Vec3 from(0,cameraHeight,0);
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
  m_light = new ngl::Light(ngl::Vec3(-2,5,2),ngl::Colour(0.8,0.8,0.8,1),ngl::Colour(1,1,1,1),ngl::POINTLIGHT );
  //m_light->setTransform(iv);
  // load these values to the shader as well
  m_light->loadToShader("light");

  m_width = _width;
  m_height = _height;

  m_gameState = gameMenu;

  m_buttonPlay = new UIButton(buttonPlay, "../font/arial.ttf", 40, ngl::Vec2(m_width, m_height));
  m_buttonPlay->updateButton(ngl::Vec2(0.f, 0.f), ngl::Vec2(0.35f, 0.2f), ngl::Vec4(0.2f, 0.2f, 0.9f, 1.f));
  m_buttonPlay->updateText("New Game", ngl::Vec3(1.f, 1.f, 1.f), ngl::Vec2(-98.f, -25.f));
  m_buttons.push_back(m_buttonPlay);

  m_buttonQuit = new UIButton(buttonQuit,  "../font/arial.ttf", 40, ngl::Vec2(m_width, m_height));
  m_buttonQuit->updateButton(ngl::Vec2(0.f, -0.3f), ngl::Vec2(0.35f, 0.2f), ngl::Vec4(0.2f, 0.2f, 0.9f, 1.f));
  m_buttonQuit->updateText("Quit", ngl::Vec3(1.f, 1.f, 1.f), ngl::Vec2(-40.f, -25.f));
  m_buttons.push_back(m_buttonQuit);

  m_buttonPause = new UIButton(buttonPause, "../font/arial.ttf", 40, ngl::Vec2(m_width, m_height));
  m_buttonPause->updateButton(ngl::Vec2(0.9f, 0.95f), ngl::Vec2(0.2f, 0.1f), ngl::Vec4(0.2f, 0.2f, 0.9f, 1.f));
  m_buttonPause->updateText("Pause", ngl::Vec3(1.f, 1.f, 1.f), ngl::Vec2(-52.f, -25.f));
  m_buttons.push_back(m_buttonPause);

  m_buttonMenu = new UIButton(buttonMenu, "../font/arial.ttf", 40, ngl::Vec2(m_width, m_height));
  m_buttonMenu->updateButton(ngl::Vec2(0.9f, 0.83f), ngl::Vec2(0.2f, 0.1f), ngl::Vec4(0.2f, 0.2f, 0.9f, 1.f));
  m_buttonMenu->updateText("Menu", ngl::Vec3(1.f, 1.f, 1.f), ngl::Vec2(-52.f, -25.f));
  m_buttons.push_back(m_buttonMenu);

  m_buttonCreateSquad = new UIButton(buttonCreateSquad, "../font/arial.ttf", 20, ngl::Vec2(m_width, m_height));
  m_buttonCreateSquad->updateButton(ngl::Vec2(0.f, -0.9f), ngl::Vec2(0.3f, 0.1f), ngl::Vec4(0.2f, 0.2f, 0.9f, 1.f));
  m_buttonCreateSquad->updateText("Create Squad", ngl::Vec3(1.f, 1.f, 1.f), ngl::Vec2(-60.f, -13.f));
  m_buttons.push_back(m_buttonCreateSquad);

  m_sliderSquadSize = new UISlider(sliderSquadSize, "../font/arial.ttf", 40, ngl::Vec2(m_width, m_height));
  m_sliderSquadSize->updateSlider(ngl::Vec2(-0.5f, -0.9f), ngl::Vec2(0.5f, 0.1f), ngl::Vec4(0.2f, 0.2f, 0.9f, 1.f), ngl::Vec2(-0.5f, -0.9f), ngl::Vec2(0.03f, 0.1f), ngl::Vec4(1.f, 1.f, 1.f, 1.f), 3, 9);
  m_squadSize = m_sliderSquadSize->calculateOutput();
  std::cout<<"m_squadSize"<<m_squadSize<<std::endl;
  m_ss.str(std::string());
  m_ss << m_squadSize;
  m_squadSizeString = m_ss.str();
  m_sliderSquadSize->updateText(m_squadSizeString, ngl::Vec3(1.f, 1.f, 1.f), ngl::Vec2(179.f, -27.f));

//  m_gameworld = new GameWorld();

//  m_selected = false;
//  m_selectedSquad = NULL;


  m_entityMgr = new EntityManager();

}

NGLDraw::~NGLDraw()
{
  delete m_entityMgr;
  ngl::NGLInit *Init = ngl::NGLInit::instance();
  std::cout<<"Shutting down NGL, removing VAO's and Shaders\n";
  delete m_light;
  delete m_cam;
  delete m_buttonPlay;
  delete m_buttonQuit;
  delete m_buttonPause;
  delete m_buttonCreateSquad;
  delete m_sliderSquadSize;
  Init->NGLQuit();
}

void NGLDraw::resize(int _w, int _h)
{
  glViewport(0,0,_w,_h);

  m_height = _h;
  m_width = _w;

  m_longestSide = _h;
  if (_w > _h)
  {
      m_longestSide = _w;
  }

  // now set the camera size values as the screen size has changed
  m_cam->setShape(45,(float)_w/_h,0.05,350);


  unsigned int numberOfButtons = m_buttons.size();
  for (unsigned int i = 0; i < numberOfButtons; i++)
  {
    m_buttons[i]->setScreenDimensions(ngl::Vec2(m_width, m_height));
  }
  m_sliderSquadSize->setScreenDimensions(ngl::Vec2(m_width, m_height));


  m_text->setScreenSize(_w, _h);
  // Ok so this is still a bit of a hack we need to scale our text based
  // on max screen size so first get the size of the screen
  SDL_Rect s;
  SDL_GetDisplayBounds(0,&s);
  float x,y;
  // now get a scale transform for the text shader


  x=1.0-float(s.w-_w)/s.w;
  y=1.0-float(s.h-_h)/s.h;
  // now set the new transform element for this shader
  m_text->setTransform(x,y);
}

void NGLDraw::startGame(int level)
{
  switch(level)
  {
    case 0:
        m_gameworld = new GameWorld(0, 100);
        break;
    case 1:
        m_gameworld = new GameWorld(100, 50);
        break;
    case 2:
        m_gameworld = new GameWorld(200, 40);
        break;
    case 3:
        m_gameworld = new GameWorld(300, 30);
        break;
    case 4:
        m_gameworld = new GameWorld(400, 25);
        break;
  default:
      std::cout<<"undefined level"<<std::endl;
      break;
  }

    m_selected = false;
    m_selectedSquad = NULL;
    m_selectedSquadID = -1;

}

void NGLDraw::endGame()
{
    delete m_gameworld;

    // reset camera
    ngl::Vec3 from(0,cameraHeight,0);
    ngl::Vec3 to(0,0,0);
    ngl::Vec3 up(0,0,-1);

    m_cam = new ngl::Camera(from,to,up);
    m_cam->setShape(45,(float)m_width/m_height,0.05,50);

    m_mouseGlobalTX = ngl::Mat4();
    m_modelPos = ngl::Vec3(0,0,0);

}

void NGLDraw::draw()
{
  // clear the screen and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);



  switch (m_gameState)
  {
    case gamePlay:
    {
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

      m_buttonPause->draw();
      m_buttonMenu->draw();
      m_buttonCreateSquad->draw();

      m_sliderSquadSize->draw();

      break;
    }
    case gameMenu:
    {
      glClearColor(0.0f, 0.3f, 0.3f, 1.0f);
      m_buttonPlay->draw();
      m_buttonQuit->draw();
      break;
    }
    case gamePause:
    {
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

      m_buttonPause->draw();
      m_buttonMenu->draw();

      break;
    }
    case gameQuit:
    {
      break;
    }
    default:
    {
      std::cout<<"Error: Invalid gameState"<<std::endl;
      break;
    }
  }

}

void NGLDraw::update(double timeElapsed, double currentTime)
{
  m_gameworld->Update(timeElapsed, currentTime);

  m_ss.str(std::string());
  m_ss << m_squadSize;
  m_squadSizeString = m_ss.str();

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
    std::cout<<"EYE: "<<m_cam->getEye().m_x<<" "<< m_cam->getEye().m_z<<std::endl;

    int diffX = (int)(_event.x - m_origXPos);
    int diffZ = (int)(_event.y - m_origYPos);
    m_origXPos=_event.x;
    m_origYPos=_event.y;

    m_cam->move(INCREMENT * (abs(m_modelPos.m_y-cameraHeight+1)*0.05) * diffX,0,INCREMENT * (abs(m_modelPos.m_y-cameraHeight+1)*0.05) * diffZ);




  }
  if (m_sliderSquadSize->getIsSliding() == true)
  {
      float cameraLimitX = (50.0);
      //cameraLimitX += (m_modelPos.m_y/2.0);

      float cameraLimitZ = (50.0);
      //cameraLimitZ += (m_modelPos.m_y/2.0);

  //    float cameraLimitX = 50;
  //    float cameraLimitZ = 50;
    m_squadSize = m_sliderSquadSize->slideBar(_event.x);
    m_sliderSquadSize->setTextString(m_squadSizeString);

    std::cout<<"MODELPOS "<<m_modelPos.m_x<<" "<<m_modelPos.m_y<<" "<<m_modelPos.m_z<<std::endl;

    if(m_cam->getEye().m_x> cameraLimitX)
    {
        m_cam->setEye(ngl::Vec3(cameraLimitX,m_cam->getEye().m_y,m_cam->getEye().m_z));
    }
    if(m_cam->getEye().m_x< -cameraLimitX)
    {
        m_cam->setEye(ngl::Vec3(-cameraLimitX,m_cam->getEye().m_y,m_cam->getEye().m_z));
    }
    if(m_cam->getEye().m_z> cameraLimitZ)
    {
        m_cam->setEye(ngl::Vec3(m_cam->getEye().m_x,m_cam->getEye().m_y,cameraLimitZ));
    }
    if(m_cam->getEye().m_z< -cameraLimitZ)
    {
        m_cam->setEye(ngl::Vec3(m_cam->getEye().m_x,m_cam->getEye().m_y,-cameraLimitZ));
    }


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
      if (m_gameState == gamePlay)
      {
         if(m_selected == true)
         {
            doMovement(_event.x, _event.y);
         }
         else
         {
            doSelection(_event.x, _event.y);
         }
      }

       unsigned int numberOfButtons = m_buttons.size();
       for (unsigned int i = 0; i < numberOfButtons; i++)
       {
         if (m_buttons[i]->isClicked(_event.x, _event.y) && m_buttons[i]->getIsActive() == true)
         {
           switch (m_buttons[i]->getName())
           {
             case buttonPlay:
             {
               startGame(1);
               m_gameState = gamePause;
               m_buttonQuit->setIsActive(false);
               m_buttonPlay->setIsActive(false);
               m_buttonPause->setIsActive(true);
               m_buttonMenu->setIsActive(true);

               break;
             }
             case buttonQuit :
             {
               m_gameState = gameQuit;
               break;
             }
             case buttonPause :
             {
               if (m_gameState == gamePlay)
               {
                 m_gameState = gamePause;
                 m_gameTimer.resetTimer();
                 m_buttonPause->updateButton(ngl::Vec2(0.9f, 0.95f), ngl::Vec2(0.2f, 0.1f), ngl::Vec4(0.2f, 0.2f, 0.9f, 1.f));
                 m_buttonPause->updateText("Play", ngl::Vec3(1.f, 1.f, 1.f), ngl::Vec2(-33.f, -25.f));
                 break;
               }
               else if (m_gameState == gamePause)
               {
                 m_gameState = gamePlay;
                 m_gameTimer.resetTimer();
                 m_buttonPause->updateButton(ngl::Vec2(0.9f, 0.95f), ngl::Vec2(0.2f, 0.1f), ngl::Vec4(0.2f, 0.2f, 0.9f, 1.f));
                 m_buttonPause->updateText("Pause", ngl::Vec3(1.f, 1.f, 1.f), ngl::Vec2(-52.f, -25.f));
                 break;
               }
               break;
             }
             case buttonMenu :
             {
               m_gameState = gameMenu;
               //endGame();
               m_buttonQuit->setIsActive(true);
               m_buttonPlay->setIsActive(true);
               m_buttonMenu->setIsActive(false);
               m_buttonMenu->setIsActive(false);
               break;

             }
             case buttonCreateSquad :
             {
               createSquad(m_squadSize);
               break;
             }
             default:
             {
               break;
             }

           }
         }
       }
       if (m_sliderSquadSize->isClicked(_event.x, _event.y))
       {
         std::cout<<"SLIDER"<<std::endl;
         m_sliderSquadSize->setIsSliding(true);
         m_squadSize = m_sliderSquadSize->slideBar(_event.x);
         m_sliderSquadSize->setTextString(m_squadSizeString);
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
    m_sliderSquadSize->setIsSliding(false);
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
      m_modelPos.m_y+=(ZOOM*(cameraHeight-m_modelPos.m_y)*0.05);

      float nearest = 3.0;

      if(cameraHeight-m_modelPos.m_y < nearest)
      {
        m_modelPos.m_y = cameraHeight - nearest;
      }
  }
  else if(_event.y < 0)
  {
      m_modelPos.m_y-=(ZOOM*(cameraHeight-m_modelPos.m_y)*0.05);

      float furthest = ((49.0*5000.0)/(m_longestSide));

      if(cameraHeight-m_modelPos.m_y > furthest)
      {
        m_modelPos.m_y= cameraHeight - furthest;
      }
  }

}
//----------------------------------------------------------------------------------------------------------------------

void NGLDraw::doSelection(const int _x, const int _y)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for(int i=0; i < m_gameworld->getSquads().size(); i++)
    {
        Squad* currentSquad = m_gameworld->getSquads()[i];

        currentSquad->selectionDraw(m_cam, m_mouseGlobalTX);

    }

    ngl::Vec3 pixel;

    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    glReadPixels(_x, viewport[3] - _y , 1, 1, GL_RGB, GL_FLOAT, &pixel);


    for(int i=0; i < m_gameworld->getSquads().size(); i++)
    {
        Squad* currentSquad = m_gameworld->getSquads()[i];

        if(currentSquad->checkSelectionColour(pixel) == true)
        {
            currentSquad->setSquadColour(ngl::Colour(0.0f,0.5f,0.5f,1.0f));
            m_selected = true;
            m_selectedSquad = currentSquad;
            m_selectedSquadID = currentSquad->getID();
            break;
        }
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

void NGLDraw::doMovement(const int _x, const int _y)
{
    std::map<int, BaseGameEntity*>::const_iterator entity = m_entityMgr->getEntityMap().find(m_selectedSquadID);

    if(entity->first !=  m_entityMgr->getEntityMap().end()->first)
    {
        m_clickPosition = getWorldSpace(_x, _y);

        m_gameworld->createPath(m_selectedSquad, m_clickPosition);

        m_selectedSquad->setSquadColour(ngl::Colour(1.0f,1.0f,0.0f,1.0f));
    }


    m_selected = false;
}

ngl::Vec3 NGLDraw::getWorldSpace(int _x, int _y)
{

    ngl::Mat4 m;
    m = m*m_mouseGlobalTX;
    ngl::Mat4 t=m_cam->getProjectionMatrix();
    ngl::Mat4 v=m_cam->getViewMatrix();

    // as ngl:: and OpenGL use different formats need to transpose the matrix.
    t.transpose();
    v.transpose();
    m.transpose();
    ngl::Mat4 inverse=(t*v*m).inverse();

    ngl::Vec4 tmp(0,0,-1.0f,1.0f);
    // convert into NDC
    tmp.m_x=(2.0f * _x) / m_width- 1.0f;
    tmp.m_y=1.0f - (2.0f * _y) / m_height;
    // scale by inverse MV * Project transform

    ngl::Vec4 near(tmp.m_x,tmp.m_y,-1.0f,1.0f);
    ngl::Vec4 far(tmp.m_x,tmp.m_y,1.0f,1.0f);

    //get world point on near and far clipping planes
    ngl::Vec4 obj_near=inverse*near;
    ngl::Vec4 obj_far=inverse*far;

    // Scale by w
    obj_near/=obj_near.m_w;
    obj_far/=obj_far.m_w;

    ngl::Vec3 nearPoint(obj_near.m_x,obj_near.m_y,obj_near.m_z);
    ngl::Vec3 farPoint(obj_far.m_x,obj_far.m_y,obj_far.m_z);

    //create ray
    ngl::Vec3 rayDir(farPoint-nearPoint);
    if(rayDir.lengthSquared() == 0.0f)
    {
        std::cout<<"Ray Direction in getWorldSpace equals zero, can't normalise"<<std::endl;
    }
    else
    {
        rayDir.normalize();
    }

    //calculate distance to zx plane
    float dist = (-nearPoint.m_y)/rayDir.m_y;

    //set world space coordinate where y = 0
    ngl::Vec3 obj(nearPoint.m_x + (dist*rayDir.m_x),nearPoint.m_y + (dist*rayDir.m_y),nearPoint.m_z + (dist*rayDir.m_z));



    obj.m_y = 0.0;

    return obj;
}

void NGLDraw::createSquad(int size)
{
    m_gameworld->createSquad(size);
}





