#pragma once
#include "d3dApp.h"
#include <vector>

using namespace DirectX;

template <class T>
using ComPtr = Microsoft::WRL::ComPtr<T>;

struct MeshBuffer
{
	ComPtr<ID3D11Buffer> vertexBuffer;
	ComPtr<ID3D11Buffer> indexBuffer;
	size_t count;
};

class GameObjectN
{
private:
	static std::vector<GameObjectN*> m_gameObjectList, m_updateList, m_drawList;
	size_t m_gameObjectIndex, m_updateIndex, m_drawIndex;

protected:
	enum Status
	{
		visable = 1,
		updateActive = 1 << 1,
		updatePassive = 1 << 2,
		statical = 1 << 3
	};



	XMFLOAT3 m_location;
	XMFLOAT3 m_scale;
	XMFLOAT3 m_rotation;

	XMFLOAT4X4 m_localMatrix;
	ComPtr<ID3D11ShaderResourceView> m_texture;
	ComPtr<ID3D11Buffer> m_vertexBuffer;
	ComPtr<ID3D11Buffer> m_indexBuffer;
	size_t m_indexCount;

	UINT32 m_status;
	std::vector<GameObjectN*> m_childen;
	GameObjectN* m_parent;

public:
	GameObjectN();
	virtual ~GameObjectN();

	static void updateAll(float dt);									// 更新全部
	static void drawAll(ID3D11DeviceContext* pDeviceContext);			// 绘制全部

	const XMFLOAT3& getLocation() const;
	virtual void setLocation(float x, float y, float z);
	virtual void setLocation(const XMFLOAT3& location);

	const XMFLOAT3& getScale() const;
	virtual void setScale(float x, float y, float z);
	virtual void setScale(const XMFLOAT3& scale);

	const XMFLOAT3& getRotation() const;
	virtual void setRotation(float x, float y, float z);
	virtual void setRotation(const XMFLOAT3& rotation);

	const bool getVisable() const;
	void setVisable(bool lb);

	const bool getActiveUpdate() const;
	void setActiveUpdate(bool lb);

	virtual void update(float dt);
	virtual void draw(ID3D11DeviceContext* pDeviceContext);

};

