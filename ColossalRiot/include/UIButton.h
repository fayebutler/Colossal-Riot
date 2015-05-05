#ifndef UIBUTTON_H
#define UIBUTTON_H
#include <ngl/Transformation.h>
#include <ngl/Camera.h>
#include "Text.h"
class UIButton
{
public:
  UIButton();
  UIButton(std::string _font, int _fontSize);
  ~UIButton();

  void addText(std::string _font, int _fontSize);

  void updateButton(ngl::Vec2 _pos, ngl::Vec2 _dimensions, ngl::Vec4 _colour);
  void updateText(std::string _text, ngl::Vec3 _colour, ngl::Vec2 _offset);

  ngl::Vec2 getButtonPosition() { return m_buttonPos; }
  void setButtonPosition(ngl::Vec2 _pos) { m_buttonPos = _pos; }
  ngl::Vec2 getButtonDimension() { return m_buttonDimensions; }
  void setButtonDimension(ngl::Vec2 _dimensions) { m_buttonDimensions = _dimensions; }
  ngl::Vec4 getButtonColour() { return m_buttonColour; }
  void setButtonColour(ngl::Vec4 _colour) { m_buttonColour = _colour; }

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
  void executeClick();


  void draw();
  void loadMatricesToShader();


private:
  ngl::Vec2 m_buttonPos;
  ngl::Vec2 m_buttonDimensions;
  ngl::Vec4 m_buttonColour;

  Text* m_text;
  bool m_hasText;
  std::string m_textString;
  ngl::Vec2 m_textPos;
  ngl::Vec2 m_textOffset;

  ngl::Vec2 m_screenDimensions;
};

#endif // UIBUTTON_H
