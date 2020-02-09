#include "GameObject3D.h"

std::vector<GameObject3D*> GameObject3D::m_gameObject3DList, GameObject3D::m_updateList, GameObject3D::m_drawList;

using namespace XMF_MATH;

GameObject3D::GameObject3D() :
	m_parent(nullptr),
	m_location(.0f, .0f, .0f),
	m_scale(1.0f, 1.0f, 1.0f),
	m_rotation(.0f, .0f, .0f),
	m_locationP(.0f, .0f, .0f),
	m_scaleP(1.0f, 1.0f, 1.0f),
	m_rotationP(.0f, .0f, .0f),
	m_status(NULL),// 不依靠初始化列表，需要用函数初始化状态
	m_trans(NULL),
	m_drawIndex(-1),
	m_updateIndex(-1),
	m_cbWorld({ XMMatrixIdentity(), XMMatrixIdentity() })
{
	// 把自己的指针放进GameObject统计列表里面
	size_t index = -1;
	size_t count = m_gameObject3DList.size();
	for (size_t i = 0; i < count; ++i)
	{
		if (m_gameObject3DList[i] == nullptr)
		{
			m_gameObject3DList[i] = this;
			index = i;
			break;
		}
	}
	if (index == -1)
	{
		m_gameObject3DList.push_back(this);
		m_gameObject3DIndex = count;
	}

	setUpdateActive(true);
	setVisable(true);
}

GameObject3D::~GameObject3D()
{
	m_gameObject3DList[m_gameObject3DIndex] = nullptr;
	m_gameObject3DIndex = -1;
	if (m_status & Status::visable)
	{
		m_drawList[m_drawIndex] = nullptr;
	}
	if (m_status & Status::updateActive)
	{
		m_updateList[m_updateIndex] = nullptr;
	}
}

void GameObject3D::init(ComPtr<ID3D11DeviceContext> pd3dImmediateContext)
{
	m_pd3dImmediateContext = pd3dImmediateContext;
}

void GameObject3D::updateAll(float dt)
{
	for (auto p : m_updateList)
	{
		if (p && p->getUpdateActive())
		{
			p->update(dt);
		}
	}
}

void GameObject3D::drawAll()
{
	for (auto p : m_drawList)
	{
		if (p)
		{
			p->draw();
		}
	}
}

const XMFLOAT3& GameObject3D::setLocation() const
{
	return m_location;
}

void GameObject3D::setLocation(float x, float y, float z)
{
	m_location = { x, y, z };
	m_trans |= Trans::locate_t;
}

void GameObject3D::setLocation(const XMFLOAT3& location)
{
	m_location = location;
	m_trans |= Trans::locate_t;
}

const XMFLOAT3& GameObject3D::getScale() const
{
	return m_scale;
}

void GameObject3D::setScale(float x, float y, float z)
{
	m_scale = { x, y, z };
	m_trans |= Trans::scale_t;
}

void GameObject3D::setScale(const XMFLOAT3& scale)
{
	m_scale = scale;
	m_trans |= Trans::scale_t;
}

const XMFLOAT3& GameObject3D::getRotation() const
{
	return m_rotation;
}

void GameObject3D::setRotation(float x, float y, float z)
{
	m_rotation = { x, y, z };
	m_trans |= Trans::rotate_t;
}

void GameObject3D::setRotation(const XMFLOAT3& rotation)
{
	m_rotation = rotation;
	m_trans |= Trans::rotate_t;
}

const bool GameObject3D::getVisable() const
{
	return m_status & Status::visable;
}

void GameObject3D::setVisable(bool lb)
{
	if (lb && m_drawIndex == -1)
	{
		m_status |= Status::visable;
		size_t count = m_drawList.size();
		for (size_t i = 0; i < count; ++i)
		{
			if (m_drawList[i] == nullptr)
			{
				m_drawList[i] = this;
				m_drawIndex = i;
				break;
			}
		}
		if (m_drawIndex == -1)
		{
			m_drawIndex = count;
			m_drawList.push_back(this);
		}
	}
	else if(!lb && m_drawIndex != -1)
	{
		m_status &= ~Status::visable;
		m_drawList[m_drawIndex] = nullptr;
		m_drawIndex = -1;
	}
}

