#pragma once
#include <DirectXMath.h>

using namespace DirectX;

namespace XMF_MATH
{
	// 向量相加
	XMFLOAT3 operator+(const XMFLOAT3& lhs, const XMFLOAT3& rhs);

	XMFLOAT3 operator-(const XMFLOAT3& lhs, const XMFLOAT3& rhs);
	
	// 向量点乘
	XMFLOAT3 operator*(const XMFLOAT3& lhs, const XMFLOAT3& rhs);

	// 向量相加
	void operator+=(XMFLOAT3& lhs, const XMFLOAT3& rhs);

	// 向量点乘
	void operator*=(XMFLOAT3& lhs, const XMFLOAT3& rhs);

	XMFLOAT3 operator*(const XMFLOAT3& lhs, float f);

	void operator*=(XMFLOAT3& lhs, float f);

	float toRadian(float angle);

	float toAngle(float radian);
}
