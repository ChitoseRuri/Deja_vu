#pragma once
#include "Sence.h"
#include"Car.h"

class Sence_DriverTest :
	public Sence 
{
private:
	Car m_car;
	GameObject3D m_land, m_box;

	// UI
	Label m_labelSpeed;

protected:
public:
	Sence_DriverTest();
	~Sence_DriverTest();

	bool initResource(ID3D11Device* pDevice) override;
	bool initEffect(ID3D11Device* pDevice) override;

	void update(float dt) override;
	void draw() override;

	void beforeResize();
	void afterResize();

	void input(Keyboard& keyboard, Mouse& mouse, float dt) override;

	std::shared_ptr<Camera> getCamera() override;
};

