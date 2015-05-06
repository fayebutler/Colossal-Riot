#ifndef UISLIDER_H
#define UISLIDER_H

#include <ngl/Transformation.h>
#include "Text.h"

enum eSliderName
{
  sliderSquadSize,
};

class UISlider
{
public:
  UISlider(eSliderName _name, ngl::Vec2 _screenDimensions);
  UISlider(eSliderName _name, std::string _font, int _fontSize, ngl::Vec2 _screenDimensions);
  ~UISlider();

  void addText(std::string _font, int _fontSize);

  void updateSlider(ngl::Vec2 _pos, ngl::Vec2 _dimensions, ngl::Vec4 _colour, ngl::Vec2 _barPos, ngl::Vec2 _barDimensions, ngl::Vec4 _barColour);
  void updateText(std::string _text, ngl::Vec3 _colour, ngl::Vec2 _offset);

  eSliderName getName() { return m_name; }
  void setName(eSliderName _name) { m_name = _name; }

  bool getIsActive() { return m_isActive; }
  void setIsActive(bool _inBool) { m_isActive = _inBool; }
  bool getIsSliding() { return m_isSliding; }
  void setIsSliding(bool _inBool) { m_isSliding = _inBool; }

  ngl::Vec2 getSliderPosition() { return m_sliderPos; }
  void setSliderPosition(ngl::Vec2 _pos) { m_sliderPos = _pos; }
  ngl::Vec2 getSliderDimension() { return m_sliderDimensions; }
  void setSliderDimension(ngl::Vec2 _dimensions) { m_sliderDimensions = _dimensions; }
  ngl::Vec4 getSliderColour() { return m_sliderColour; }
  void setSliderColour(ngl::Vec4 _colour) { m_sliderColour = _colour; }

  ngl::Vec2 getSliderBarPosition() { return m_sliderBarPos; }
  void setSliderBarPosition(ngl::Vec2 _pos) { m_sliderBarPos = _pos; }
  ngl::Vec2 getSliderBarDimension() { return m_sliderBarDimensions; }
  void setSliderBarDimension(ngl::Vec2 _dimensions) { m_sliderBarDimensions = _dimensions; }
  ngl::Vec4 getSliderBarColour() { return m_sliderBarColour; }
  void setSliderBarColour(ngl::Vec4 _colour) { m_sliderBarColour = _colour; }

  bool getHasText() { return m_hasText; }
  std::string getTextString() { return m_textString; }
  void setTextString(std::string _text) { m_textString = _text; }
  ngl::Vec2 getTextPos() { return m_textPos; }
  void setTextPos(ngl::Vec2 _pos) { m_textPos = _pos; }
  ngl::Vec2 getTextOffset() { return m_textOffset; }
  void setTextOffset(ngl::Vec2 _offset) { m_textOffset = _offset; }
  void setTextColour(ngl::Vec3 _colour) { m_text->setColour(_colour.m_x, _colour.m_y, _colour.m_z); }

  ngl::Vec2 getScreenDimensions() { return m_screenDimensions; }
  void setScreenDimensions(ngl::Vec2 _screenDimensions) { m_screenDimensions = _screenDimensions; }

  bool isClicked(int _x, int _y);
  void slideBar(int _x);
  void executeClick();


  void draw();
  void loadMatricesToShader(ngl::Vec2 _pos, ngl::Vec4 _colour);


private:
  eSliderName m_name;

  bool m_isActive;
  bool m_isSliding;

  ngl::Vec2 m_sliderPos;
  ngl::Vec2 m_sliderDimensions;
  ngl::Vec4 m_sliderColour;

  ngl::Vec2 m_sliderBarPos;
  ngl::Vec2 m_sliderBarDimensions;
  ngl::Vec4 m_sliderBarColour;

  Text* m_text;
  bool m_hasText;
  std::string m_textString;
  ngl::Vec2 m_textPos;
  ngl::Vec2 m_textOffset;

  ngl::Vec2 m_screenDimensions;
};

#endif // UISLIDER_H
