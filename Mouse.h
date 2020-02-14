#pragma once

#include <windows.h>
#include <memory>
#include "d3dUtil.h"

class Mouse
{
private:
	HWND m_hwnd;
	POINT m_posNow, m_posLast;
	POINT m_posLock;
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
	POINT getMousePos() const;

	void buttonUp(Button button);
	void buttonDown(Button button);

	bool isButonUp(Button button) const;
	bool isButtonDown(Button button) const;
	bool isButtonRelease(Button button) const;
	bool isButtonPress(Button button) const;

	void update();

	void lock();
	void lock(const POINT& pos);
	void unlock();
	bool isLock() const;

	POINT getRelativeMovement() const;// 获取相对上一帧的移动
};