#pragma once
#include "Camera.h"

class Character :
	public GameObject3D
{
private:
protected:
	XMFLOAT3 m_speedVector;							// 速度矢量
	XMFLOAT3 m_acceleration;						// 加速度
	XMFLOAT3 m_resistance;							// 阻力加速度
	std::shared_ptr<Camera> m_pCamera;

public:
	Character();
	~Character();

	virtual void init();

	std::shared_ptr<Camera> getCamera() const;

	void setSpeedVector(const XMFLOAT3& speed);
	void setSpeedVector(float x, float y, float z);
	const XMFLOAT3& getSpeedVector() const;

	void setAcceleration(const XMFLOAT3& acceleration);
	void setAcceleration(float x, float y, float z);
	const XMFLOAT3& getAcceleration() const;

	void setResistance(const XMFLOAT3& acceleration);
	void setResistance(float x, float y, float z);
	const XMFLOAT3& getResistance() const;

	void update(float dt) override;

protected:
	void updateSpeed(float dt);
};

