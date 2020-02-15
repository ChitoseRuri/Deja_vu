#pragma once
#include "Character.h"
#include "Geometry.h"
#include "ResourceDepot.h"
#include "Camera_FirstPerson.h"
#include "Camera_ThirdPerson.h"

class Car :
	public Character
{
private:
	GameObject3D m_wheelLF, m_wheelRF, m_wheelLB, m_wheelRB, m_body;
	float m_power;
	std::shared_ptr<Camera_ThirdPerson> m_pCameraT;
	std::shared_ptr<Camera_FirstPerson> m_pCameraF;

protected:
public:
	Car();
	~Car();

	void initCar(ID3D11Device* pDevice);

	void init() override;
	//void update(float dt) override;

	void straight(float dt);
	void back(float dt);
	void left(float dt);
	void right(float dt);
	void stop(float dt);

	void viewRotate(const XMFLOAT3& r);
};

