#include "UIButton.h"
#include "ngl/VAOPrimitives.h"
#include "ngl/ShaderLib.h"
#include "ngl/Transformation.h"

UIButton::UIButton()
{
  m_hasText = false;
}

UIButton::UIButton(std::string _font, int _fontSize)
{
  m_text = new Text(_font, _fontSize);
  m_hasText = true;
}

UIButton::~UIButton()
{
  delete m_text;
}

void UIButton::addText(std::string _font, int _fontSize)
{
  if (m_hasText = false)
  {
    m_text = new Text(_font, _fontSize);
    m_hasText = true;
  }
  else
  {
    std::cout<<"Error: This button already has text."<<std::endl;
  }
}

void UIButton::updateButton(ngl::Vec2 _pos, ngl::Vec2 _dimensions, ngl::Vec4 _colour)
{
  m_buttonPos = _pos;
  m_buttonDimensions = _dimensions;
  m_buttonColour = _colour;
}

void UIButton::updateText(std::string _text, ngl::Vec3 _colour, ngl::Vec2 _offset)
{
  if (m_hasText == true)
  {
    m_textString = _text;
    m_text->setColour(_colour.m_x, _colour.m_y, _colour.m_z);
    m_textOffset = _offset;

    m_textPos.m_x = ((m_buttonPos.m_x + 1) * m_screenDimensions.m_x) / 2;
    m_textPos.m_y = ((1 - m_buttonPos.m_y) * m_screenDimensions.m_y) / 2;
  }
  else
  {
    std::cout<<"Error: Text has not been initialised for this button."<<std::endl;
  }
}

bool UIButton::isClicked(int _x, int _y)
{
  ngl::Vec2 ndc;
  ndc.m_x = ((2.f * _x) / m_screenDimensions.m_x) - 1.f;
  ndc.m_y = 1.f - ((2.f * _y) / m_screenDimensions.m_y);

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

void UIButton::executeClick()
{
  m_buttonColour = ngl::Vec4(0.f, 0.f, 1.f, 1.f);
}

void UIButton::draw()
{
  loadMatricesToShader();
  ngl::VAOPrimitives::instance()->createTrianglePlane("button", m_buttonDimensions.m_x, m_buttonDimensions.m_y, 1, 1, ngl::Vec3(0, 1, 1));
  ngl::VAOPrimitives::instance()->draw("button");
  m_text->renderText(m_textPos.m_x + m_textOffset.m_x, m_textPos.m_y + m_textOffset.m_y, m_textString);
}

void UIButton::loadMatricesToShader()
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  (*shader)["nglColourShader"]->use();
  shader->setShaderParam4f("Colour", m_buttonColour.m_x, m_buttonColour.m_y,m_buttonColour.m_z, 1.f);

  ngl::Transformation t;
  t.setPosition(ngl::Vec3(m_buttonPos.m_x, m_buttonPos.m_y, -10.f));
  t.setRotation(90.0,0.0,0.0);

  ngl::Mat4 M;
  ngl::Mat4 MV;
  ngl::Mat4 MVP;

  M=t.getMatrix();
  MV=  M*ngl::lookAt(ngl::Vec3(0, 0, 10), ngl::Vec3(0, 0, 0), ngl::Vec3(0, 1, 0));
  MVP= MV*ngl::ortho(-1, 1, -1, 1, 0.1f, 100.f);
  shader->setShaderParamFromMat4("MVP",MVP);
}
