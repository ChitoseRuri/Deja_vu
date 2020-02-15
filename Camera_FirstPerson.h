#pragma once
#include "Camera.h"

class Camera_FirstPerson :
	public Camera
{
private:
protected:

public:
	Camera_FirstPerson();
	~Camera_FirstPerson();

	void input(const XMFLOAT3& ir) override;
};	