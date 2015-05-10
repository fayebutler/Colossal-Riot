#include "UISlider.h"
//----------------------------------------------------------------------------------------------------------------------
/// @file UISlider.cpp
/// @brief A simple slider class that uses SDL TTF text and NGL triangle planes
//----------------------------------------------------------------------------------------------------------------------

#include "ngl/VAOPrimitives.h"
#include "ngl/ShaderLib.h"
#include <ngl/Camera.h>
//----------------------------------------------------------------------------------------------------------------------
UISlider::UISlider(eSliderName _name, ngl::Vec2 _windowDimensions)
{
  m_name = _name;
  m_windowDimensions = _windowDimensions;
  m_hasText = false;
  m_isActive = true;
  m_isSliding = false;
}

//----------------------------------------------------------------------------------------------------------------------
UISlider::UISlider(eSliderName _name, std::string _font, int _fontSize, ngl::Vec2 _windowDimensions)
{
  m_name = _name;
  m_text = new Text(_font, _fontSize);
  m_windowDimensions = _windowDimensions;
  m_hasText = true;
  m_isActive = true;
  m_isSliding = false;
}

//----------------------------------------------------------------------------------------------------------------------
UISlider::~UISlider()
{
  if (m_hasText)
  {
    delete m_text;
  }
}

//----------------------------------------------------------------------------------------------------------------------
void UISlider::addText(std::string _font, int _fontSize)
{
  if (m_hasText == false)
  {
    m_text = new Text(_font, _fontSize);
    m_hasText = true;
  }
  else
  {
    std::cout<<"Error: This slider already has text."<<std::endl;
  }
}

//----------------------------------------------------------------------------------------------------------------------
void UISlider::updateSlider(ngl::Vec2 _pos, ngl::Vec2 _dimensions, ngl::Vec4 _colour,
                            ngl::Vec2 _barPos, ngl::Vec2 _barDimensions, ngl::Vec4 _barColour,
                            int _outputMin, int _outputMax)
{
  m_sliderPos = _pos;
  m_sliderDimensions = _dimensions;
  m_sliderColour = _colour;
  m_sliderBarPos = _barPos;
  m_sliderBarDimensions = _barDimensions;
  m_sliderBarColour = _barColour;
  m_outputMin = _outputMin;
  m_outputMax = _outputMax;
  calculateOutput();
}

//----------------------------------------------------------------------------------------------------------------------
void UISlider::updateText(std::string _text, ngl::Vec3 _colour, ngl::Vec2 _offset)
{
  if (m_hasText == true)
  {
    m_textString = _text;
    m_text->setColour(_colour.m_x, _colour.m_y, _colour.m_z);
    m_textOffset = _offset;

    m_textPos.m_x = ((m_sliderPos.m_x + 1) * m_windowDimensions.m_x) / 2;
    m_textPos.m_y = ((1 - m_sliderPos.m_y) * m_windowDimensions.m_y) / 2;
  }
  else
  {
    std::cout<<"Error: Text has not been initialised for this slider."<<std::endl;
  }
}

//----------------------------------------------------------------------------------------------------------------------
bool UISlider::isClicked(int _x, int _y)
{
  ngl::Vec2 ndc;
  ndc.m_x = ((2.f * _x) / m_windowDimensions.m_x) - 1.f;
  ndc.m_y = 1.f - ((2.f * _y) / m_windowDimensions.m_y);
  if (ndc.m_x >= (m_sliderPos.m_x - m_sliderDimensions.m_x/2) && ndc.m_x <= (m_sliderPos.m_x + m_sliderDimensions.m_x/2) &&
      ndc.m_y >= (m_sliderPos.m_y - m_sliderDimensions.m_y/2) && ndc.m_y <= (m_sliderPos.m_y + m_sliderDimensions.m_y/2))
  {
    return true;
  }
  else
  {
    return false;
  }
}

//----------------------------------------------------------------------------------------------------------------------
int UISlider::slideBar(int _x)
{
  double ndc_x = ((2.f * _x) / m_windowDimensions.m_x) - 1.f;
  if (ndc_x > (m_sliderPos.m_x - m_sliderDimensions.m_x/2) && ndc_x < (m_sliderPos.m_x + m_sliderDimensions.m_x/2))
  {
    m_sliderBarPos.m_x = ndc_x;
  }
  return calculateOutput();
}

