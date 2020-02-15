#include "Camera_FirstPerson.h"
#include "XMF_MATH.h"

Camera_FirstPerson::Camera_FirstPerson()
{
}

Camera_FirstPerson::~Camera_FirstPerson()
{
}

void Camera_FirstPerson::input(const XMFLOAT3& ir)
{
	setRotation(ir);
}
