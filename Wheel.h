#pragma once
#include "GameObject3D.h"
#include "ResourceDepot.h"
#include "Geometry.h"

class Wheel :
	public GameObject3D
{
private:
protected:
public:
	Wheel();
	virtual ~Wheel();

	void init(ID3D11Device* pDevice);

	void rotWheel(float angle);
	void rollWheel(float distance);
};

