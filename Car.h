#pragma once
#include "Character.h"
#include "Geometry.h"
#include "ResourceDepot.h"
#include "Camera_FirstPerson.h"
#include "Camera_ThirdPerson.h"
#include "Wheel.h"

class Car :
	public Character
{
private:
protected:
	Wheel m_wheelLF, m_wheelRF, m_wheelLB, m_wheelRB;
	XMVECTOR m_brakeV;// 刹车前的法线
	GameObject3D m_body;
	float m_power, m_speed, m_brake;
	bool m_isPowerOn, m_isRotOn;
	std::shared_ptr<Camera_ThirdPerson> m_pCameraT;
	std::shared_ptr<Camera_FirstPerson> m_pCameraF;
	float m_wheelRadian;
	int m_cameraType;

public:
	Car();
	virtual ~Car();

	void initCar(ID3D11Device* pDevice);

	void init() override;
	void update(float dt) override;

	void straight(float dt);
	void backup(float dt);
	void left(float dt);
	void right(float dt);
	void brakeOn(float dt);
	void brakeOff(float dt);

	float getSpeed() const;

	float getCameraTRadius() const;
	void setCameraTRadius(float radius);

	void viewRotate(const XMFLOAT3& r);
	void changeCamera();

protected:
private:
};

