#include "Sence.h"

std::function<void(std::shared_ptr<Sence> p)> Sence::m_senceChangeFunction = nullptr;

Sence::Sence()
{
}

Sence::~Sence()
{
}

void Sence::setSenceChangeFunction(std::function<void(std::shared_ptr<Sence> p)> scf)
{
	m_senceChangeFunction = scf;
}

auto Sence::getSenceChangeFunction()
{
	return m_senceChangeFunction;
}

void Sence::input(Keyboard& keyboard, Mouse& mouse)
{
}