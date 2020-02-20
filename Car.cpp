#include "Car.h"
#include "XMF_MATH.h"

using namespace XMF_MATH;

constexpr float POWER_MAX  = 50.0F; // 最大马力
constexpr float POWER_ADD  = POWER_MAX / 4.0f; // POWER_MAX / 4.0s
constexpr float CAR_LENGTH = 2.0f; // 车身长度，假定为前后轮胎圆心距离
constexpr float WHEEL_ROT_MAX = XM_PIDIV2* 0.3333333f; // 轮胎最大旋转角度
constexpr float WHEEL_ROT_MIN = -XM_PIDIV2 * 0.3333333f;// 轮胎最小旋转角度
constexpr float WHEEL_ROT_RATE = XM_PIDIV2 * 0.5f;// 转向速率
constexpr float WHEEL_ROT_RATE_AUTO = XM_PIDIV2 * 0.2f;// 自动摆正速率
constexpr float WHEEL_ROT_PROPORTION = 1.3;// 近侧轮胎旋转角度增幅
constexpr float BRAKING_RESISTANCE = 5.0f;// 刹车阻力
constexpr float WIND_RESISTANCE_RATE = 0.5f * 0.3f * 1.293f * 0.2;// 风阻系数，乘速度的平方得风阻力
constexpr float INHERENT_RESISTANCE = 1.0f;// 固有阻力

Car::Car() :
	m_wheelRadian(0.0f),
	m_speed(0.0f),
	m_brake(0.0f),m_cameraType(0),
	m_power(0.0f), m_isPowerOn(false), m_isRotOn(false)
{
}

Car::~Car()
{
}

void Car::initCar(ID3D11Device* pDevice)
{
	init();
	m_wheelLF.init(pDevice);
	m_wheelLB.init(pDevice);
	m_wheelRF.init(pDevice);
	m_wheelRB.init(pDevice);

	size_t index = ResourceDepot::loadGeometry(pDevice, Geometry::CreateCone());
	m_body.setMeshbuffer(ResourceDepot::getMeshBuffer(index));

	index = ResourceDepot::loadDDSTesture(pDevice, L"Texture\\brick.dds", L"brick");
	m_body.setTexture(ResourceDepot::getShaderResource(index));

	m_wheelLF.setScale(0.5f, 0.1f, 0.5f);
	m_wheelLB.setScale(0.5f, 0.1f, 0.5f);
	m_wheelRF.setScale(0.5f, 0.1f, 0.5f);
	m_wheelRB.setScale(0.5f, 0.1f, 0.5f);
	m_body.setScale(0.3f, 1.0f, 0.3f);

	m_wheelLF.setPosition(-0.8f, 0.0f, CAR_LENGTH * 0.5f);
	m_wheelLB.setPosition(-0.8f, 0.0f, -CAR_LENGTH * 0.5f);
	m_wheelRF.setPosition(0.8f, 0.0f, CAR_LENGTH * 0.5f);
	m_wheelRB.setPosition(0.8f, 0.0f, -CAR_LENGTH * 0.5f);

	m_body.setRotation(90.0f, 0.0f, 0.0f);
}

void Car::init()
{
	m_pCameraF = std::make_shared<Camera_FirstPerson>();
	m_pCameraT = std::make_shared<Camera_ThirdPerson>();
	m_pCameraT->lookAt(XMFLOAT3(0.0f, 0.0f, -4.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f));
	m_pCameraF->lookAt(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f));
	m_pCameraT->setRadius(5.0f);
	m_pCamera = m_pCameraF;
	assert(m_childen.empty());
	addChild(&*m_pCameraF);// 切换摄像机记得更改这个child的值，默认放在0号位
	addChild(&*m_pCameraT);
	addChild(&m_wheelLF); // 轮子是2-5
	addChild(&m_wheelRF);
	addChild(&m_wheelLB);
	addChild(&m_wheelRB);
	addChild(&m_body);    // 车身是6
}

