#include "XMF_MATH.h"


XMFLOAT3 XMF_MATH::operator+(const XMFLOAT3& lhs, const XMFLOAT3& rhs)
{
	return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z };
}

XMFLOAT3 XMF_MATH::operator*(const XMFLOAT3& lhs, const XMFLOAT3& rhs)
{
	return { lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z };
}

void XMF_MATH::operator+=(XMFLOAT3& lhs, const XMFLOAT3& rhs)
{
	lhs.x += rhs.x, lhs.y += rhs.y, lhs.z += rhs.z;
}

void XMF_MATH::operator*=(XMFLOAT3& lhs, const XMFLOAT3& rhs)
{
	lhs.x *= rhs.x, lhs.y *= rhs.y, lhs.z *= rhs.z;
}

XMFLOAT3 XMF_MATH::operator*(const XMFLOAT3& lhs, float f)
{
	return { lhs.x * f, lhs.y * f, lhs.z * f };
}

void XMF_MATH::operator*=(XMFLOAT3& lhs, float f)
{
	lhs.x *= f, lhs.y *= f, lhs.z *= f;
}