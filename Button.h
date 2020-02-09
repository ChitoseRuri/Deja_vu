#pragma once
#include "Label.h"
#include "Image.h"

class Button :
	public GameObject2D
{
private:
protected:
	Label m_label;
	Image m_image;

public:
	Button();
	~Button();

	void setRect(const D2D1_RECT_F& rect) override;
	void setRect(float left, float top, float right, float bottom) override;

	Label& getLabel();
	Image& getImage();

	void update(float dt) override;
	void draw() override;
};

