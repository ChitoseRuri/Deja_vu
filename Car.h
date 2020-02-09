#pragma once
#include "Character.h"
#include "Geometry.h"
#include "ResourceDepot.h"

class Car :
	public Character
{
private:
	GameObject3D m_wheelLF, m_wheelRF, m_wheelLB, m_wheelRB, m_body;
	float m_power;

protected:
public:
	Car();
	~Car();

	void init(ID3D11Device* pDevice);
	//void update(float dt) override;

	void straight(float dt);
	void back(float dt);
	void left(float dt);
	void right(float dt);
	void stop(float dt);
};

