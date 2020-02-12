#pragma once
#include "Sence.h"
#include "Sence_DriverTest.h"
#include "Car.h"

class Sence_MainMenu :
	public Sence
{
private:
	std::vector<Button*> m_buttonList;// list从上到下从小到大
	int m_buttonIndex;

	Label m_title;
	Button m_buttonChangeSence;
	Button m_buttonExit;

	std::shared_ptr<Camera> m_pCamera;
	

protected:
public:
	Sence_MainMenu();
	~Sence_MainMenu();

	bool initResource(ID3D11Device* pDevice) override;
	bool initEffect(ID3D11Device* pDevice) override;

	void update(float dt) override;
	void draw() override;

	void beforeResize();
	void afterResize();

	void keyboardInput(const Keyboard& keyboard) override; //键盘输入
	void mouseInput(const Mouse& mouse) override;// 鼠标输入
	std::shared_ptr<Camera> getCamera() override;

private:
	
};

