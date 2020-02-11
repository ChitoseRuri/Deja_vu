#pragma once
#include "Label.h"
#include "Image.h"

class Button :
	public GameObject2D
{
private:
	enum class ButtonStatus :char
	{
		normal = 0,
		selected = 1,
		pressed = 1 << 1,
		released = 1 << 2
	};

protected:
	Image m_image;
	Label m_label;

	ButtonStatus m_buttonStatus;

	std::function<void()> m_normalFunction, m_selectFunction, m_pressFunction, m_releaseFunction;

public:
	Button();
	~Button();

	void setRect(const D2D1_RECT_F& rect) override;
	void setRect(float left, float top, float right, float bottom) override;

	Label& getLabel();
	Image& getImage();

	void update(float dt) override;
	void draw() override;

	void normal();
	void setNormalFunction(std::function<void()> pf);				// 设置默认行为
	auto getNormalFunction() const;

	void select();
	void setSelectFunction(std::function<void()> pf);				// 设置被选中时候的行为
	auto getSelectFunction() const;

	void press();
	void setPressFunction(std::function<void()> pf);				// 设置按钮按下的行为
	auto getPressFunction() const;

	void release();
	void setReleaseFunction(std::function<void()> pf);				// 设置按钮松开的行为
	auto getReleaseFunction() const;

	void setDepth(float depth) override;
};

