#pragma once
#include "d3dApp.h"
#include "d3dUtil.h"
#include"DXTrace.h"
#include <vector>

using namespace DirectX;

template <class T>
using ComPtr = Microsoft::WRL::ComPtr<T>;

struct CBWorld
{
	XMMATRIX world;
	XMMATRIX worldInvTranspose;
};

struct MeshBuffer
{
	ComPtr<ID3D11Buffer> vertexBuffer;
	ComPtr<ID3D11Buffer> indexBuffer;
	size_t count;
	size_t vertexStride;
};

class GameObject
{
private:
	static std::vector<GameObject*> m_gameObjectList, m_updateList, m_drawList;
	size_t m_gameObjectIndex, m_updateIndex, m_drawIndex;

protected:
	enum Status
	{
		visable = 1,
		updateActive = 1 << 1,
		updatePassive = 1 << 2,
		statical = 1 << 3
	};

	// P后缀的为亲节点变化的集合

	XMFLOAT3 m_location, m_locationP;
	XMFLOAT3 m_scale, m_scaleP;
	XMFLOAT3 m_rotation, m_rotationP;

	XMMATRIX m_localMatrix;
	ComPtr<ID3D11ShaderResourceView> m_texture;
	ComPtr<ID3D11Buffer> m_vertexBuffer;
	ComPtr<ID3D11Buffer> m_indexBuffer;
	size_t m_indexCount, m_vertexStride;

	UINT32 m_status;
	bool m_isMoved;
	std::vector<GameObject*> m_childen;
	GameObject* m_parent;

public:
	GameObject();
	virtual ~GameObject();

	static void updateAll(float dt);									// 更新全部
	static void drawAll(ID3D11DeviceContext* pDeviceContext);			// 绘制全部

	const XMFLOAT3& getLocation() const;
	void setLocation(float x, float y, float z);
	void setLocation(const XMFLOAT3& location);

	const XMFLOAT3& getScale() const;
	void setScale(float x, float y, float z);
	void setScale(const XMFLOAT3& scale);

	const XMFLOAT3& getRotation() const;
	void setRotation(float x, float y, float z);
	void setRotation(const XMFLOAT3& rotation);

	const bool getVisable() const;
	void setVisable(bool lb);

	const bool getActiveUpdate() const;
	void setActiveUpdate(bool lb);

	void setTexture(ComPtr<ID3D11ShaderResourceView> texture);
	auto getTexture() const;

	void setMeshbuffer(const MeshBuffer& meshBuffer);
	auto getMeshBuffer() const;

	virtual void update(float dt);
	virtual void draw(ID3D11DeviceContext* pDeviceContext);

	// 设置调试对象名
// 若缓冲区被重新设置，调试对象名也需要被重新设置
	void setDebugObjectName(const std::string& name);

private:
	void updateLocalMatrix();
	void setLocationP(const XMFLOAT3& location);
	void setScaleP(const XMFLOAT3& scale);
	void setRotationP(const XMFLOAT3& rotation);
};

