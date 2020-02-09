#include "Image.h"

using namespace XMF_MATH;

ComPtr<ID3D11Buffer> Image::m_pVertexBuffer = nullptr;
ComPtr<ID3D11Buffer> Image::m_pIndexBuffer = nullptr;

Image::Image():
	m_pTexture(nullptr)
{
}

Image::~Image()
{
}

void Image::init(ID3D11Device* pDevice)
{
	auto meshData = Geometry::Create2DShow();

	// 设置顶点缓冲区描述
	size_t vertexStride = sizeof(VertexPosTex);
	D3D11_BUFFER_DESC vbd;
	ZeroMemory(&vbd, sizeof(vbd));
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = (UINT)meshData.vertexVec.size() * vertexStride;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	// 新建顶点缓冲区
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = meshData.vertexVec.data();
	HR(pDevice->CreateBuffer(&vbd, &InitData, m_pVertexBuffer.GetAddressOf()));


	// 设置索引缓冲区描述
	size_t indexCount = (UINT)meshData.indexVec.size();
	D3D11_BUFFER_DESC ibd;
	ZeroMemory(&ibd, sizeof(ibd));
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = indexCount * sizeof(WORD);
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	// 新建索引缓冲区
	InitData.pSysMem = meshData.indexVec.data();
	HR(pDevice->CreateBuffer(&ibd, &InitData, m_pIndexBuffer.GetAddressOf()));
}

void Image::setTexture(ComPtr<ID3D11ShaderResourceView> texture)
{
	m_pTexture = texture;
}

auto Image::getTexture() const
{
	return m_pTexture;
}

void Image::setRotation(float rotation)
{
	m_rotation = (rotation > 0) ? (rotation > 360.0f ? rotation - 360.0f : rotation) : (rotation < -360.0f ? rotation + 360.0f : rotation);
	updateLocalMatrix();
}


float Image::getRoation() const
{
	return m_rotation;
}

void Image::setRect(const D2D1_RECT_F& rect)
{
	m_rect = rect;
	updateLocalMatrix();
}

void Image::setRect(float left, float top, float right, float bottom)
{
	m_rect = { left, top, right, bottom };
	updateLocalMatrix();
}

void Image::update(float dt)
{
}

void Image::draw()
{
	// 设置顶点/索引缓冲区
	UINT strides = sizeof(VertexPosTex);
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
	m_pd3dImmediateContext->DrawIndexed(6, 0, 0);
}

void Image::updateLocalMatrix()
{
	XMFLOAT3 scale = { (m_rect.right - m_rect.left) / m_viewWidth, (m_rect.bottom - m_rect.top) / m_viewHeight, 1.0f };
	XMFLOAT3 rotation = { 0.0f, 0.0f, m_rotation };
	XMFLOAT3 location = { (m_rect.right + m_rect.left) / m_viewWidth - 1.0f, -((m_rect.bottom + m_rect.top) / m_viewHeight - 1.0f), m_depth };

	XMFLOAT4X4 scaleF4 = {
		scale.x, 0.0f, 0.0f, 0.0f,
		0.0f, scale.y, 0.0f, 0.0f,
		0.0f, 0.0f, scale.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
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
}
