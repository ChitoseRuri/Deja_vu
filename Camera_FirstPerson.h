#pragma once
#include "Camera.h"

class Camera_FirstPerson :
	public Camera
{
private:
protected:
	XMFLOAT3 m_targetL;

public:
	Camera_FirstPerson();
	~Camera_FirstPerson();

	void setPosition(float x, float y, float z) override;
	void setPosition(const XMFLOAT3& location) override;

	void setScale(float x, float y, float z) override;
	void setScale(const XMFLOAT3& scale) override;

	void setRotation(float x, float y, float z) override;
	void setRotation(const XMFLOAT3& rotation) override;
};	