#include "Button.h"

Button::Button()
{
	m_label.setText(L"Button");
}

Button::~Button()
{
}

void Button::setRect(const D2D1_RECT_F& rect)
{
	m_label.setRect(rect);
	m_image.setRect(rect);
}

void Button::setRect(float left, float top, float right, float bottom)
{
	m_label.setRect(left, top, right, bottom);
	m_image.setRect(left, top, right, bottom);
}

Label& Button::getLabel()
{
	return m_label;
}

Image& Button::getImage()
{
	return m_image;
}

void Button::update(float dt)
{
}

void Button::draw()
{
}
