#include "Sence.h"

std::function<void(std::shared_ptr<Sence> p)> Sence::switchSence = nullptr;

Sence::Sence()
{
}

Sence::~Sence()
{
}

void Sence::setSwitchSenceFunction(std::function<void(std::shared_ptr<Sence> p)> scf)
{
	switchSence = scf;
}

auto Sence::getSwitchSenceFunction()
{
	return switchSence;
}

void Sence::input(Keyboard& keyboard, Mouse& mouse, float dt)
{
}