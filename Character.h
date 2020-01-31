#pragma once
#include "Camera.h"

class Character :
	public GameObject
{
private:
protected:
	XMFLOAT3 m_speed, m_acceleration;
	std::vector<XMFLOAT3> m_eyePos;					// 视点集
	UINT32 m_eyeIndex;								// 选用的视点
	Camera m_camera;

public:
	Character();
	~Character();

	XMMATRIX getViewXM() const;
	XMVECTOR getEyePos() const;

	void setSpeed(const XMFLOAT3& speed);
	void setSpeed(float x, float y, float z);
	const XMFLOAT3& getSpeed();

	void setAcceleration(const XMFLOAT3& acceleration);
	void setAcceleration(float x, float y, float z);
	const XMFLOAT3& getAcceleration();
};

