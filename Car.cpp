#include "Car.h"
#include "XMF_MATH.h"

using namespace XMF_MATH;

#define POWER_MAX 5000.0F
#define POWER_ADD 5000.0F // POWER_MAX / 1.0s

Car::Car()
{
	addChild(&m_wheelLB);
	addChild(&m_wheelLF);
	addChild(&m_wheelRB);
	addChild(&m_wheelRF);
	addChild(&m_body);
}

Car::~Car()
{
}

void Car::initCar(ID3D11Device* pDevice)
{
	init();

	size_t index = ResourceDepot::loadGeometry(pDevice, Geometry::CreateCylinder());
	m_wheelLB.setMeshbuffer(ResourceDepot::getMeshBuffer(index));
	m_wheelLF.setMeshbuffer(ResourceDepot::getMeshBuffer(index));
	m_wheelRB.setMeshbuffer(ResourceDepot::getMeshBuffer(index));
	m_wheelRF.setMeshbuffer(ResourceDepot::getMeshBuffer(index));
	
	index = ResourceDepot::loadGeometry(pDevice, Geometry::CreateCone());
	m_body.setMeshbuffer(ResourceDepot::getMeshBuffer(index));

	index = ResourceDepot::loadDDSTesture(pDevice, L"Texture\\brick.dds", L"brick");
	m_wheelLB.setTexture(ResourceDepot::getShaderResource(index));
	m_wheelLF.setTexture(ResourceDepot::getShaderResource(index));
	m_wheelRB.setTexture(ResourceDepot::getShaderResource(index));
	m_wheelRF.setTexture(ResourceDepot::getShaderResource(index));
	m_body.setTexture(ResourceDepot::getShaderResource(index));

	m_wheelLF.setScale(0.5f, 0.1f, 0.5f);
	m_wheelLB.setScale(0.5f, 0.1f, 0.5f);
	m_wheelRF.setScale(0.5f, 0.1f, 0.5f);
	m_wheelRB.setScale(0.5f, 0.1f, 0.5f);
	m_body.setScale(0.3f, 1.0f, 0.3f);

	m_wheelLF.setPosition(-0.5f, 0.0f, 1.0f);
	m_wheelLB.setPosition(-0.5f, 0.0f, -1.0f);
	m_wheelRF.setPosition(0.5f, 0.0f, 1.0f);
	m_wheelRB.setPosition(0.5f, 0.0f, -1.0f);

	m_wheelLF.setRotation(0.0f, 0.0f, 90.0f);
	m_wheelLB.setRotation(0.0f, 0.0f, 90.0f);
	m_wheelRF.setRotation(0.0f, 0.0f, 90.0f);
	m_wheelRB.setRotation(0.0f, 0.0f, 90.0f);
	m_body.setRotation(90.0f, 0.0f, 0.0f);
}

void Car::init()
{
	m_pCameraF = std::make_shared<Camera_FirstPerson>();
	m_pCameraT = std::make_shared<Camera_ThirdPerson>();
	m_pCameraT->lookAt(XMFLOAT3(0.0f, 0.0f, -4.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f));
	m_pCameraF->lookAt(XMFLOAT3(0.0f, 0.0f, -4.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f));
	m_pCameraT->setRadius(5.0f);
	m_pCamera = m_pCameraT;
	addChild(&*m_pCamera);// 切换摄像机记得更改这个child的值，默认放在0号位
}


void Car::straight(float dt)
{
}

void Car::back(float dt)
{
}

void Car::left(float dt)
{
}

void Car::right(float dt)
{
}

void Car::stop(float dt)
{
}

void Car::viewRotate(const XMFLOAT3& r)
{
	XMFLOAT3 rotation = m_pCamera->getRotation() + r;
	m_pCamera->input(rotation);
}