void Car::update(float dt)// 弃用character的速度方案
{
	// 计算熄火动力消退
	if (!m_isPowerOn)
	{
		if (m_power > 0.0f)
		{
			if (m_power < POWER_ADD)
			{
				m_power = 0.0f;
			}
			else
			{
				m_power -= POWER_ADD;
			}
		}
		else
		{
			if (m_power > -POWER_ADD)
			{
				m_power = 0.0f;
			}
			else
			{
				m_power += POWER_ADD;
			}
		}
	}
	// 修正轮胎角度
	if (m_wheelRadian < 0.0f)// 左转
	{
		m_wheelLF.rotWheel(toAngle(m_wheelRadian * WHEEL_ROT_PROPORTION));
		m_wheelRF.rotWheel(toAngle(m_wheelRadian));
	}
	else// 右转
	{
		m_wheelLF.rotWheel(toAngle(m_wheelRadian));
		m_wheelRF.rotWheel(toAngle(m_wheelRadian * WHEEL_ROT_PROPORTION));
	}

	XMVECTOR normal = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);// 车体原本方向
	XMVECTOR rot = XMQuaternionRotationRollPitchYaw(0.0f, toRadian(m_localRotation.y), 0.0f);
	normal = XMVector3Normalize(XMVector3Rotate(normal, rot));// 旋转后的方向
	// 计算速度
	if (!m_brake)
	{
		m_speed += m_power * dt;
	}
	float distance = m_speed * dt;// 车移动的路程
	// 更新轮胎旋转，轮胎是2 - 6
	for (unsigned i = 2; i < 6; ++i)
	{
		dynamic_cast<Wheel*>(m_childen[i])->rollWheel(distance);
	}
	if (m_speed == 0.0f)
	{

	}
	else if (m_speed > 0.0f)
	{
		m_speed -= (WIND_RESISTANCE_RATE * m_speed * m_speed + m_brake + INHERENT_RESISTANCE) * dt;// (风阻矢量 + 刹车制动 + 恒定阻力) * 时间
		if (m_speed < 0.0f)
		{
			m_speed = 0.0f;
		}
	}
	else
	{
		m_speed += (WIND_RESISTANCE_RATE * m_speed * m_speed + m_brake + INHERENT_RESISTANCE) * dt;
		if (m_speed > 0.0f)
		{
			m_speed = 0.0f;
		}
	}

	if (m_wheelRadian && m_speed)
	{
		if (m_brake)// 漂移
		{
			setRotation(m_localRotation + XMFLOAT3(0.0f, toAngle(WHEEL_ROT_RATE * dt), 0.0f));
			XMVECTOR pos = XMLoadFloat3(&m_localPositon);
			pos += m_brakeV * m_speed * dt;
			XMStoreFloat3(&m_localPositon, pos);
			m_trans |= Trans::locate_t;
		}
		else// 做圆弧运动
		{
			float r = CAR_LENGTH / sin(m_wheelRadian);// 圆弧运动半径
			float theta = distance / r;// 车的移动弧度
			XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
			XMVECTOR pos = XMLoadFloat3(&m_localPositon);
			XMVECTOR normalTC = XMVector3Normalize(XMVector3Cross(up, normal)); // 汽车原位置指向圆心的向量
			XMVECTOR center = normalTC * r + pos;// 圆心位置
			XMVECTOR normalCC = XMVector3Normalize(XMVector3Rotate(-normalTC, XMQuaternionRotationRollPitchYaw(0.0f, theta, 0.0f)));// 圆心指向汽车移动后位置的向量
			XMVECTOR pos2 = normalCC * r + center;// 旋转后的位置
			XMStoreFloat3(&m_localPositon, pos2);
			normal = XMVector3Normalize(XMVector3Cross(up, normalCC));// 汽车移动后的方向向量
			XMFLOAT3 nF;
			XMStoreFloat3(&nF, normal);
			float rotY = atan(nF.x / nF.z);
			// 分析象限
			if (nF.z < 0.0f)
			{
				rotY += XM_PI;
			}
			setRotation(0.0f, toAngle(rotY), 0.0f);

			m_trans |= Trans::locate_t;
		}
	}
	else// 做直线运动
	{
		XMStoreFloat3(&m_speedVector, normal * m_speed * dt);
		setPosition(m_localPositon + m_speedVector);
	}

	// 放开方向盘车轮自动偏向正轨
	if (!m_isRotOn)
	{
		float rf = WHEEL_ROT_RATE_AUTO * dt;
		if (m_wheelRadian > 0.0f)
		{
			if (m_wheelRadian < rf)
			{
				m_wheelRadian = 0.0f;
			}
			else
			{
				m_wheelRadian -= rf;
			}
		}
		else
		{
			if (m_wheelRadian > -rf)
			{
				m_wheelRadian = 0.0f;
			}
			else
			{
				m_wheelRadian += rf;
			}
		}
	}	

	m_isRotOn = false;
	m_isPowerOn = false;
	GameObject3D::update(dt);
}


void Car::straight(float dt)
{
	m_power += POWER_ADD * dt;
	if (m_power > POWER_MAX)
	{
		m_power = POWER_MAX;
	}
	m_isPowerOn = true;
}

void Car::backup(float dt)
{
	m_power -= POWER_ADD * dt * 0.5f;// 倒车加速没那么快
	if (m_power < -POWER_MAX)
	{
		m_power = -POWER_MAX;
	}
	m_isPowerOn = true;
}

void Car::left(float dt)
{
	m_wheelRadian -= WHEEL_ROT_RATE * dt;
	if (m_wheelRadian < WHEEL_ROT_MIN)
	{
		m_wheelRadian = WHEEL_ROT_MIN;
	}
	m_isRotOn = true;
}

void Car::right(float dt)
{
	m_wheelRadian += WHEEL_ROT_RATE * dt;
	if (m_wheelRadian > WHEEL_ROT_MAX)
	{
		m_wheelRadian = WHEEL_ROT_MAX;
	}
	m_isRotOn = true;
}

void Car::brakeOn(float dt)
{
	if (m_brake == 0.0f)
	{
		m_brake = BRAKING_RESISTANCE;
		m_brakeV = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
		m_brakeV = XMVector3Rotate(m_brakeV, XMQuaternionRotationRollPitchYaw(0.0f, toRadian(m_localRotation.y), 0.0f));
		m_brakeV = XMVector3Normalize(m_brakeV);
	}
}

void Car::brakeOff(float dt)
{
	if (m_brake)
	{
		m_brake = 0.0f;
		m_wheelRadian = 0.0f;
	}	
}

float Car::getSpeed() const
{
	return m_speed;
}

float Car::getCameraTRadius() const
{
	return m_pCameraT->getRadius();
}

void Car::setCameraTRadius(float radius)
{
	m_pCameraT->setRadius(radius);
}

void Car::viewRotate(const XMFLOAT3& r)
{
	XMFLOAT3 rotation = m_pCamera->getRotation() + r;
	m_pCamera->input(rotation);
}

void Car::changeCamera()
{
	m_cameraType ^= 1;
	switch (m_cameraType)
	{
	case 0:
	{
		m_pCamera = m_pCameraF;
	}break;
	case 1:
	{
		m_pCamera = m_pCameraT;
	}break;
	default:
		break;
	}
}
