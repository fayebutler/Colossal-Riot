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
  // layout attribute 0 is the vertex data (x,y,z)1
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

  m_entityMgr = new EntityManager();

  initialiseUI();
  m_selectedLevel = 1;

}

NGLDraw::~NGLDraw()
{
  delete m_entityMgr;
  ngl::NGLInit *Init = ngl::NGLInit::instance();
  std::cout<<"Shutting down NGL, removing VAO's and Shaders\n";
  delete m_light;
  delete m_cam;
  delete m_buttonPlay;
  delete m_buttonLevel1;
  delete m_buttonLevel2;
  delete m_buttonLevel3;
  delete m_buttonLevel4;
  delete m_buttonLevel5;
  delete m_buttonQuit;
  delete m_buttonPause;
  delete m_buttonCreateSquad;
  delete m_buttonSquadWall;
  delete m_sliderSquadSize;
  delete m_textSmall;
  delete m_textMedium;
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

  m_textSmall->setScreenSize(_w, _h);
  m_textMedium->setScreenSize(_w, _h);
  // Ok so this is still a bit of a hack we need to scale our text based
  // on max screen size so first get the size of the screen
  SDL_Rect s;
  SDL_GetDisplayBounds(0,&s);
  float x,y;
  // now get a scale transform for the text shader


  x=1.0-float(s.w-_w)/s.w;
  y=1.0-float(s.h-_h)/s.h;
  // now set the new transform element for this shader
  m_textSmall->setTransform(x, y);
  m_textMedium->setTransform(x, y);
}

void NGLDraw::startGame(int level)
{
    m_gameworld = new GameWorld(level);
    m_selected = false;
    m_selectedSquad = NULL;
    m_selectedSquadID = -1;

}

