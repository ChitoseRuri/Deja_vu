#pragma once
#include "Mouse.h"
#include "Keyboard.h"
#include <d3d11.h>

class Sence
{
private:
protected:

public:
	Sence();
	virtual ~Sence();

	virtual void update(float dt) = NULL;
	virtual void draw(ID3D11DeviceContext* pDeviceContext) = NULL;

protected:
	virtual bool initResource() = NULL;				  // 这个函数在init中调用
	virtual bool initEffect() = NULL;				  // 这个函数在init中调用
};

