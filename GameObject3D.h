#pragma once
#include "GameObject.h"
#include "DXTrace.h"
#include "XMF_MATH.h"
#include <vector>
#include <assert.h>

using namespace DirectX;

template <class T>
using ComPtr = Microsoft::WRL::ComPtr<T>;

struct MeshBuffer
{
	ComPtr<ID3D11Buffer> vertexBuffer;
	ComPtr<ID3D11Buffer> indexBuffer;
	size_t count;
	size_t vertexStride;
};

class GameObject3D:
	public GameObject
{
private:
	static std::vector<GameObject3D*> m_gameObject3DList, m_updateList, m_drawList;
	size_t m_gameObject3DIndex, m_updateIndex, m_drawIndex;

protected:
	enum Trans
	{
		scale_t,
		rotate_t,
		locate_t
	};
	// P后缀的为亲节点变化的集合

	XMFLOAT3 m_localPositon;
	XMFLOAT3 m_localScale;
	XMFLOAT3 m_localRotation;
	XMMATRIX m_worldMatrix, m_localMatrix;

	CBWorld m_cbWorld;												// 计算后的矩阵
	ComPtr<ID3D11ShaderResourceView> m_pTexture;
	ComPtr<ID3D11Buffer> m_pVertexBuffer;
	ComPtr<ID3D11Buffer> m_pIndexBuffer;
	size_t m_indexCount, m_vertexStride;

	UINT32 m_status;												// 标记，记录GameObject的状态@enum Status
	UINT32 m_trans;													// 标记，记录做了什么空间变换@enum Trans
	std::vector<GameObject3D*> m_childen;
	GameObject3D* m_parent;

public:
	GameObject3D();
	virtual ~GameObject3D();

	static void init(ComPtr<ID3D11DeviceContext> pd3dImmediateContext); // 系统初始化
	static void updateAll(float dt);									// 更新全部
	static void drawAll();			// 绘制全部

	const XMFLOAT3& setPosition() const;
	virtual void setPosition(float x, float y, float z);
	virtual void setPosition(const XMFLOAT3& location);

	const XMFLOAT3& getScale() const;
	virtual void setScale(float x, float y, float z);
	virtual void setScale(const XMFLOAT3& scale);

	const XMFLOAT3& getRotation() const;
	virtual void setRotation(float x, float y, float z);						// 角度
	virtual void setRotation(const XMFLOAT3& rotation);							// 角度

	const bool getVisable() const;
	void setVisable(bool lb);

	const bool getUpdate() const;										// 判断是否为 主动|被动 更新
	void setUpdate(bool lb);

	void setTexture(ComPtr<ID3D11ShaderResourceView> texture);
	auto getTexture() const;

	void setMeshbuffer(const MeshBuffer& meshBuffer);
	auto getMeshBuffer() const;

	virtual void update(float dt);// 更新，亲节点负责更新子节点
	virtual void draw();

	void setParent(GameObject3D* parent);
	GameObject3D* getParent()const;
	void addChild(GameObject3D* child);
	void delChild(GameObject3D* child);

	const XMMATRIX& getWorldMatrix() const;
	// 设置调试对象名
// 若缓冲区被重新设置，调试对象名也需要被重新设置
	void setDebugObjectName(const std::string& name);

protected:
	const bool getUpdateActive() const;						// 判断是否为主动更新
	void setUpdateActive(bool lb);

	const bool getUpdatePassive() const;					// 判断是否为被动更新
	void setUpdatePassive(bool lb);

	virtual void updateLocalMatrix();								// 自己的变换矩阵更变的时候调用
	virtual void updateWorldMatrix();								// 亲节点的变化矩阵更变的时候调用

	void setParentPassive(GameObject3D* parent);
	void addChildPassive(GameObject3D* child);
};
