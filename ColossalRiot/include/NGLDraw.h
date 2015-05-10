#ifndef NGLDRAW_H__
#define NGLDRAW_H__

#include <SDL.h>
#include <ngl/Camera.h>
#include <ngl/Light.h>
#include <ngl/SpotLight.h>
#include "GameWorld.h"
#include "Police.h"
#include "Rioter.h"
#include "UIButton.h"
#include "UISlider.h"
#include "Text.h"
#include "Timer.h"
#include "EntityManager.h"

enum eGameState
{
    gameMenu,
    gamePlay,
    gamePause,
    gameLose,
    gameWin,
    gameQuit
};

class NGLDraw
{
  public :
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief ctor this will have a valid OpenGL context so we can create gl stuff
    //----------------------------------------------------------------------------------------------------------------------
    NGLDraw();
    NGLDraw(int _width, int _height);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief dtor used to remove any NGL stuff created
    //----------------------------------------------------------------------------------------------------------------------
    ~NGLDraw();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief resize our screen and set the camera aspect ratio
    /// @param[in] _w the new width
    /// @param[in] _h the new height
    //----------------------------------------------------------------------------------------------------------------------
    void resize(int _w, int _h);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief draw the scene
    //----------------------------------------------------------------------------------------------------------------------
    void draw();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief update the scene
    /// @param _timeElapsed the time elapsed between each frame
    /// @param _currentTime the current time of the current level
    //----------------------------------------------------------------------------------------------------------------------
    void update(double _timeElapsed, double _currentTime);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this method is called every time a mouse is moved
    /// @param _event the SDL mouse event structure containing all mouse info
    //----------------------------------------------------------------------------------------------------------------------
    void mouseMoveEvent (const SDL_MouseMotionEvent &_event);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this method is called every time a mouse button is presses
    /// @param _event the SDL mouse event structure containing all mouse info
    //----------------------------------------------------------------------------------------------------------------------
    void mousePressEvent (const SDL_MouseButtonEvent &_event);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this method is called everytime the mouse button is released
    /// @param _event the SDL mouse event structure containing all mouse info
    //----------------------------------------------------------------------------------------------------------------------
    void mouseReleaseEvent (const SDL_MouseButtonEvent &_event);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this method is called everytime the mouse wheel is moved
    /// @param _event the SDL mouse event structure containing all mouse info
    //----------------------------------------------------------------------------------------------------------------------
    void wheelEvent(const SDL_MouseWheelEvent &_event);

    void doSelection(const int _x, const int _y);

    void doMovement(const int _x, const int _y);

    ngl::Vec3 getWorldSpace(int _x, int _y);

    void startGame(int level);
    void endGame();

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this method returns the current state of the game, such as gamePlay, gameMenu, gamePause
    /// @return returns the game state as a eGameState enum
    //----------------------------------------------------------------------------------------------------------------------
    eGameState getGameState()const{return m_gameState;}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this method sets the current state of the game to an enum of type eGameState
    //----------------------------------------------------------------------------------------------------------------------
    void setGameState(eGameState _gameState){m_gameState = _gameState;}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this method is used to initialise all the buttons and sliders that make up the UI
    //----------------------------------------------------------------------------------------------------------------------
    void initialiseUI();

  private :
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief method to load transform data to the shaders
    //----------------------------------------------------------------------------------------------------------------------
    void loadMatricesToShader();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief used to store the x rotation mouse value
    //----------------------------------------------------------------------------------------------------------------------
    int m_spinXFace;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief used to store the y rotation mouse value
    //----------------------------------------------------------------------------------------------------------------------
    int m_spinYFace;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief flag to indicate if the mouse button is pressed when dragging
    //----------------------------------------------------------------------------------------------------------------------
    bool m_rotate;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief flag to indicate if the Right mouse button is pressed when dragging
    //----------------------------------------------------------------------------------------------------------------------
    bool m_translate;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the previous x mouse value
    //----------------------------------------------------------------------------------------------------------------------
    int m_origX;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the previous y mouse value
    //----------------------------------------------------------------------------------------------------------------------
    int m_origY;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the previous x mouse value for Position changes
    //----------------------------------------------------------------------------------------------------------------------
    int m_origXPos;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the previous y mouse value for Position changes
    //----------------------------------------------------------------------------------------------------------------------
    int m_origYPos;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief used to store the global mouse transforms
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Mat4 m_mouseGlobalTX;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Our Camera
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Camera *m_cam;
    ngl::Camera *m_camOrth;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the model position for mouse movement
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Vec3 m_modelPos;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief a simple light use to illuminate the screen
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Light *m_light;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief a spotlight that points from the camera to the floor
    //----------------------------------------------------------------------------------------------------------------------
    ngl::SpotLight m_spot;
    //----------------------------------------------------------------------------------------------------------------------
    /// @ brief a NGL transformation member
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Transformation m_trans;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief a pointer to the game world created when a new level starts
    //----------------------------------------------------------------------------------------------------------------------
    GameWorld* m_gameworld;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the currently selected squad
    //----------------------------------------------------------------------------------------------------------------------
    Squad* m_selectedSquad;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the currently selected squad ID
    //----------------------------------------------------------------------------------------------------------------------
    int m_selectedSquadID;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the position of the mouse when it is clicked
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Vec3 m_clickPosition;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief a boolean determining if something is selected
    //----------------------------------------------------------------------------------------------------------------------
    bool m_selected;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the current game state
    //----------------------------------------------------------------------------------------------------------------------
    eGameState m_gameState;

    std::vector<UIButton*> m_buttons;

    UIButton* m_buttonPlay;
    UIButton* m_buttonLevel1;
    UIButton* m_buttonLevel2;
    UIButton* m_buttonLevel3;
    UIButton* m_buttonLevel4;
    UIButton* m_buttonLevel5;

    int m_selectedLevel;

    UIButton* m_buttonQuit;
    UIButton* m_buttonPause;
    UIButton* m_buttonMenu;
    UIButton* m_buttonCreateSquad;

    UIButton* m_buttonSquadPatrol;
    UIButton* m_buttonSquadAggressive;
    UIButton* m_buttonSquadDefensive;
    UIButton* m_buttonSquadWall;

    UIButton* m_buttonRioterDeadBar;
    UIButton* m_buttonRioterHomeBar;

    UISlider* m_sliderSquadSize;

    Text* m_textSmall;
    Text* m_textMedium;
    Text* m_textLarge;

    Timer m_gameTimer;

    int m_squadSize;
    std::string m_squadSizeString;

    std::stringstream m_ss;

    EntityManager* m_entityMgr;

    float m_width;
    float m_height;
    float m_longestSide;

    ngl::Colour m_squadCurrentColour;
    ngl::Colour m_squadSelectedColour;



};


#endif