const bool GameObject3D::getUpdate() const
{
	return m_status & Status::updateAP;
}

void GameObject3D::setUpdate(bool lb)
{
	if (m_parent == nullptr)
	{
		setUpdateActive(lb);
	}
	else
	{
		setUpdatePassive(lb);
	}
}

void GameObject3D::setTexture(ComPtr<ID3D11ShaderResourceView> texture)
{
	m_pTexture = texture;
}

auto GameObject3D::getTexture() const
{
	return m_pTexture;
}

void GameObject3D::setMeshbuffer(const MeshBuffer& meshBuffer)
{
	m_pVertexBuffer = meshBuffer.vertexBuffer;
	m_pIndexBuffer = meshBuffer.indexBuffer;
	m_indexCount = meshBuffer.count;
	m_vertexStride = meshBuffer.vertexStride;
}

auto GameObject3D::getMeshBuffer() const
{
	return MeshBuffer{ m_pVertexBuffer, m_pIndexBuffer, m_indexCount };
}

void GameObject3D::update(float dt)
{
	updateLocalMatrix();
	for (auto child : m_childen)
	{
		child->update(dt);
	}
}

void GameObject3D::draw()
{
	if (!m_pVertexBuffer || !m_pTexture)
		return;

	// 设置顶点/索引缓冲区
	UINT strides = m_vertexStride;
	UINT offsets = 0;
	m_pd3dImmediateContext->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &strides, &offsets);
	m_pd3dImmediateContext->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

	// 获取之前已经绑定到渲染管线上的常量缓冲区并进行修改
	ComPtr<ID3D11Buffer> cBuffer = nullptr;
	m_pd3dImmediateContext->VSGetConstantBuffers(0, 1, cBuffer.GetAddressOf());

	// 更新常量缓冲区
	D3D11_MAPPED_SUBRESOURCE mappedData;
	HR(m_pd3dImmediateContext->Map(cBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
	memcpy_s(mappedData.pData, sizeof(CBWorld), &m_cbWorld, sizeof(CBWorld));
	m_pd3dImmediateContext->Unmap(cBuffer.Get(), 0);

	// 设置纹理
	m_pd3dImmediateContext->PSSetShaderResources(0, 1, m_pTexture.GetAddressOf());
	// 可以开始绘制
	m_pd3dImmediateContext->DrawIndexed(m_indexCount, 0, 0);
}

void GameObject3D::setParent(GameObject3D* parent)
{
	if (parent == m_parent)
	{
		return;
	}

	if (parent == nullptr)//亲节点为空，自己主动更新
	{
		m_parent = parent;
		setUpdatePassive(false);
		setUpdateActive(true);
		m_scaleP = { 1.0f,1.0f,1.0f };
		m_rotation = { 0.0f,0.0f,0.0f };
		m_locationP = { 0.0f, 0.0f, 0.0f };
		m_trans = UINT32_MAX;
	}
	else
	{
		parent->addChildPassive(this);
		m_parent = parent;
		setUpdateActive(false);// 关掉主动更新，改为亲节点更新
		setUpdatePassive(true);
	}
}

GameObject3D* GameObject3D::getParent() const
{
	return m_parent;
}

void GameObject3D::addChild(GameObject3D* child)
{
	addChildPassive(child);
	child->setUpdateActive(false);
	child->setParentPassive(this);
	child->setUpdatePassive(true);
}

void GameObject3D::delChild(GameObject3D* child)
{
	size_t size = m_childen.size();
	for (size_t index = 0; index < size; ++index)
	{
		if (m_childen[index] == child)
		{
			m_childen.erase(m_childen.begin() + index);
			child->setParent(nullptr);
			child->setUpdateActive(true);
			return;
		}
	}
	assert(true);
}

void GameObject3D::setDebugObjectName(const std::string& name)
{
#if (defined(DEBUG) || defined(_DEBUG)) && (GRAPHICS_DEBUGGER_OBJECT_NAME)
	std::string vbName = name + ".VertexBuffer";
	std::string ibName = name + ".IndexBuffer";
	m_pVertexBuffer->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<UINT>(vbName.length()), vbName.c_str());
	m_pIndexBuffer->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<UINT>(ibName.length()), ibName.c_str());
