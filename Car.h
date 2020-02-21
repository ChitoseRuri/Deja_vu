#pragma once
#include "Character.h"
#include "Geometry.h"
#include "ResourceDepot.h"
#include "Camera_FirstPerson.h"
#include "Camera_ThirdPerson.h"
#include "Wheel.h"

class Car :
	public Character
{
private:
protected:
	Wheel m_wheelLF, m_wheelRF, m_wheelLB, m_wheelRB;
	XMVECTOR m_brakeV;// 刹车前的法线
	GameObject3D m_body;
	float m_power, m_speed, m_brake;
	bool m_isPowerOn, m_isRotOn;
	std::shared_ptr<Camera_ThirdPerson> m_pCameraT;
	std::shared_ptr<Camera_FirstPerson> m_pCameraF;
	float m_wheelRadian;
	int m_cameraType;

public:
	Car();
	virtual ~Car();

	void initCar(ID3D11Device* pDevice);

	void init() override;
	void update(float dt) override;

	void straight(float dt);
	void backup(float dt);
	void left(float dt);
	void right(float dt);
	void brakeOn(float dt);
	void brakeOff(float dt);

	float getSpeed() const;

	float getCameraTRadius() const;
	void setCameraTRadius(float radius);

	void viewRotate(const XMFLOAT3& r);
	void changeCamera();

protected:
	template<class VertexType = VertexPosNormalTex, class IndexType = WORD>
	inline Geometry::MeshData<VertexType, IndexType> createCarBody();
private:
};

template<class VertexType, class IndexType>
inline Geometry::MeshData<VertexType, IndexType> Car::createCarBody()
{
	Geometry::MeshData<VertexType, IndexType> meshData = Geometry::CreateBox();

	// 右面(+X面)
	meshData.vertexVec[1].pos.z *= 0.5f;
	meshData.vertexVec[2].pos.z *= 0.5f;
	// 左面(-X面)
	meshData.vertexVec[5].pos.z *= 0.5f;
	meshData.vertexVec[6].pos.z *= 0.5f;
	// 顶面(+Y面)
	meshData.vertexVec[8].pos.z *= 0.5f;
	meshData.vertexVec[9].pos.z *= 0.5f;
	meshData.vertexVec[10].pos.z *= 0.5f;
	meshData.vertexVec[11].pos.z *= 0.5f;
	// 背面(+Z面)
	meshData.vertexVec[17].pos.z *= 0.5f;
	meshData.vertexVec[18].pos.z *= 0.5f;
	// 正面(-Z面)
	meshData.vertexVec[21].pos.z *= 0.5f;
	meshData.vertexVec[22].pos.z *= 0.5f;

	for (UINT i = 0; i < 4; ++i)
	{
		// 背面(+Z面)
		meshData.vertexVec[i + 16].normal = XMFLOAT3(0.0f, 1.0f, 1.0f);
		// 正面(-Z面)
		meshData.vertexVec[i + 20].normal = XMFLOAT3(0.0f, 1.0f, -1.0f);
	}

	return meshData;
}
