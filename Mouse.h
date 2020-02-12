#pragma once

#include <windows.h>
#include <memory>
#include "d3dUtil.h"

class Mouse
{
private:
	HWND m_hwnd;
	DirectX::XMINT2 m_pos;
	std::vector<bool> m_status[2];
	int m_indexNow, m_indexLast;

protected:
public:
	enum class Button
	{
		LeftButton = 0,
		RightButton,
		ButonEnd// 用来计算长度的值
	};

	Mouse();
	~Mouse();

	void afterResize();
	void afterResize(HWND hwnd);

	void setMousePos(WPARAM lParam);
	DirectX::XMINT2 getMousePos() const;

	void buttonUp(Button button);
	void buttonDown(Button button);

	bool isButonUp(Button button) const;
	bool isButtonDown(Button button) const;
	bool isButtonRelease(Button button) const;
	bool isButtonPress(Button button) const;

	void update();
};