//----------------------------------------------------------------------------------------------------------------------
int UISlider::calculateOutput()
{
  double sliderParam = (m_sliderBarPos.m_x - (m_sliderPos.m_x - m_sliderDimensions.m_x/2)) / ((m_sliderPos.m_x + m_sliderDimensions.m_x/2) - (m_sliderPos.m_x - m_sliderDimensions.m_x/2));
  double outputDivisions = 1.0 / ((m_outputMax - m_outputMin) + 1);
  int outputSection = (sliderParam / outputDivisions);
  m_output = m_outputMin + outputSection;
  return m_output;
}

//----------------------------------------------------------------------------------------------------------------------
std::string UISlider::getTextString()
{
  if (m_hasText)
  {
    return m_textString;
  }
  else
  {
    std::cout<<"The slider does not have text"<<std::endl;
  }
}

//----------------------------------------------------------------------------------------------------------------------
void UISlider::setTextString(std::string _text)
{
  if (m_hasText)
  {
    m_textString = _text;
  }
  else
  {
    std::cout<<"The slider does not have text"<<std::endl;
  }
}

//----------------------------------------------------------------------------------------------------------------------
ngl::Vec2 UISlider::getTextOffset()
{
  if (m_hasText)
  {
    return m_textOffset;
  }
  else
  {
    std::cout<<"The slider does not have text"<<std::endl;
  }
}

//----------------------------------------------------------------------------------------------------------------------
void UISlider::setTextOffset(ngl::Vec2 _offset)
{
  if (m_hasText)
  {
    m_textOffset = _offset;
  }
  else
  {
    std::cout<<"The slider does not have text"<<std::endl;
  }
}

//----------------------------------------------------------------------------------------------------------------------
void UISlider::setTextColour(ngl::Vec3 _colour)
{
  if (m_hasText)
  {
    m_text->setColour(_colour.m_x, _colour.m_y, _colour.m_z);
  }
  else
  {
    std::cout<<"The slider does not have text"<<std::endl;
  }
}

//----------------------------------------------------------------------------------------------------------------------
void UISlider::draw()
{
  loadMatricesToShader(m_sliderBarPos, m_sliderBarColour);
  ngl::VAOPrimitives::instance()->createTrianglePlane("sliderBar", m_sliderBarDimensions.m_x, m_sliderBarDimensions.m_y, 1, 1, ngl::Vec3(0, 1, 0));
  ngl::VAOPrimitives::instance()->draw("sliderBar");
  loadMatricesToShader(m_sliderPos, m_sliderColour);
  ngl::VAOPrimitives::instance()->createTrianglePlane("slider", m_sliderDimensions.m_x, m_sliderDimensions.m_y, 1, 1, ngl::Vec3(0, 1, 0));
  ngl::VAOPrimitives::instance()->draw("slider");

  if (m_hasText == true)
  {
    m_text->renderText(m_textPos.m_x + m_textOffset.m_x, m_textPos.m_y + m_textOffset.m_y, m_textString);
  }
}

//----------------------------------------------------------------------------------------------------------------------
void UISlider::loadMatricesToShader(ngl::Vec2 _pos, ngl::Vec4 _colour)
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  (*shader)["nglColourShader"]->use();
  shader->setShaderParam4f("Colour", _colour.m_x, _colour.m_y,_colour.m_z, 1.f);

  ngl::Transformation t;
  t.setPosition(ngl::Vec3(_pos.m_x, _pos.m_y, 0.f));
  t.setRotation(90.f, 0.f, 0.f);

  ngl::Mat4 M;
  ngl::Mat4 MV;
  ngl::Mat4 MVP;
  M=t.getMatrix();
  MV=  M*ngl::lookAt(ngl::Vec3(0, 0, 10), ngl::Vec3(0, 0, 0), ngl::Vec3(0, 1, 0));
  MVP= MV*ngl::ortho(-1, 1, -1, 1, 0.1f, 100.f);
  shader->setShaderParamFromMat4("MVP",MVP);
}
