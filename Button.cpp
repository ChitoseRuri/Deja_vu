#include "Button.h"

Button::Button():
	m_buttonStatus(ButtonStatus::normal),
	m_normalFunction(nullptr),
	m_selectFunction(nullptr),
	m_pressFunction(nullptr),
	m_releaseFunction(nullptr)
{
	m_label.setText(L"Button");
	setDepth(0.01f);
}

Button::~Button()
{
}

void Button::setRect(const D2D1_RECT_F& rect)
{
	constexpr float w = 8.0f, h = 4.0f; // 测试用数据，待调整
	assert(rect.right - rect.left > 2.0 * w);
	assert(rect.bottom - rect.top > 2.0 * h);

	m_rect = rect;
	m_label.setRect(rect.left + w, rect.top + h, rect.right - w, rect.bottom - h);
	m_image.setRect(rect);
}

void Button::setRect(float left, float top, float right, float bottom)
{
	// 让文字的区域稍微比图片小
	constexpr float w = 8.0f, h = 4.0f; // 测试用数据，待调整
	assert(right - left > 2.0 * w);
	assert(bottom - top > 2.0 * h);

	m_rect = { left, top, right, bottom };
	m_label.setRect(left + w, top + h, right - w, bottom - h);
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

void Button::normal()
{
	// nomal是一个持续的状态
	m_buttonStatus = ButtonStatus::normal;
	if(m_normalFunction)
		m_normalFunction();
}

void Button::setNormalFunction(std::function<void()> pf)
{
	m_normalFunction = pf;
}

auto Button::getNormalFunction() const
{
	return m_normalFunction;
}

void Button::select()
{
	// select是一个持续的状态
	m_buttonStatus = ButtonStatus::selected;
	if(m_selectFunction)
		m_selectFunction();
}

void Button::setSelectFunction(std::function<void()> pf)
{
	m_selectFunction = pf;
}

auto Button::getSelectFunction() const
{
	return m_selectFunction;
}

void Button::press()
{
	// press可以是瞬间，也可以是持续的状态
	m_buttonStatus = ButtonStatus::pressed;
	if(m_pressFunction)
		m_pressFunction();
}

void Button::setPressFunction(std::function<void()> pf)
{
	m_pressFunction = pf;
}

auto Button::getPressFunction() const
{
	return m_pressFunction;
}

void Button::release()
{
	// release是一个瞬间的状态
	m_buttonStatus = ButtonStatus::released;
	if(m_releaseFunction)
		m_releaseFunction();
}

void Button::setReleaseFunction(std::function<void()> pf)
{
	m_releaseFunction = pf;
}

auto Button::getReleaseFunction() const
{
	return m_releaseFunction;
}

void Button::setDepth(float depth)
{
	m_image.setDepth(depth);
	m_label.setDepth(depth - 0.0001f);
}