void NGLDraw::endGame()
{
    m_gameTimer.resetTimer();
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
      if (m_selectedSquad)
      {
        switch (m_selectedSquad->getSquadState())
        {
          case squadPatrol :
          {
            m_buttonSquadPatrol->setButtonColour(ngl::Vec4(0.2f, 0.2f, 0.4f));
            m_buttonSquadAggressive->setButtonColour(ngl::Vec4(0.2f, 0.2f, 0.9f));
            m_buttonSquadDefensive->setButtonColour(ngl::Vec4(0.2f, 0.2f, 0.9f));
            m_buttonSquadWall->setButtonColour(ngl::Vec4(0.2f, 0.2f, 0.9f));
            break;
          }
          case squadAggressive :
          {
            m_buttonSquadPatrol->setButtonColour(ngl::Vec4(0.2f, 0.2f, 0.9f));
            m_buttonSquadAggressive->setButtonColour(ngl::Vec4(0.2f, 0.2f, 0.4f));
            m_buttonSquadDefensive->setButtonColour(ngl::Vec4(0.2f, 0.2f, 0.9f));
            m_buttonSquadWall->setButtonColour(ngl::Vec4(0.2f, 0.2f, 0.9f));
            break;
          }
          case squadDefensive :
          {
            m_buttonSquadPatrol->setButtonColour(ngl::Vec4(0.2f, 0.2f, 0.9f));
            m_buttonSquadAggressive->setButtonColour(ngl::Vec4(0.2f, 0.2f, 0.9f));
            m_buttonSquadDefensive->setButtonColour(ngl::Vec4(0.2f, 0.2f, 0.4f));
            m_buttonSquadWall->setButtonColour(ngl::Vec4(0.2f, 0.2f, 0.9f));
            break;
          }
          case squadWall :
          {
            m_buttonSquadPatrol->setButtonColour(ngl::Vec4(0.2f, 0.2f, 0.9f));
            m_buttonSquadAggressive->setButtonColour(ngl::Vec4(0.2f, 0.2f, 0.9f));
            m_buttonSquadDefensive->setButtonColour(ngl::Vec4(0.2f, 0.2f, 0.9f));
            m_buttonSquadWall->setButtonColour(ngl::Vec4(0.2f, 0.2f, 0.4f));
            break;
          }
          default :
          {
            std::cout<<"Error: Invalid squad state"<<std::endl;
            break;
          }
        }
        m_buttonSquadPatrol->setIsActive(true);
        m_buttonSquadPatrol->draw();
        m_buttonSquadAggressive->setIsActive(true);
        m_buttonSquadAggressive->draw();
        m_buttonSquadDefensive->setIsActive(true);
        m_buttonSquadDefensive->draw();
        m_buttonSquadWall->setIsActive(true);
        m_buttonSquadWall->draw();
      }
      else
      {
        m_buttonSquadPatrol->setIsActive(false);
        m_buttonSquadAggressive->setIsActive(false);
        m_buttonSquadDefensive->setIsActive(false);
        m_buttonSquadWall->setIsActive(false);
      }



      m_sliderSquadSize->draw();

      m_ss.str(std::string());
      m_ss << m_gameworld->getAvailablePolice();
      m_textMedium->renderText(732.f, 1118.f, m_ss.str());

      break;
    }
    case gameMenu:
    {
      glClearColor(0.0f, 0.3f, 0.3f, 1.0f);
      m_buttonPlay->draw();
      switch (m_selectedLevel)
      {
        case 1 :
        {
          m_buttonLevel1->setButtonColour(ngl::Vec4(0.2f, 0.2f, 0.4f));
          m_buttonLevel2->setButtonColour(ngl::Vec4(0.2f, 0.2f, 0.9f));
          m_buttonLevel3->setButtonColour(ngl::Vec4(0.2f, 0.2f, 0.9f));
          m_buttonLevel4->setButtonColour(ngl::Vec4(0.2f, 0.2f, 0.9f));
          m_buttonLevel5->setButtonColour(ngl::Vec4(0.2f, 0.2f, 0.9f));
          break;
        }
        case 2 :
        {
          m_buttonLevel1->setButtonColour(ngl::Vec4(0.2f, 0.2f, 0.9f));
          m_buttonLevel2->setButtonColour(ngl::Vec4(0.2f, 0.2f, 0.4f));
          m_buttonLevel3->setButtonColour(ngl::Vec4(0.2f, 0.2f, 0.9f));
          m_buttonLevel4->setButtonColour(ngl::Vec4(0.2f, 0.2f, 0.9f));
          m_buttonLevel5->setButtonColour(ngl::Vec4(0.2f, 0.2f, 0.9f));
          break;
        }
        case 3 :
        {
          m_buttonLevel1->setButtonColour(ngl::Vec4(0.2f, 0.2f, 0.9f));
          m_buttonLevel2->setButtonColour(ngl::Vec4(0.2f, 0.2f, 0.9f));
          m_buttonLevel3->setButtonColour(ngl::Vec4(0.2f, 0.2f, 0.4f));
          m_buttonLevel4->setButtonColour(ngl::Vec4(0.2f, 0.2f, 0.9f));
          m_buttonLevel5->setButtonColour(ngl::Vec4(0.2f, 0.2f, 0.9f));
          break;
        }
        case 4 :
        {
          m_buttonLevel1->setButtonColour(ngl::Vec4(0.2f, 0.2f, 0.9f));
          m_buttonLevel2->setButtonColour(ngl::Vec4(0.2f, 0.2f, 0.9f));
          m_buttonLevel3->setButtonColour(ngl::Vec4(0.2f, 0.2f, 0.9f));
          m_buttonLevel4->setButtonColour(ngl::Vec4(0.2f, 0.2f, 0.4f));
          m_buttonLevel5->setButtonColour(ngl::Vec4(0.2f, 0.2f, 0.9f));
          break;
        }
        case 5 :
        {
          m_buttonLevel1->setButtonColour(ngl::Vec4(0.2f, 0.2f, 0.9f));
          m_buttonLevel2->setButtonColour(ngl::Vec4(0.2f, 0.2f, 0.9f));
          m_buttonLevel3->setButtonColour(ngl::Vec4(0.2f, 0.2f, 0.9f));
          m_buttonLevel4->setButtonColour(ngl::Vec4(0.2f, 0.2f, 0.9f));
          m_buttonLevel5->setButtonColour(ngl::Vec4(0.2f, 0.2f, 0.4f));
          break;
        }
      }


      m_buttonLevel1->draw();
      m_buttonLevel2->draw();
      m_buttonLevel3->draw();
      m_buttonLevel4->draw();
      m_buttonLevel5->draw();
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
    case gameWin:
    {
      glClearColor(0.0f, 0.3f, 0.3f, 1.0f);
      m_buttonMenu->draw();
      m_buttonQuit->draw();
      break;
    }
    case gameLose:
    {
      glClearColor(0.0f, 0.3f, 0.3f, 1.0f);
      m_buttonMenu->draw();
      m_buttonQuit->draw();
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


  if(m_gameworld->hasWon()==1)
  {
      m_gameState = gameWin;
      endGame();
  }
  else if(m_gameworld->hasLost()==1)
  {
      m_gameState = gameLose;
      endGame();
  }
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

    m_cam->move(INCREMENT * (abs(m_modelPos.m_y-cameraHeight+1)*0.05) * diffX,0,INCREMENT * (abs(m_modelPos.m_y-cameraHeight+1)*0.05) * diffZ);
    float cameraLimitX = (50.0);
    float cameraLimitZ = (50.0);
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

  if (m_sliderSquadSize->getIsSliding() == true)
  {
    m_squadSize = m_sliderSquadSize->slideBar(_event.x);
    m_sliderSquadSize->setTextString(m_squadSizeString);  
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
      unsigned int numberOfButtons = m_buttons.size();
      for (unsigned int i = 0; i < numberOfButtons; i++)
      {
        if (m_buttons[i]->isClicked(_event.x, _event.y) && m_buttons[i]->getIsActive() == true)
        {
          std::cout<<"button "<<m_buttons[i]->getName()<<std::endl;
          switch (m_buttons[i]->getName())
          {
            case buttonPlay:
            {
              std::cout<<m_selectedLevel<<std::endl;
              startGame(m_selectedLevel);
              m_gameState = gamePlay;
              m_buttonQuit->setIsActive(false);
              m_buttonPlay->setIsActive(false);
              m_buttonPause->setIsActive(true);
              m_buttonMenu->setIsActive(true);
              m_sliderSquadSize->setIsActive(true);
              m_buttonCreateSquad->setIsActive(true);
              break;
            }
            case buttonLevel1 :
            {
              m_selectedLevel = 1;
              break;
            }
            case buttonLevel2 :
            {
              m_selectedLevel = 2;
              break;
            }
            case buttonLevel3 :
            {
              m_selectedLevel = 3;
              break;
            }
            case buttonLevel4 :
            {
              m_selectedLevel = 4;
              break;
            }
            case buttonLevel5 :
            {
              m_selectedLevel = 5;
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
                m_sliderSquadSize->setIsActive(false);
                m_buttonCreateSquad->setIsActive(false);
                break;
              }
              else if (m_gameState == gamePause)
              {
                m_gameState = gamePlay;
                m_gameTimer.resetTimer();
                m_buttonPause->updateButton(ngl::Vec2(0.9f, 0.95f), ngl::Vec2(0.2f, 0.1f), ngl::Vec4(0.2f, 0.2f, 0.9f, 1.f));
                m_buttonPause->updateText("Pause", ngl::Vec3(1.f, 1.f, 1.f), ngl::Vec2(-52.f, -25.f));
                m_sliderSquadSize->setIsActive(true);
                m_buttonCreateSquad->setIsActive(true);
                break;
              }
              break;
            }
            case buttonMenu :
            {
              m_gameState = gameMenu;
              endGame();
              m_buttonQuit->setIsActive(true);
              m_buttonPlay->setIsActive(true);
              m_buttonMenu->setIsActive(false);
              m_buttonPause->setIsActive(false);
              m_sliderSquadSize->setIsActive(false);
              m_buttonCreateSquad->setIsActive(false);
              break;

            }
            case buttonCreateSquad :
            {
              createSquad(m_squadSize);
              break;
            }
            case buttonSquadPatrol :
            {
              m_selectedSquad->setSquadState("patrol", squadPatrol);
              break;
            }
            case buttonSquadAggressive :
            {
              m_selectedSquad->setSquadState("aggressive", squadAggressive);
              break;
            }
            case buttonSquadDefensive :
            {
              m_selectedSquad->setSquadState("defensive", squadDefensive);
              break;
            }
            case buttonSquadWall :
            {
              m_selectedSquad->setSquadState("wall", squadWall);
              break;
            }
            default:
            {
              break;
            }

          }
          return;
        }
      }
      if (m_sliderSquadSize->isClicked(_event.x, _event.y) && m_sliderSquadSize->getIsActive() == true)
      {
        m_sliderSquadSize->setIsSliding(true);
        m_squadSize = m_sliderSquadSize->slideBar(_event.x);
        m_sliderSquadSize->setTextString(m_squadSizeString);
        return;
      }

      if (m_gameState == gamePlay)
      {

         doSelection(_event.x, _event.y);

      }
    }
    if(_event.button == SDL_BUTTON_RIGHT)
    {
        if (m_gameState == gamePlay)
        {

           if (m_selected)
           {
               m_selectedSquad->setSquadColour(ngl::Colour(0.0f,0.5f,0.5f,1.0f));

               m_selected = false;
               m_selectedSquad = NULL;
               m_selectedSquadID = -1;
           }

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

        //set de-selected squad colour
        currentSquad->setSquadColour(ngl::Colour(0.0f,0.5f,0.5f,1.0f));

    }

    ngl::Vec3 pixel;

    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    glReadPixels(_x, viewport[3] - _y , 1, 1, GL_RGB, GL_FLOAT, &pixel);


    bool newSelection = false;

    for(int i=0; i < m_gameworld->getSquads().size(); i++)
    {
        Squad* currentSquad = m_gameworld->getSquads()[i];

        if(currentSquad->checkSelectionColour(pixel) == true)
        {
            //set selected squad colour
            currentSquad->setSquadColour(ngl::Colour(1.0f,1.0f,0.0f,1.0f));

            m_selected = true;
            m_selectedSquad = currentSquad;
            m_selectedSquadID = currentSquad->getID();
            newSelection = true;
            break;
        }
    }

    if(newSelection == false && m_selected == true)
    {
        doMovement(_x, _y);
    }


    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

void NGLDraw::doMovement(const int _x, const int _y)
{
    std::map<int, BaseGameEntity*>::const_iterator entity = m_entityMgr->getEntityMap().find(m_selectedSquadID);

    if(entity->first !=  m_entityMgr->getEntityMap().end()->first)
    {
        m_clickPosition = getWorldSpace(_x, _y);

        m_gameworld->squadTarget(m_selectedSquad, m_clickPosition);
    }

    m_selectedSquad = NULL;
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

void NGLDraw::initialiseUI()
{
  m_textSmall = new Text("../font/arial.ttf", 20);
  m_textMedium = new Text("../font/arial.ttf", 40);

  m_buttonPlay = new UIButton(buttonPlay, "../font/arial.ttf", 40, ngl::Vec2(m_width, m_height));
  m_buttonPlay->updateButton(ngl::Vec2(0.f, 0.1f), ngl::Vec2(0.5f, 0.2f), ngl::Vec4(0.2f, 0.2f, 0.9f, 1.f));
  m_buttonPlay->updateText("New Game", ngl::Vec3(1.f, 1.f, 1.f), ngl::Vec2(-98.f, -25.f));
  m_buttons.push_back(m_buttonPlay);

  m_buttonLevel1 = new UIButton(buttonLevel1, "../font/arial.ttf", 30, ngl::Vec2(m_width, m_height));
  m_buttonLevel1->updateButton(ngl::Vec2(-0.2f, -0.1f), ngl::Vec2(0.1f, 0.1f), ngl::Vec4(0.2f, 0.2f, 0.4f, 1.f));
  m_buttonLevel1->updateText("1", ngl::Vec3(1.f, 1.f, 1.f), ngl::Vec2(-9.f, -17.f));
  m_buttons.push_back(m_buttonLevel1);

  m_buttonLevel2 = new UIButton(buttonLevel2, "../font/arial.ttf", 30, ngl::Vec2(m_width, m_height));
  m_buttonLevel2->updateButton(ngl::Vec2(-0.1f, -0.1f), ngl::Vec2(0.1f, 0.1f), ngl::Vec4(0.2f, 0.2f, 0.9f, 1.f));
  m_buttonLevel2->updateText("2", ngl::Vec3(1.f, 1.f, 1.f), ngl::Vec2(-9.f, -17.f));
  m_buttons.push_back(m_buttonLevel2);

  m_buttonLevel3 = new UIButton(buttonLevel3, "../font/arial.ttf", 30, ngl::Vec2(m_width, m_height));
  m_buttonLevel3->updateButton(ngl::Vec2(0.f, -0.1f), ngl::Vec2(0.1f, 0.1f), ngl::Vec4(0.2f, 0.2f, 0.9f, 1.f));
  m_buttonLevel3->updateText("3", ngl::Vec3(1.f, 1.f, 1.f), ngl::Vec2(-9.f, -17.f));
  m_buttons.push_back(m_buttonLevel3);

  m_buttonLevel4 = new UIButton(buttonLevel4, "../font/arial.ttf", 30, ngl::Vec2(m_width, m_height));
  m_buttonLevel4->updateButton(ngl::Vec2(0.1f, -0.1f), ngl::Vec2(0.1f, 0.1f), ngl::Vec4(0.2f, 0.2f, 0.9f, 1.f));
  m_buttonLevel4->updateText("4", ngl::Vec3(1.f, 1.f, 1.f), ngl::Vec2(-9.f, -17.f));
  m_buttons.push_back(m_buttonLevel4);

  m_buttonLevel5 = new UIButton(buttonLevel5, "../font/arial.ttf", 30, ngl::Vec2(m_width, m_height));
  m_buttonLevel5->updateButton(ngl::Vec2(0.2f, -0.1f), ngl::Vec2(0.1f, 0.1f), ngl::Vec4(0.2f, 0.2f, 0.9f, 1.f));
  m_buttonLevel5->updateText("5", ngl::Vec3(1.f, 1.f, 1.f), ngl::Vec2(-9.f, -17.f));
  m_buttons.push_back(m_buttonLevel5);

  m_buttonQuit = new UIButton(buttonQuit,  "../font/arial.ttf", 40, ngl::Vec2(m_width, m_height));
  m_buttonQuit->updateButton(ngl::Vec2(0.f, -0.3f), ngl::Vec2(0.5f, 0.2f), ngl::Vec4(0.2f, 0.2f, 0.9f, 1.f));
  m_buttonQuit->updateText("Quit", ngl::Vec3(1.f, 1.f, 1.f), ngl::Vec2(-9.f, -17.f));
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

  m_buttonSquadPatrol = new UIButton(buttonSquadPatrol, "../font/arial.ttf", 20, ngl::Vec2(m_width, m_height));
  m_buttonSquadPatrol->updateButton(ngl::Vec2(0.7f, -0.9f), ngl::Vec2(0.1f, 0.1f), ngl::Vec4(0.2f, 0.2f, 0.9f, 1.f));
  m_buttonSquadPatrol->updateText("Patr", ngl::Vec3(1.f, 1.f, 1.f), ngl::Vec2(-20.f, -13.f));
  m_buttons.push_back(m_buttonSquadPatrol);

  m_buttonSquadAggressive = new UIButton(buttonSquadAggressive, "../font/arial.ttf", 20, ngl::Vec2(m_width, m_height));
  m_buttonSquadAggressive->updateButton(ngl::Vec2(0.5f, -0.9f), ngl::Vec2(0.1f, 0.1f), ngl::Vec4(0.2f, 0.2f, 0.9f, 1.f));
  m_buttonSquadAggressive->updateText("Aggr", ngl::Vec3(1.f, 1.f, 1.f), ngl::Vec2(-20.f, -13.f));
  m_buttons.push_back(m_buttonSquadAggressive);

  m_buttonSquadDefensive = new UIButton(buttonSquadDefensive, "../font/arial.ttf", 20, ngl::Vec2(m_width, m_height));
  m_buttonSquadDefensive->updateButton(ngl::Vec2(0.6f, -0.9f), ngl::Vec2(0.1f, 0.1f), ngl::Vec4(0.2f, 0.2f, 0.9f, 1.f));
  m_buttonSquadDefensive->updateText("Def", ngl::Vec3(1.f, 1.f, 1.f), ngl::Vec2(-20.f, -13.f));
  m_buttons.push_back(m_buttonSquadDefensive);

  m_buttonSquadWall = new UIButton(buttonSquadWall, "../font/arial.ttf", 20, ngl::Vec2(m_width, m_height));
  m_buttonSquadWall->updateButton(ngl::Vec2(0.4f, -0.9f), ngl::Vec2(0.1f, 0.1f), ngl::Vec4(0.2f, 0.2f, 0.9f, 1.f));
  m_buttonSquadWall->updateText("Wall", ngl::Vec3(1.f, 1.f, 1.f), ngl::Vec2(-20.f, -13.f));
  m_buttons.push_back(m_buttonSquadWall);

  m_sliderSquadSize = new UISlider(sliderSquadSize, "../font/arial.ttf", 40, ngl::Vec2(m_width, m_height));
  m_sliderSquadSize->updateSlider(ngl::Vec2(-0.5f, -0.9f), ngl::Vec2(0.5f, 0.1f), ngl::Vec4(0.2f, 0.2f, 0.9f, 1.f), ngl::Vec2(-0.5f, -0.9f), ngl::Vec2(0.02f, 0.1f), ngl::Vec4(1.f, 1.f, 1.f, 1.f), 3, 9);
  m_squadSize = m_sliderSquadSize->calculateOutput();
  m_ss.str(std::string());
  m_ss << m_squadSize;
  m_squadSizeString = m_ss.str();
  m_sliderSquadSize->updateText(m_squadSizeString, ngl::Vec3(1.f, 1.f, 1.f), ngl::Vec2(177.f, -27.f));
}




