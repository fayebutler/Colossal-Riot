//----------------------------------------------------------------------------------------------------------------------
/// @file UIButton.cpp
/// @brief A simple button class that uses SDL TTF text and NGL triangle planes
//----------------------------------------------------------------------------------------------------------------------

#include "UIButton.h"
#include "ngl/VAOPrimitives.h"
#include "ngl/ShaderLib.h"
#include <ngl/Camera.h>

//----------------------------------------------------------------------------------------------------------------------
UIButton::UIButton(eButtonName _name, ngl::Vec2 _windowDimensions)
{
  m_name = _name;
  m_windowDimensions = _windowDimensions;
  m_hasText = false;
  m_isActive = true;
}

//----------------------------------------------------------------------------------------------------------------------
UIButton::UIButton(eButtonName _name, std::string _font, int _fontSize, ngl::Vec2 _windowDimensions)
{
  m_name = _name;
  m_text = new Text(_font, _fontSize);
  m_windowDimensions = _windowDimensions;
  m_hasText = true;
  m_isActive = true;
}

//----------------------------------------------------------------------------------------------------------------------
UIButton::~UIButton()
{
  if (m_hasText)
  {
    delete m_text;
  }
}

//----------------------------------------------------------------------------------------------------------------------
void UIButton::updateButton(ngl::Vec2 _pos, ngl::Vec2 _dimensions, ngl::Vec4 _colour)
{
  m_buttonPos = _pos;
  m_buttonDimensions = _dimensions;
  m_buttonColour = _colour;
}

//----------------------------------------------------------------------------------------------------------------------
void UIButton::updateText(std::string _text, ngl::Vec3 _colour, ngl::Vec2 _offset)
{
  if (m_hasText == true)
  {
    m_textString = _text;
    m_text->setColour(_colour.m_x, _colour.m_y, _colour.m_z);
    m_textOffset = _offset;

    m_textPos.m_x = ((m_buttonPos.m_x + 1) * m_windowDimensions.m_x) / 2;
    m_textPos.m_y = ((1 - m_buttonPos.m_y) * m_windowDimensions.m_y) / 2;
  }
  else
  {
    std::cout<<"Error: Text has not been initialised for this button."<<std::endl;
  }
}

//----------------------------------------------------------------------------------------------------------------------
void UIButton::addText(std::string _font, int _fontSize)
{
  if (m_hasText == false)
  {
    m_text = new Text(_font, _fontSize);
    m_hasText = true;
  }
  else
  {
    std::cout<<"Error: This button already has text."<<std::endl;
  }
}

//----------------------------------------------------------------------------------------------------------------------
bool UIButton::isClicked(int _x, int _y)
{
  ngl::Vec2 ndc;
  ndc.m_x = ((2.f * _x) / m_windowDimensions.m_x) - 1.f;
  ndc.m_y = 1.f - ((2.f * _y) / m_windowDimensions.m_y);

  if (ndc.m_x >= (m_buttonPos.m_x - m_buttonDimensions.m_x/2) && ndc.m_x <= (m_buttonPos.m_x + m_buttonDimensions.m_x/2) &&
      ndc.m_y >= (m_buttonPos.m_y - m_buttonDimensions.m_y/2) && ndc.m_y <= (m_buttonPos.m_y + m_buttonDimensions.m_y/2))
  {
    return true;
  }
  else
  {
    return false;
  }
}

//----------------------------------------------------------------------------------------------------------------------
std::string UIButton::getTextString()
{
  if (m_hasText)
  {
    return m_textString;
  }
  else
  {
    std::cout<<"The button does not have text"<<std::endl;
  }
}

//----------------------------------------------------------------------------------------------------------------------
void UIButton::setTextString(std::string _text)
{
  if (m_hasText)
  {
    m_textString = _text;
  }
  else
  {
    std::cout<<"The button does not have text"<<std::endl;
  }
}

//----------------------------------------------------------------------------------------------------------------------
ngl::Vec2 UIButton::getTextOffset()
{
  if (m_hasText)
  {
    return m_textOffset;
  }
  else
  {
    std::cout<<"The button does not have text"<<std::endl;
  }
}

//----------------------------------------------------------------------------------------------------------------------
void UIButton::setTextOffset(ngl::Vec2 _offset)
{
  if (m_hasText)
  {
    m_textOffset = _offset;
  }
  else
  {
    std::cout<<"The button does not have text"<<std::endl;
  }
}

//----------------------------------------------------------------------------------------------------------------------
void UIButton::setTextColour(ngl::Vec3 _colour)
{
  if (m_hasText)
  {
    m_text->setColour(_colour.m_x, _colour.m_y, _colour.m_z);
  }
  else
  {
    std::cout<<"The button does not have text"<<std::endl;
  }
}

//----------------------------------------------------------------------------------------------------------------------
void UIButton::draw()
{
  loadMatricesToShader();
  ngl::VAOPrimitives::instance()->createTrianglePlane("button", m_buttonDimensions.m_x, m_buttonDimensions.m_y, 1, 1, ngl::Vec3(0, 1, 0));
  ngl::VAOPrimitives::instance()->draw("button");
  if (m_hasText == true)
  {
    m_text->renderText(m_textPos.m_x + m_textOffset.m_x, m_textPos.m_y + m_textOffset.m_y, m_textString);
  }
}

//----------------------------------------------------------------------------------------------------------------------
void UIButton::loadMatricesToShader()
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  (*shader)["nglColourShader"]->use();
  shader->setShaderParam4f("Colour", m_buttonColour.m_x, m_buttonColour.m_y,m_buttonColour.m_z, 1.f);

  ngl::Transformation t;
  t.setPosition(ngl::Vec3(m_buttonPos.m_x, m_buttonPos.m_y, 0.f));
  t.setRotation(90.f, 0.f, 0.f);

  ngl::Mat4 M;
  ngl::Mat4 MV;
  ngl::Mat4 MVP;
  M=t.getMatrix();
  MV=  M*ngl::lookAt(ngl::Vec3(0, 0, 10), ngl::Vec3(0, 0, 0), ngl::Vec3(0, 1, 0));
  MVP= MV*ngl::ortho(-1, 1, -1, 1, 0.1f, 100.f);
  shader->setShaderParamFromMat4("MVP",MVP);
}
