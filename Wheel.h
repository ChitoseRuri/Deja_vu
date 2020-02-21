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

protected:
	template<class VertexType = VertexPosNormalTex, class IndexType = WORD>
	inline Geometry::MeshData<VertexType, IndexType> createWheel(float radius = 1.0f, float height = 2.0f, UINT slices = 20,
		const DirectX::XMFLOAT4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
};

template<class VertexType, class IndexType>
inline Geometry::MeshData<VertexType, IndexType> Wheel::createWheel(float radius, float height, UINT slices, const DirectX::XMFLOAT4& color)
{
	Geometry::MeshData<VertexPosNormalTex, WORD> meshData = Geometry::CreateCylinder(radius, height, slices, color);
	UINT vertexCount = 2 * (slices + 1);
	UINT vertexEnd = 4 * (slices + 1) + 2;
	// 调整贴图位置
	for (UINT i = 0; i < vertexCount; ++i)
	{
		meshData.vertexVec[i].tex.x *= 0.79f;
	}

	for (UINT i = vertexCount; i < vertexEnd; ++i)
	{
		meshData.vertexVec[i].tex.x *= 0.2f;
		meshData.vertexVec[i].tex.x += 0.8f;
	}

	return meshData;
}
