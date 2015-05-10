#ifndef UISLIDER_H
#define UISLIDER_H

#include <ngl/Transformation.h>
#include "Text.h"

//----------------------------------------------------------------------------------------------------------------------
/// @file UISlider.h
/// @class UISlider "include/UISlider.h"
/// @brief A simple slider class that uses SDL TTF text and NGL triangle planes
/// @author Will Herbert
/// @version 1.0
/// @date Last revision 10/05/2015 Updated to comply with doxygen and NCCA coding standard
//----------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------
/// @brief enum to describe the name of an individual slider
//----------------------------------------------------------------------------------------------------------------------
enum eSliderName
{
  sliderSquadSize // 0
};

class UISlider
{
public:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief ctor to create a simple slider that does not have any text functionality
  /// @param[in] _name enum name to set as the name of the slider
  /// @param[in] _windowDimensions width and height of the initial window, used for correct UI scaling
  //----------------------------------------------------------------------------------------------------------------------
  UISlider(eSliderName _name, ngl::Vec2 _windowDimensions);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief ctor to create a simple slider that has additional text functionality
  /// @param[in] _name enum name to set as the name of the slider
  /// @param[in] _font the font of the text, cannot be changed after construction
  /// @param[in] _fontSize the size of the text, cannot be changed after contruction
  /// @param[in] _windowDimensions width and height of the initial window, used for correct UI scaling
  //----------------------------------------------------------------------------------------------------------------------
  UISlider(eSliderName _name, std::string _font, int _fontSize, ngl::Vec2 _windowDimensions);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief dtor cleans up any text pointeres if necessary
  //----------------------------------------------------------------------------------------------------------------------
  ~UISlider();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method used to updated the button position, dimensions and colour
  /// @param[in] _pos input NDC position to set as new button position
  /// @param[in] _dimensions input NDC dimensions to set as new button dimensions
  /// @param[in] _colour input colour to set as the new button colour
  /// @param[in] _barPos input NDC position to set as new slider bar position
  /// @param[in] _barDimensions input NDC dimensions to set as new slider bar dimensions
  /// @param[in] _barColour input colour to set as the new slider bar colour
  /// @param[in] _outputMin the minimum integer value that the slider can output
  /// @param[in] _outputMax the maximum integer value taht the slider can ouput
  //----------------------------------------------------------------------------------------------------------------------
  void updateSlider(ngl::Vec2 _pos, ngl::Vec2 _dimensions, ngl::Vec4 _colour,
                    ngl::Vec2 _barPos, ngl::Vec2 _barDimensions, ngl::Vec4 _barColour,
                    int _outputMin, int _outputMax);
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
  /// @brief method called while the slider is being clicked to update the position of the slider bar
  /// @param[in] _x the input x pixel position of the mouse
  //----------------------------------------------------------------------------------------------------------------------
  int slideBar(int _x);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method used to calculate the output of the slider, based on the ouput min, max and slider bar position
  //----------------------------------------------------------------------------------------------------------------------
  int calculateOutput();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief gets the enum name of the button
  /// @return enum eSliderName the enum value of the name
  //----------------------------------------------------------------------------------------------------------------------
  eSliderName getName() { return m_name; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief sets the enum name of the button
  /// @param[in] _name the input enum name to give to the button
  //----------------------------------------------------------------------------------------------------------------------
  void setName(eSliderName _name) { m_name = _name; }
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
  /// @brief gets if the slider is sliding or not
  /// @return bool true if the slider is sliding, false if it is not sliding
  //----------------------------------------------------------------------------------------------------------------------
  bool getIsSliding() { return m_isSliding; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief sets if the slider is sliding or not
  /// @param[in] _inBool the boolean to set if the slider is sliding or not sliding
  //----------------------------------------------------------------------------------------------------------------------
  void setIsSliding(const bool _inBool) { m_isSliding = _inBool; }
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
  /// @brief gets the slider position in NDC
  /// @return ngl::Vec2 the x and y position of the slider in NDC
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec2 getSliderPosition() { return m_sliderPos; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief sets the slider position in NDC
  /// @param[in] _pos the x and y position in NDC to set the slider to
  //----------------------------------------------------------------------------------------------------------------------
  void setSliderPosition(const ngl::Vec2 _pos) { m_sliderPos = _pos; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief gets the slider dimensions in NDC
  /// @return ngl::Vec2 the x and y dimensions of the slider in NDC
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec2 getSliderDimension() { return m_sliderDimensions; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief sets the slider dimensions in NDC
  /// @param[in] _dimensions the x and y dimensions in NDC to set the slider to
  //----------------------------------------------------------------------------------------------------------------------
  void setSliderDimension(const ngl::Vec2 _dimensions) { m_sliderDimensions = _dimensions; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief gets the colour of the slider
  /// @return ngl::Vec4 the r, g, b, a values of the slider colour
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec4 getSliderColour() { return m_sliderColour; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief sets the colour of the slider
  /// @param[in] _colour the r, g, b, a to assign to the slider colour
  //----------------------------------------------------------------------------------------------------------------------
  void setSliderColour(const ngl::Vec4 _colour) { m_sliderColour = _colour; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief gets the slider bar position in NDC
  /// @return ngl::Vec2 the x and y position of the slider bar in NDC
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec2 getSliderBarPosition() { return m_sliderBarPos; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief sets the slider bar position in NDC
  /// @param[in] _pos the x and y position in NDC to set the slider bar to
  //----------------------------------------------------------------------------------------------------------------------
  void setSliderBarPosition(const ngl::Vec2 _pos) { m_sliderBarPos = _pos; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief gets the slider bar dimensions in NDC
  /// @return ngl::Vec2 the x and y dimensions of the slider bar in NDC
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec2 getSliderBarDimension() { return m_sliderBarDimensions; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief sets the slider bar dimensions in NDC
  /// @param[in] _dimensions the x and y dimensions in NDC to set the slider bar to
  //----------------------------------------------------------------------------------------------------------------------
  void setSliderBarDimension(const ngl::Vec2 _dimensions) { m_sliderBarDimensions = _dimensions; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief gets the colour of the slider bar
  /// @return ngl::Vec4 the r, g, b, a values of the slider bar colour
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec4 getSliderBarColour() { return m_sliderBarColour; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief sets the colour of the slider bar
  /// @param[in] _colour the r, g, b, a to assign to the slider bar colour
  //----------------------------------------------------------------------------------------------------------------------
  void setSliderBarColour(const ngl::Vec4 _colour) { m_sliderBarColour = _colour; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief gets if the button has valid text
  /// @return bool returns true if the button has text, false if it does not have text
  //----------------------------------------------------------------------------------------------------------------------
  bool getHasText();
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
  void setTextString(const std::string _text);
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
  void setTextOffset(const ngl::Vec2 _offset);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief sets the colour of the text
  /// @param[in] _colour the r, g, b values to be assigned to the text colour
  //----------------------------------------------------------------------------------------------------------------------
  void setTextColour(const ngl::Vec3 _colour);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief draws the button, and any text if necessary
  //----------------------------------------------------------------------------------------------------------------------
  void draw();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to load transform data to the shaders
  /// @param[in] _pos the position of the slider or bar to be rendered
  /// @param[in] _colour the colour of the slider or bar to be rendered
  //----------------------------------------------------------------------------------------------------------------------
  void loadMatricesToShader(ngl::Vec2 _pos, ngl::Vec4 _colour);

private:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the enum name used by the button
  //----------------------------------------------------------------------------------------------------------------------
  eSliderName m_name;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief boolean to determin if the button is active
  //----------------------------------------------------------------------------------------------------------------------
  bool m_isActive;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief boolean to determin if the button is sliding
  //----------------------------------------------------------------------------------------------------------------------
  bool m_isSliding;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the x and y values of the window dimensions
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec2 m_windowDimensions;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the x and y position of the slider in NDC
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec2 m_sliderPos;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the x and y dimensions of the slider in NDC
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec2 m_sliderDimensions;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the r, g, b, a values of the slider colour
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec4 m_sliderColour;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the x and y position of the slider bar in NDC
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec2 m_sliderBarPos;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the x and y dimensions of the slider bar in NDC
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec2 m_sliderBarDimensions;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the r, g, b, a values of the slider bar colour
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec4 m_sliderBarColour;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the output integer value produced by the slider
  //----------------------------------------------------------------------------------------------------------------------
  int m_output;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the inclusive minimum integer output that can be produced by the button
  //----------------------------------------------------------------------------------------------------------------------
  int m_outputMin;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the inclusive maximum integer output that can be produced by the slider
  //----------------------------------------------------------------------------------------------------------------------
  int m_outputMax;
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

#endif // UISLIDER_H
