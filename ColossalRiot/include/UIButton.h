#ifndef UIBUTTON_H
#define UIBUTTON_H

//----------------------------------------------------------------------------------------------------------------------
/// @file UIButton.h
/// @class UIButton "include/UIButton.h"
/// @brief A simple button class that uses SDL TTF text and NGL triangle planes
/// @author Will Herbert
/// @version 1.0
/// @date Last revision 10/05/2015 Updated to comply with doxygen and NCCA coding standard
//----------------------------------------------------------------------------------------------------------------------

#include <ngl/Transformation.h>
#include "Text.h"

//----------------------------------------------------------------------------------------------------------------------
/// @brief enum to describe the name of an individual button
//----------------------------------------------------------------------------------------------------------------------
enum eButtonName
{
  buttonPlay,             // 0
  buttonLevel1,           // 1
  buttonLevel2,           // 2
  buttonLevel3,           // 3
  buttonLevel4,           // 4
  buttonLevel5,           // 5
  buttonQuit,             // 6
  buttonPause,            // 7
  buttonMenu,             // 8
  buttonCreateSquad,      // 9
  buttonSquadWall,        // 10
  buttonSquadAggressive,  // 11
  buttonSquadDefensive,   // 12
  buttonSquadPatrol,      // 13
  buttonRioterDeadBar,    // 14
  buttonRioterHomeBar     // 15
};

