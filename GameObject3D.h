#pragma once
#include "d3dApp.h"
#include "d3dUtil.h"
#include "DXTrace.h"
#include "XMF_MATH.h"
#include <vector>
#include <assert.h>

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

class GameObject3D
{
private:
	static std::vector<GameObject3D*> m_gameObjectList, m_updateList, m_drawList;
	size_t m_gameObjectIndex, m_updateIndex, m_drawIndex;

protected:
	enum Status
	{
		visable = 1,
		updateActive = 1 << 1,
		updatePassive = 1 << 2,
		updateAP = updateActive | updatePassive,
		statical = 1 << 3
	};

	enum Trans
	{
		scale_t,
		rotate_t,
		locate_t
	};
	// P后缀的为亲节点变化的集合

	XMFLOAT3 m_location, m_locationP;
	XMFLOAT3 m_scale, m_scaleP;
	XMFLOAT3 m_rotation, m_rotationP;

	CBWorld m_cbWorld;												// 计算后的矩阵
	ComPtr<ID3D11ShaderResourceView> m_texture;
	ComPtr<ID3D11Buffer> m_vertexBuffer;
	ComPtr<ID3D11Buffer> m_indexBuffer;
	size_t m_indexCount, m_vertexStride;

	UINT32 m_status;												// 标记，记录GameObject的状态@enum Status
	UINT32 m_trans;													// 标记，记录做了什么空间变换@enum Trans
	std::vector<GameObject3D*> m_childen;
	GameObject3D* m_parent;

public:
	GameObject3D();
	virtual ~GameObject3D();

	static void updateAll(float dt);									// 更新全部
	static void drawAll(ID3D11DeviceContext* pDeviceContext);			// 绘制全部

	const XMFLOAT3& getLocation() const;
	void setLocation(float x, float y, float z);
	void setLocation(const XMFLOAT3& location);

	const XMFLOAT3& getScale() const;
	void setScale(float x, float y, float z);
	void setScale(const XMFLOAT3& scale);

	const XMFLOAT3& getRotation() const;
	void setRotation(float x, float y, float z);						// 角度
	void setRotation(const XMFLOAT3& rotation);							// 角度

	const bool getVisable() const;
	void setVisable(bool lb);

	const bool getUpdate() const;										// 判断是否为 主动|被动 更新
	void setUpdate(bool lb);

	void setTexture(ComPtr<ID3D11ShaderResourceView> texture);
	auto getTexture() const;

	void setMeshbuffer(const MeshBuffer& meshBuffer);
	auto getMeshBuffer() const;

	virtual void update(float dt);// 更新，亲节点负责更新子节点
	virtual void draw(ID3D11DeviceContext* pDeviceContext);

	void setParent(GameObject3D* parent);
	GameObject3D* getParent()const;
	void addChild(GameObject3D* child);
	void delChild(GameObject3D* child);
	// 设置调试对象名
// 若缓冲区被重新设置，调试对象名也需要被重新设置
	void setDebugObjectName(const std::string& name);

protected:
	const bool getUpdateActive() const;						// 判断是否为主动更新
	void setUpdateActive(bool lb);

	const bool getUpdatePassive() const;					// 判断是否为被动更新
	void setUpdatePassive(bool lb);

	void updateLocalMatrix();
	void setLocationP(const XMFLOAT3& location);
	void setScaleP(const XMFLOAT3& scale);
	void setRotationP(const XMFLOAT3& rotation);

	void setParentPassive(GameObject3D* parent);
	void addChildPassive(GameObject3D* child);
};
