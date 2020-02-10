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
	std::function<void(std::shared_ptr<Sence> p)> m_senceChangeFunction;

public:
	Sence();
	virtual ~Sence();

	virtual void update(float dt) = NULL;
	virtual void draw() = NULL;

	virtual std::shared_ptr<Camera> getCamera() = NULL;

	virtual bool initResource(ID3D11Device * pDevice) = NULL;				  // 这个函数在init中调用
	virtual bool initEffect(ID3D11Device* pDevice) = NULL;				  // 这个函数在init中调用

	void setSenceChangeFunction(std::function<void(std::shared_ptr<Sence> p)> scf);
	auto getSenceChangeFunction() const;

protected:
};

