#pragma once
#include "Mouse.h"
#include "Keyboard.h"
#include <d3d11.h>

class Sence
{
private:
protected:
	std::shared_ptr<DirectX::Mouse> m_pMouse;					// 鼠标
	DirectX::Mouse::ButtonStateTracker m_MouseTracker;			// 鼠标状态追踪器
	std::shared_ptr<DirectX::Keyboard> m_pKeyboard;				// 键盘
	DirectX::Keyboard::KeyboardStateTracker m_KeyboardTracker;	// 键盘状态追踪器

public:
	Sence();
	virtual ~Sence();

	void init(std::shared_ptr<DirectX::Mouse> mouse, std::shared_ptr<DirectX::Keyboard> keyBoard);
	virtual void update(float dt) = NULL;
	virtual void draw(ID3D11DeviceContext* pDeviceContext) = NULL;

protected:
	virtual bool initResource() = NULL;				  // 这个函数在init中调用
	virtual bool initEffect() = NULL;				  // 这个函数在init中调用
};

