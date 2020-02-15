#pragma once
#include "Mouse.h"
#include "Keyboard.h"
#include "Canvas.h"
#include <d3d11.h>
#include "Camera.h"

class Sence
{
private:
protected:
	static std::function<void(std::shared_ptr<Sence> p)> switchSence;// 切换scene时候调用的方法

public:
	Sence();
	virtual ~Sence();

	virtual void update(float dt) = NULL;
	virtual void draw() = NULL;

	virtual std::shared_ptr<Camera> getCamera() = NULL;

	virtual bool initResource(ID3D11Device * pDevice) = NULL;				  // 这个函数在init中调用
	virtual bool initEffect(ID3D11Device* pDevice) = NULL;					  // 这个函数在init中调用

	static void setSwitchSenceFunction(std::function<void(std::shared_ptr<Sence> p)> scf);
	static auto getSwitchSenceFunction();

	virtual void input(Keyboard& keyboard, Mouse& mouse, float dt);

protected:
};