class UIButton
{
public:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief ctor to create a simple button that does not have any text functionality
  /// @param[in] _name enum name to set as the name of the button
  /// @param[in] _windowDimensions width and height of the initial window, used for correct UI scaling
  //----------------------------------------------------------------------------------------------------------------------
  UIButton(eButtonName _name, ngl::Vec2 _windowDimensions);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief ctor to create a simple button that has additional text functionality
  /// @param[in] _name enum name to set as the name of the button
  /// @param[in] _font the font of the text, cannot be changed after construction
  /// @param[in] _fontSize the size of the text, cannot be changed after contruction
  /// @param[in] _windowDimensions width and height of the initial window, used for correct UI scaling
  //----------------------------------------------------------------------------------------------------------------------
  UIButton(eButtonName _name, std::string _font, int _fontSize, ngl::Vec2 _windowDimensions);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief dtor cleans up any text pointeres if necessary
  //----------------------------------------------------------------------------------------------------------------------
  ~UIButton();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method used to updated the button position, dimensions and colour
  /// @param[in] _pos input NDC position to set as new button position
  /// @param[in] _dimensions input NDC dimensions to set as new button dimensions
  /// @param[in] _colour input colour to set as the new button colour
  //----------------------------------------------------------------------------------------------------------------------
  void updateButton(ngl::Vec2 _pos, ngl::Vec2 _dimensions, ngl::Vec4 _colour);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method used to update the text string, colour and offset from the button centre
  /// @param[in] _text input std::string to set as the new text string to display
  /// @param[in] _colour input colour to set as the new text colour
  /// @param[in] _offset input pixel offset to set as the new text offset from the button centre
  //----------------------------------------------------------------------------------------------------------------------
  void updateText(std::string _text, ngl::Vec3 _colour, ngl::Vec2 _offset);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief if the button does not have text it adds text drawing functionality
  /// @param[in] _font the font of the text, cannot be changed after addition
  /// @param[in] _fontSize the size of the text, cannot be changed after addition
  //----------------------------------------------------------------------------------------------------------------------
  void addText(std::string _font, int _fontSize);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief a method to detect if a button has been clicked, based on it's NDC screen position and dimensions
  /// @param[in] _x the input x pixel position of the mouse click
  /// @param[in] _y the input y pixel position of the mouse click
  //----------------------------------------------------------------------------------------------------------------------
  bool isClicked(int _x, int _y);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief gets the enum name of the button
  /// @return enum eButtonName the enum value of the name
  //----------------------------------------------------------------------------------------------------------------------
  eButtonName getName() { return m_name; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief sets the enum name of the button
  /// @param[in] _name the input enum name to give to the button
  //----------------------------------------------------------------------------------------------------------------------
  void setName(const eButtonName _name) { m_name = _name; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief gets if the button is active or not
  /// @return bool true if the button is active, false if it is inactive
  //----------------------------------------------------------------------------------------------------------------------
  bool getIsActive() { return m_isActive; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief sets if the button is active or inactive
  /// @param[in] _inBool the boolean to set if the button is active or inactive
  //----------------------------------------------------------------------------------------------------------------------
  void setIsActive(const bool _inBool) { m_isActive = _inBool; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief gets the window dimensions that are used by the button for scaling
  /// @return ngl::Vec2 the width and height of the window dimensions
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec2 getWindowDimensions() { return m_windowDimensions; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief sets the window dimensions to be used by the button for scaling
  /// @param[in] _windowDimensions the width and height to be used by the button for scaling
  //----------------------------------------------------------------------------------------------------------------------
  void setWindowDimensions(const ngl::Vec2 _windowDimensions) { m_windowDimensions = _windowDimensions; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief gets the button position in NDC
  /// @return ngl::Vec2 the x and y position of the button in NDC
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec2 getButtonPosition() { return m_buttonPos; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief sets the button position in NDC
  /// @param[in] _pos the x and y position in NDC to set the button to
  //----------------------------------------------------------------------------------------------------------------------
  void setButtonPosition(const ngl::Vec2 _pos) { m_buttonPos = _pos; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief gets the button dimensions in NDC
  /// @return ngl::Vec2 the x and y dimensions of the button in NDC
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec2 getButtonDimension() { return m_buttonDimensions; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief sets the button dimensions in NDC
  /// @param[in] _dimensions the x and y dimensions in NDC to set the button to
  //----------------------------------------------------------------------------------------------------------------------
  void setButtonDimension(const ngl::Vec2 _dimensions) { m_buttonDimensions = _dimensions; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief gets the colour of the button
  /// @return ngl::Vec4 the r, g, b, a values of the button colour
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec4 getButtonColour() { return m_buttonColour; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief sets the colour of the button
  /// @param[in] _colour the r, g, b, a to assign to the button colour
  //----------------------------------------------------------------------------------------------------------------------
  void setButtonColour(const ngl::Vec4 _colour) { m_buttonColour = _colour; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief gets if the button has valid text
  /// @return bool returns true if the button has text, false if it does not have text
  //----------------------------------------------------------------------------------------------------------------------
  bool getHasText() { return m_hasText; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief sets if the button has text or does not have text
  /// @param[in] _inBool the boolean to set if the button has text or does not have text
  //----------------------------------------------------------------------------------------------------------------------
  void setHasText(const bool _inBool) { m_hasText = _inBool; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief gets the text string that is being displayed
  /// @return std::string returns the text string that is being displayed
  //----------------------------------------------------------------------------------------------------------------------
  std::string getTextString();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief sets the text string of the button
  /// @param[in] _text the text string input to display
  //----------------------------------------------------------------------------------------------------------------------
  void setTextString(std::string _text);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief gets the position of the text in NDC
  /// @return the x and y position of the text in NDC
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec2 getTextPos();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief gets the amout of pixel offset applied to the text
  /// @return the x and y pixel offset applied to the text
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec2 getTextOffset();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief sets the pixel offset of the text from the centre of the button
  /// @param[in] _offset the x and y pixel offset to be applied to the button
  //----------------------------------------------------------------------------------------------------------------------
  void setTextOffset(ngl::Vec2 _offset);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief sets the colour of the text
  /// @param[in] _colour the r, g, b values to be assigned to the text colour
  //----------------------------------------------------------------------------------------------------------------------
  void setTextColour(ngl::Vec3 _colour);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief draws the button, and any text if necessary
  //----------------------------------------------------------------------------------------------------------------------
  void draw();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to load transform data to the shaders
  //----------------------------------------------------------------------------------------------------------------------
  void loadMatricesToShader();

private:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the enum name used by the button
  //----------------------------------------------------------------------------------------------------------------------
  eButtonName m_name;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief boolean to determin if the button is active
  //----------------------------------------------------------------------------------------------------------------------
  bool m_isActive;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the x and y values of the window dimensions
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec2 m_windowDimensions;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the x and y position of the button in NDC
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec2 m_buttonPos;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the x and y dimensions of the button in NDC
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec2 m_buttonDimensions;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the r, g, b, a values of the button colour
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec4 m_buttonColour;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief pointer to a text object
  //----------------------------------------------------------------------------------------------------------------------
  Text* m_text;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief boolean to determin if the button has text
  //----------------------------------------------------------------------------------------------------------------------
  bool m_hasText;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the text string that is displayed on screen
  //----------------------------------------------------------------------------------------------------------------------
  std::string m_textString;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the position of the text in NDC
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec2 m_textPos;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the pixel offset of the text from the centre of the button
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec2 m_textOffset;
};

#endif // UIBUTTON_H
