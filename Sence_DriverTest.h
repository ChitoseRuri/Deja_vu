#pragma once
#include "Sence.h"

class Sence_DriverTest :
	public Sence 
{
private:
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

	std::shared_ptr<Camera> getCamera() override;
};