#else
	UNREFERENCED_PARAMETER(name);
#endif
}

const bool GameObject3D::getUpdateActive() const
{
	return m_status & Status::updateActive;
}

void GameObject3D::setUpdateActive(bool lb)
{
	if (lb && m_updateIndex == -1)
	{
		assert(!(m_status & Status::updatePassive));
		assert(!m_parent);

		m_status |= Status::updateActive;
		size_t count = m_updateList.size();
		for (size_t i = 0; i < count; ++i)
		{
			if (m_updateList[i] == nullptr)
			{
				m_updateList[i] = this;
				m_updateIndex = i;
				break;
			}
		}
		if (m_updateIndex == -1)
		{
			m_updateIndex = count;
			m_updateList.push_back(this);
		}
	}
	else if (!lb && m_updateIndex != -1)
	{
		m_status &= ~Status::updateActive;
		m_updateList[m_updateIndex] = nullptr;
		m_updateIndex = -1;
	}
}

const bool GameObject3D::getUpdatePassive() const
{
	return m_status & Status::updatePassive;
}

void GameObject3D::setUpdatePassive(bool lb)
{
	if (lb)
	{
		assert(!(m_status & Status::updateActive));
		assert(m_parent);

		m_status |= Status::updatePassive;
	}
	else
	{
		m_status &= ~Status::updatePassive;
	}
}

void GameObject3D::updateLocalMatrix()
{
	if (!m_trans)
	{
		return;
	}
	XMFLOAT3 scale = m_scale * m_scaleP;
	XMFLOAT3 rotation = m_rotation + m_rotationP;
	XMFLOAT3 location = m_location + m_locationP;
	XMFLOAT4X4 scaleF4 = {
		scale.x, 0.0f, 0.0f, 0.0f,
		0.0f, scale.y, 0.0f, 0.0f,
		0.0f, 0.0f, scale.z, 0.0f,
		0.0f,0.0f,0.0f,1.0f
	};
	XMMATRIX scaleMatrix = XMLoadFloat4x4(&scaleF4);
	XMMATRIX rotationMatrix = XMMatrixRotationX(toRadian(rotation.x)) * XMMatrixRotationY(toRadian(rotation.y)) * XMMatrixRotationZ(toRadian(rotation.z));
	XMFLOAT4X4 locationF4 = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		location.x, location.y, location.z, 1.0f
	};
	XMMATRIX locationMatrix = XMLoadFloat4x4(&locationF4);
	XMMATRIX worldMatrix = scaleMatrix * rotationMatrix * locationMatrix;
	// 内部进行转置，这样外部就不需要提前转置了
	m_cbWorld.world = XMMatrixTranspose(worldMatrix);
	m_cbWorld.worldInvTranspose = XMMatrixInverse(nullptr, worldMatrix);	// 两次转置抵消
	// 根据更新总量更新子节点
	if (m_trans & Trans::scale_t)
	{
		for (auto child : m_childen)
		{
			child->setScaleP(scale);
		}
	}
	if (m_trans & Trans::rotate_t)
	{
		for (auto child : m_childen)
		{
			child->setRotationP(rotation);
		}
	}
	if (m_trans & Trans::locate_t)
	{
		for (auto child : m_childen)
		{
			child->setLocationP(location);
		}
	}
	m_trans = NULL;// 重置修改判断符
}

void GameObject3D::setLocationP(const XMFLOAT3& location)
{
	m_locationP = location;
	m_trans |= Trans::locate_t;
}

void GameObject3D::setScaleP(const XMFLOAT3& scale)
{
	m_scaleP = scale;
	m_trans |= Trans::scale_t;
}

void GameObject3D::setRotationP(const XMFLOAT3& rotation)
{
	m_rotationP = rotation;
	m_trans |= rotate_t;
}

void GameObject3D::setParentPassive(GameObject3D* parent)
{
	m_parent = parent;
}

void GameObject3D::addChildPassive(GameObject3D* child)
{
	if (child->getParent() == this)
	{
		return;
	}

	m_childen.push_back(child);
	child->setScaleP(m_scale * m_scaleP);
	child->setRotationP(m_rotation * m_rotationP);
	child->setLocationP(m_location + m_locationP);
}

