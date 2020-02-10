#pragma once
#include "Sence.h"
#include "Sence_DriverTest.h"
#include "Car.h"

class Sence_MainMenu :
	public Sence
{
private:
	Label m_title;
	Button m_buttonChangeSence;

	std::shared_ptr<Camera> m_pCamera;
	

protected:
public:
	Sence_MainMenu();
	~Sence_MainMenu();

	bool initResource(ID3D11Device* pDevice) override;
	bool initEffect(ID3D11Device* pDevice) override;

	void update(float dt) override;
	void draw() override;

	void beforeResize();
	void afterResize();

	std::shared_ptr<Camera> getCamera() override;
};

