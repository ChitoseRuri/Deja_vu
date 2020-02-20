#pragma once
#include "Camera.h"

class Camera_ThirdPerson :
	public Camera
{
private:
protected:
	float m_radius;
	float m_phi, m_theta;

public:
	Camera_ThirdPerson();
	virtual ~Camera_ThirdPerson();

	void setRadius(float radius);
	float getRadius() const;

	void input(const XMFLOAT3& ir) override;

protected:
	void fixView();
};

